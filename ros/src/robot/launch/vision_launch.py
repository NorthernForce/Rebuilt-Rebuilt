import launch
import launch_ros.actions

def generate_launch_description():
    return launch.LaunchDescription([
        launch_ros.actions.Node(
            package="robot",
            executable="robot_bridge",
            name="robot_bridge"
        )
        # TODO: realsense2 node, robot_localization node, + remappings
    ])
