import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
import os
import csv

class ImuPublisher(Node):
    def __init__(self):
        super().__init__('imu_publisher')
        self.publisher_ = self.create_publisher(Imu, "/imu/data", 10)
        self.timer_ = self.create_timer(0.1, self.publish_imu)

        #load the csv
        csv_path = os.path.expanduser('~/catkin_ws/src/imu_publisher/data/imu_data.csv')
        with open(csv_path, 'r') as f:
            reader = csv.DictReader(f)
            self.imu_data = list(reader)

        self.index = 0
        self.get_logger().info(f'Loaded {len(self.imu_data)} rows from {csv_path}')

    def publish_imu(self):
        row = self.imu_data[self.index]
        
        msg = Imu()

        #header
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'base_link'

        #orientation
        # Orientation
        msg.orientation.x = float(row['orient_x'])
        msg.orientation.y = float(row['orient_y'])
        msg.orientation.z = float(row['orient_z'])
        msg.orientation.w = float(row['orient_w'])

        # Angular velocity
        msg.angular_velocity.x = float(row['ang_x'])
        msg.angular_velocity.y = float(row['ang_y'])
        msg.angular_velocity.z = float(row['ang_z'])

        # Linear acceleration
        msg.linear_acceleration.x = float(row['acc_x'])
        msg.linear_acceleration.y = float(row['acc_y'])
        msg.linear_acceleration.z = float(row['acc_z'])
        
        #covariance (all zeros = unknown)
        msg.orientation_covariance = [0.0] * 9
        msg.angular_velocity_covariance = [0.0] * 9
        msg.linear_acceleration_covariance = [0.0] * 9

        self.publisher_.publish(msg)
        self.get_logger().info(f"Publishing row[{self.index}]: acc_x={row['acc_x']}, acc_y={row['acc_y']}, acc_z={row['acc_z']}")
        #loopback to start
        self.index = (self.index + 1) % len(self.imu_data)

def main(args=None):
    rclpy.init(args=args)
    node = ImuPublisher()
    rclpy.spin(node)
    rclpy.shutdown()