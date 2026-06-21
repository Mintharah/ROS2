import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    pkg_share = get_package_share_directory('simple_turtle_patrol')
    params_file = os.path.join(pkg_share, 'params', 'patrol_params.yaml')

    return LaunchDescription([
        Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim',
        output='screen',
        ),
        Node(
        package='simple_turtle_patrol',
        executable='status_publisher',
        name='status_publisher',
        output='screen',
        parameters=[params_file],
        ),
        Node(
        package='simple_turtle_patrol',
        executable='patrol_controller',
        name='patrol_controller',
        output='screen',
        parameters=[params_file],
        )
    ])