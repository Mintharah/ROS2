import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry

class OdomPublisher(Node):
    def __init__(self):
        super().__init__('odom_publisher')
        self.publisher_ = self.create_publisher(Odometry, "/odom", 10)
        self.timer_ = self.create_timer(0.1, self.publish_odom)
        self.x = 0.0

    def publish_odom(self):
        msg = Odometry()

        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'odom'
        msg.child_frame_id = 'base_link'

        #position
        msg.pose.pose.position.x = self.x
        msg.pose.pose.position.y = 0.0
        msg.pose.pose.position.z = 0.0

        #orientation facing forward
        msg.pose.pose.orientation.w = 0.1
        msg.pose.pose.orientation.x = 0.0
        msg.pose.pose.orientation.y = 0.0
        msg.pose.pose.orientation.z = 0.0

        #velocity
        #linear 
        msg.twist.twist.linear.x = 0.1
        msg.twist.twist.linear.y = 0.0
        msg.twist.twist.linear.z = 0.0
        #angular
        msg.twist.twist.angular.x = 0.0
        msg.twist.twist.angular.y = 0.0
        msg.twist.twist.angular.z = 0.0

        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing odom x: {self.x:.2f}')

        #increment x by 0.1
        self.x += 0.1

def main(args=None):
    rclpy.init(args=args)
    node = OdomPublisher()
    rclpy.spin(node)
    rclpy.shutdown()
