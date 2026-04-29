#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

class PublisherB : public rclcpp::Node {
    public:
    PublisherB() : Node("publisher_b"), count_(100){
        pub_ = this->create_publisher<std_msgs::msg::Int32>("topic_b", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(1000),
            std::bind(&PublisherB::timer_cb, this));
    }

    private:
    void timer_cb(){
        auto msg = std_msgs::msg::Int32();
        msg.data = count_++;
        RCLCPP_INFO(this->get_logger(), "Publishing B: %d", msg.data);
        pub_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    int32_t count_;
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherB>());
    rclcpp::shutdown();
    return 0;
}