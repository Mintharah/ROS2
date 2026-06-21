#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/empty.hpp>

class PatrolController : public rclcpp::Node {
public:
  PatrolController() : Node("patrol_controller"), running_(true) {
    this->declare_parameter("linear_speed", 1.5);
    this->declare_parameter("angular_speed", 1.0);

    cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>(
        "/turtle1/cmd_vel", 10);

    timer_ = this->create_wall_timer(std::chrono::milliseconds(100),
                                     [this]() { this->publish_velocity(); });

    stop_srv_ = this->create_service<std_srvs::srv::Empty>(
        "/stop", [this](const std::shared_ptr<std_srvs::srv::Empty::Request>,
                        std::shared_ptr<std_srvs::srv::Empty::Response>) {
          running_ = false;
          geometry_msgs::msg::Twist stop_msg;
          cmd_vel_pub_->publish(stop_msg);
        });

    continue_srv_ = this->create_service<std_srvs::srv::Empty>(
        "/continue", [this](const std::shared_ptr<std_srvs::srv::Empty::Request>,
                           std::shared_ptr<std_srvs::srv::Empty::Response>) {
          running_ = true;
        });
  }

private:
  void publish_velocity() {
    if (!running_)
      return;

    double linear_speed = this->get_parameter("linear_speed").as_double();
    double angular_speed = this->get_parameter("angular_speed").as_double();

    geometry_msgs::msg::Twist msg;
    msg.linear.x = linear_speed;
    msg.angular.z = angular_speed;
    cmd_vel_pub_->publish(msg);
  }
  bool running_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr stop_srv_;
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr continue_srv_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PatrolController>());
  rclcpp::shutdown();
  return 0;
}