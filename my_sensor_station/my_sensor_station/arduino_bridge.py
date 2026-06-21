import rclpy
from rclpy.node import Node

from sensor_msgs.msg import Range
from std_msgs.msg import Float32

import serial

class ArduinoBridge(Node):
    def __init__(self):
        super().__init__('arduino_bridge')

        self.range_pub = self.create_publisher(Range, '/ultrasonic', 10)
        self.pot_pub = self.create_publisher(Float32, '/pot_threshold', 10)
        self.serial_port = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
        self.timer = self.create_timer(0.1, self.read_serial)

    def read_serial(self):
        if self.serial_port.in_waiting:
            line = self.serial_port.readline().decode().strip()

            try:
                dist, threshold = map(float, line.split(','))

                range_msg = Range()
                range_msg.range = dist
                self.range_pub.publish(range_msg)
                pot_msg = Float32()
                pot_msg.data = threshold
                self.pot_pub.publish(pot_msg)
                self.get_logger().info(f'Distance={dist:.2f}'
                                       f'Threshold={threshold:.2f}')
                
            except:
                pass

def main(args=None):
    rclpy.init(args=args)
    node = ArduinoBridge()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()