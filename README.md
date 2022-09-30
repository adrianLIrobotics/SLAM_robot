## SLAM ROS ROBOT:

![](https://img.shields.io/badge/ROS1-blue?style=for-the-badge)
![](https://img.shields.io/badge/SLAM-yellow?style=for-the-badge)
![](https://img.shields.io/badge/NAVIGATION_STACK-orange?style=for-the-badge)
![](https://img.shields.io/badge/AMLC-green?style=for-the-badge)

### Introduction:
This is the code for programming Victoria Robot with SLAM based capabilities. 
It uses ROS and Arduino Mega alongside a laptop as the main computational machine. The robot works with a Kinect 360 camera which has ROS driver support.

```mermaid
graph TD;
  Kinect-->depthimage_to_laserscan
  depthimage_to_laserscan-->navigation
  depthimage_to_laserscan-->gmapping

```

depthimage_to_laserscan 

![](https://user-images.githubusercontent.com/26432703/193227029-7cd62050-a7ac-4d19-9a58-1886f0a40c9c.png)

```
roslaunch Victoria robot_config.launch 
```

```
roslaunch Victoria slam.launch 
```
