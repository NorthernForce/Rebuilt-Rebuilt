from setuptools import find_packages, setup
from glob import glob

package_name = 'robot'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', glob("launch/*"))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Team 172',
    maintainer_email='team172@northernforcerobotics.org',
    description='A ROS2 system to handle SLAM and other positioning systems.',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'robot_bridge = robot.robot_bridge:main'
        ],
    },
)
