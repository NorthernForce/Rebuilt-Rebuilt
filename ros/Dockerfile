ARG ROS_DISTRO="jazzy"

FROM ros:${ROS_DISTRO} AS realsense-build
ARG RSVER="2.57.2"

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential cmake git pkg-config \
    libssl-dev libusb-1.0-0-dev \
    libgtk-3-dev libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev \    
    curl python3 python3-dev ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /ws
RUN curl https://codeload.github.com/IntelRealSense/librealsense/tar.gz/refs/tags/v$RSVER -o librealsense.tar.gz
# RUN cat librealsense.tar.gz
RUN tar -xzf librealsense.tar.gz && rm librealsense.tar.gz
RUN ln -s /ws/librealsense-$RSVER /ws/librs

WORKDIR /ws/librealsense-$RSVER/build
RUN cmake .. \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_PYTHON_BINDINGS=ON \
    -DBUILD_GLSL_EXTENSIONS=ON \
    -DCHECK_FOR_UPDATES=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/opt/librs
RUN make -j$(nproc) all
RUN make install

FROM ros:${ROS_DISTRO} AS robot-sys

COPY --from=realsense-build /opt/librs /usr/local/
COPY --from=realsense-build /ws/librs/config/99-realsense-libusb.rules /etc/udev/rules.d/
COPY --from=realsense-build /ws/librs/config/99-realsense-d4xx-mipi-dfu.rules /etc/udev/rules.d/

RUN apt-get update && apt-get upgrade -y

RUN apt-get install -y --no-install-recommends \	
    libusb-1.0-0 udev \
    apt-transport-https ca-certificates \
    curl iputils-ping usbutils \
    software-properties-common

RUN apt-get install -y \
    ros-${ROS_DISTRO}-realsense2-camera \
    ros-${ROS_DISTRO}-robot-localization

WORKDIR /robot_ws
COPY . .
RUN colcon build
RUN . install/local_setup.sh
CMD ["rs-enumerate-devices"]
#CMD ["ros2 launch robot vision_launch.py"]

# TODO: rosdep setup, colcon build step, and start cmd