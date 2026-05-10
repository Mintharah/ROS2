#include "simple_turtle_patrol/msg/robot_status.hpp"
#include <cmath>
#include <geometry_msgs/msg/pose2_d.hpp>
#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>

class StatusPublisher : public rclcpp::Node {
public:
  StatusPublisher() : Node("status_publisher") {
    pub_ = this->create_publisher<simple_turtle_patrol::msg::RobotStatus>(
        "/robot/status", 10);

    sub_ = this->create_subscription<turtlesim::msg::Pose>(
        "/turtle1/pose", 10, [this](const turtlesim::msg::Pose::SharedPtr msg) {
          this->pose_callback(msg);
        });

    this->declare_parameter("status_rate", 5.0);
    double rate = this->get_parameter("status_rate").as_double();

    timer_ = this->create_wall_timer(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double>(1.0 / rate)),
        [this]() { this->publish_status(); });
  }

private:
  void pose_callback(const turtlesim::msg::Pose::SharedPtr msg) {
    current_pose_ = *msg;

    if (first_pose_) {
      last_theta_ = msg->theta;
      first_pose_ = false;
      return;
    }

    if (std::fabs(msg->linear_velocity) < 0.1) {
      state_ = "stopped";
    } else {
      state_ = "running";
    }

    double delta = msg->theta - last_theta_;
    if (delta > M_PI)
      delta -= 2.0 * M_PI;
    if (delta < -M_PI)
      delta += 2.0 * M_PI;

    cumulative_angle_ += delta;
    last_theta_ = msg->theta;

    // count laps, every 2*pi is a full circle
    int new_laps =
        static_cast<int>(std::fabs(cumulative_angle_) / (2.0 * M_PI));
    if (new_laps > lap_count_) {
      lap_count_ = new_laps;
      RCLCPP_INFO(this->get_logger(), "Lap %d completed!", lap_count_);
    }
  }

  void publish_status() {
    auto msg = simple_turtle_patrol::msg::RobotStatus();

    msg.pose.x = current_pose_.x;
    msg.pose.y = current_pose_.y;
    msg.pose.theta = current_pose_.theta;

    msg.state = state_;
    msg.lap_count = lap_count_;

    // Dummy temperature - increases with speed and lap count
    msg.temperature = 20.0f + (lap_count_ * 0.5f) +
                      std::fabs(current_pose_.linear_velocity) * 5.0f;

    pub_->publish(msg);
  }

  turtlesim::msg::Pose current_pose_{};
  std::string state_{"running"};
  int lap_count_{0};
  double last_theta_{0.0};
  double cumulative_angle_{0.0};
  bool first_pose_{true};

  rclcpp::Publisher<simple_turtle_patrol::msg::RobotStatus>::SharedPtr pub_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatusPublisher>());
  rclcpp::shutdown();
  return 0;
}