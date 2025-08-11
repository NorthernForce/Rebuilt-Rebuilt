from launch import LaunchDescription
#from launch.actions import IncludeLaunchDescription
#from launch.substitutions import PathJoinSubstitution
#from launch_ros.substitutions import FindPackageShare
import launch_ros.actions

def generate_launch_description():
    return LaunchDescription([
        launch_ros.actions.Node(
            package="robot",
            executable="robot_bridge",
            name="robot_bridge"),
        launch_ros.actions.Node(
            package="realsense2_camera",
            executable="realsense2_camera_node",
            name="realsense2_camera")
        # TODO: realsense2 node, robot_localization node, + remappings
    ])
