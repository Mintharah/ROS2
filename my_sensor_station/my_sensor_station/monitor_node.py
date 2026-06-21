import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Range
from std_msgs.msg import Float32
from std_msgs.msg import Bool
from rclpy.parameter import Parameter
from sensor_msgs.msg import LaserScan

class MonitorNode(Node):
    def __init__(self):
        super().__init__('monitor_node')
    

        # default parameter
        self.declare_parameter('alert_threshold',0.8)
        self.threshold = self.get_parameter('alert_threshold').value
        self.current_distance = 999.0
        self.lidar_min = float('inf')

        # publishers
        self.min_pub = self.create_publisher(Float32, '/min_distance', 10)
        self.alert_pub = self.create_publisher(Bool, '/alert', 10)

        # subscribers
        self.create_subscription(Range, 'ultrasonic', self.ultra_callback, 10)
        self.create_subscription(Float32, '/pot_threshold', self.threshold_callback,10)
        self.create_subscription(LaserScan, '/scan', self.scan_callback, 10)

        #timer
        self.timer = self.create_timer(0.05, self.publish_status)
    
    def ultra_callback(self, msg):
        self.current_distance = msg.range

    def threshold_callback(self, msg):
        self.threshold = msg.data
        self.set_parameters([
            Parameter(
                'alert_threshold',
                Parameter.Type.DOUBLE,
                msg.data
            )
    ])
        
    def publish_status(self):
    
        # Convert LiDAR to cm
        lidar_cm = self.lidar_min * 100
    
        # Compute minimum
        combined_min = min(
            self.current_distance,
            lidar_cm
        )
    
        # Determine source
        if combined_min == self.current_distance:
            source = "Ultrasonic"
        else:
            source = "LiDAR"
    
        # Publish min distance
        min_msg = Float32()
    
        min_msg.data = combined_min
    
        self.min_pub.publish(min_msg)
    
        # Publish alert
        alert_msg = Bool()
    
        alert_msg.data = (
            combined_min < self.threshold
        )
    
        self.alert_pub.publish(alert_msg)
    
        # Debug print
        self.get_logger().info(
            f'Min={combined_min:.2f} cm '
            f'Source={source} '
            f'Ultra={self.current_distance:.2f} cm '
            f'LiDAR={lidar_cm:.2f} cm '
            f'Alert={alert_msg.data}'
        )
    
    def scan_callback(self, msg):

        valid_ranges = [
            r for r in msg.ranges
            if r > 0.0
            and r != float('inf')
        ]

        if valid_ranges:
            self.lidar_min = min(valid_ranges)
    


def main(args=None):
    rclpy.init(args=args)
    node = MonitorNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()