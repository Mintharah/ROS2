#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "fstream"

class CpuTempPublisher : public rclcpp::Node{
    public:
    CpuTempPublisher() : Node("cpu_temp_publisher"){
        publisher_ = this->create_publisher<std_msgs::msg::Float32>("/cpu_temp", 10);
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&CpuTempPublisher::publish_temp, this)
        );
    }

    private:
    void publish_temp(){
        std::ifstream file("/sys/class/thermal/thermal_zone0/temp");
        int raw_temp;
        file >> raw_temp;
        
        auto msg = std_msgs::msg::Float32();
        msg.data = raw_temp / 1000.0f; //converting to celsius
        RCLCPP_INFO(this->get_logger(), "CPU Temp: %.1f °C", msg.data);
        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char* argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CpuTempPublisher>());
    rclcpp::shutdown();
    return 0;
}