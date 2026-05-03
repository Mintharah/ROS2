#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

const double MAX_LINEAR = 1.0;
const double MAX_ANGULAR = 1.5;

class CmdVelLimiter : public rclcpp::Node {
    public:
    CmdVelLimiter() : Node("cmd_vel_limiter"){
        subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel", 10, std::bind(&CmdVelLimiter::cmd_vel_callback, this, std::placeholders::_1)
        );
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel_limited",10);
    }

    private:

    void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg){
        auto limited = geometry_msgs::msg::Twist();

        limited = *msg;

        if(std::abs(msg->linear.x) > MAX_LINEAR){
            RCLCPP_WARN(this->get_logger(), "Limiting linear speed to %.1f m/s, Speed was: %.2f m/s", MAX_LINEAR, msg->linear.x);
            limited.linear.x = (msg->linear.x > 0) ? MAX_LINEAR : -MAX_LINEAR;
        }

        if(std::abs(msg->angular.z) > MAX_ANGULAR){
            RCLCPP_WARN(this->get_logger(), "Limiting angular speed to %.1f rad/s, Speed was: %.2f rad/s", MAX_ANGULAR, msg->angular.z);
            limited.angular.z = (msg->angular.z > 0) ? MAX_ANGULAR : -MAX_ANGULAR;
        }

        publisher_->publish(limited);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscriber_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char* argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<CmdVelLimiter>());
    rclcpp::shutdown();
    return 0;
}