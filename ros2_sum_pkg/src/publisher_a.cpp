#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

class PublisherA : public rclcpp::Node
{
    public:
    PublisherA() : Node("publisher_a"), count_(0){
        pub_ = this->create_publisher<std_msgs::msg::Int32>("topic_a", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&PublisherA::timer_cb,this));
    }

    private:
    void timer_cb(){
        auto msg = std_msgs::msg::Int32();
        msg.data = count_++;
        RCLCPP_INFO(this->get_logger(), "Publishing A: %d", msg.data);
        pub_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    int32_t count_;
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherA>());
    rclcpp::shutdown();
    return 0;
}