# Arduino Robot Project - README 🚀

## Overview 📝
This project consists of an Arduino-based robot equipped with a manipulator and a camera. The robot operates using a WiFi network to facilitate communication between its components and a remote controller. The system includes multiple modules that interact to enable the robot’s functionality.

## Functionality 🎯
- **Robot Movement:** The robot can move in various directions.
- **Manipulator Control:** The robot has a manipulator that can perform predefined tasks.
- **Camera Streaming:** The robot’s camera streams live video over the WiFi network.

  ## Videos 🎥
- [Demo Video 1](https://github.com/MarkRBro69/first_robot/blob/main/vids/manipulator-vid.mp4)
- [Demo Video 2](https://github.com/MarkRBro69/first_robot/blob/main/vids/robot-vid.mp4)

## Working Principle ⚙️
The system creates its own WiFi network, which is used to transmit data between the controller, the robot, and the camera. The communication relies on a set of firmware programs tailored for each component.

## Firmware Files 📂
The project consists of the following firmware files:

### 1. **`translate2.ino`** 📡
- **Purpose:** Configures the WiFi module to create a WiFi network.
- **Usage:** Flash this firmware onto the WiFi module to enable it to broadcast a WiFi network.

### 2. **`recive2.ino`** 🌐
- **Purpose:** Configures the WiFi module to connect to an existing WiFi network.
- **Usage:** Flash this firmware onto the WiFi module to enable it to join a specific WiFi network.

### 3. **`robot.ino`** 🤖
- **Purpose:** Implements the core functionality of the robot, including movement and manipulator control.
- **Features:**
  - Processes commands received via WiFi.
  - Controls the robot’s motors and manipulator.

### 4. **`command.ino`** 🎮
- **Purpose:** Implements the remote controller functionality.
- **Features:**
  - Sends movement and control commands to the robot.
  - Interfaces with the robot and camera via the WiFi network.

### 5. **`camera.ino`** 📷
- **Purpose:** Configures the camera module to transmit video over WiFi.
- **Features:**
  - Streams live video.
  - Supports integration with the WiFi network created by the robot.

## How to Use 🚀
1. **Setup the WiFi Network:**
   - Flash `translate2.ino` onto a WiFi module to create a network.
   - Alternatively, use `recive2.ino` to connect the system to an existing network.

2. **Program the Robot:**
   - Flash `robot.ino` onto the robot’s Arduino board.

3. **Configure the Remote Controller:**
   - Flash `command.ino` onto the controller’s Arduino board.

4. **Set Up the Camera:**
   - Flash `camera.ino` onto the camera module to enable live video streaming.

5. **Start the System:**
   - Power on all components.
   - Connect the controller to the WiFi network.
   - Use the controller to send commands to the robot.
   - View the live video stream from the camera.

## Notes 🛠️
- Ensure that all components are correctly powered and connected.
- The robot and camera must operate within the range of the WiFi network.
- Adjust the network settings in the firmware files as necessary to avoid conflicts.

## Future Improvements 🚀
- Enhance the manipulator’s capabilities with additional movements.
- Improve the camera’s resolution and streaming performance.
- Implement obstacle detection and avoidance for autonomous navigation.

## Contributing 🤝

**Contributions are welcome!**  
Feel free to fork this repository and submit pull requests for:  
- **New features**  
- **Bug fixes**  
- **Performance improvements**  

## Contact 📧
For questions or suggestions, contact the project creator, [Mark Brodovsky](https://github.com/MarkRBro69), or reach out via email at markbrodovsky@gmail.com.
