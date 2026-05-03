import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32, Bool

class StopPublisher(Node):
    def __init__(self):
        super().__init__('stop_publisher')
        self.subscriber_ = self.create_subscription(
            Float32, "/sensor/distance", self.distance_callback, 10
        )
        self.publisher_ = self.create_publisher(Bool, "/cmd/stop", 10)
    
    def distance_callback(self,msg):
        stop_msg = Bool()
        stop_msg.data = msg.data < 2.0
        self.get_logger().info(f'Distance: {msg.data:.2f} -> Stop: {stop_msg.data}')
        self.publisher_.publish(stop_msg)

def main(args=None):
    rclpy.init(args=args)
    node = StopPublisher()
    rclpy.spin(node)
    rclpy.shutdown()

