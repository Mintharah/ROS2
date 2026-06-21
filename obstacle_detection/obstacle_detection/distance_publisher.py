import rclpy
import random
from rclpy.node import Node
from std_msgs.msg import Float32

class DistancePublisher(Node):
    def __init__(self):
        super().__init__('distance_publisher')
        self.publisher_ = self.create_publisher(Float32, "/sensor/distance", 10)
        self.timer_ = self.create_timer(0.1, self.publish_distance)

    def publish_distance(self):
        msg = Float32()
        msg.data = random.uniform(0.03, 5.0)
        self.get_logger().info(f'Publishing distance: {msg.data:.2f}')
        self.publisher_.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = DistancePublisher()
    rclpy.spin(node)
    rclpy.shutdown