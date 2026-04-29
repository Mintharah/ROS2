#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

class SumSubscriber : public rclcpp::Node {
public:
  SumSubscriber()
      : Node("sum_subcriber"), a_(0), b_(0), got_a_(false), got_b_(false) {
    sub_a_ = this->create_subscription<std_msgs::msg::Int32>(
        "topic_a", 10, [this](std_msgs::msg::Int32::SharedPtr msg) {
          a_ = msg->data;
          got_a_ = true;
          compute_and_publish();
        });
    sub_b_ = this->create_subscription<std_msgs::msg::Int32>(
        "topic_b", 10, [this](std_msgs::msg::Int32::SharedPtr msg) {
          b_ = msg->data;
          got_b_ = true;
          compute_and_publish();
        });

    sum_pub_ = this->create_publisher<std_msgs::msg::Int32>("topic_sum", 10);
  }

private:
  void compute_and_publish() {
    if (!got_a_ || !got_b_)
      return;
    int32_t sum = a_ + b_;
    RCLCPP_INFO(this->get_logger(), "Sum = %d + %d = %d", a_, b_, sum);
    auto msg = std_msgs::msg::Int32();
    msg.data = sum;
    sum_pub_->publish(msg);
  }
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_a_, sub_b_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr sum_pub_;
  int32_t a_, b_;
  bool got_a_, got_b_;
};

int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SumSubscriber>());
    rclcpp::shutdown();
    return 0;
}