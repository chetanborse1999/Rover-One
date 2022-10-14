# Rover-One
A Rover platform based on a Rocker Bogie suspension.

## About Project
The project demonstrates the Rocker Bogie suspension used in Mars Rovers. The assembly enables the chassis to remain stable over uneven surfaces and improves traction in soft terrain like sand. 

<p align="center">
  <img src="https://i.imgur.com/sv3pIcDl.jpg" alt="Home View" height="250">
  <img src="https://i.imgur.com/5bUla6il.jpg" alt="Side View" height="250">
  <br>
  <b>Fig. 1 and 2</b>
</p>

The robot has 6 wheels mounted on a servo that enables turning at variable turning radius, spot turns and strafing. The Rocker-Bogie assembly on each side is connected via a differential bar that maintains the chassis pitch on uneven surfaces. The chassis maintains an average pitch of both rockers.

<p align="center">
  <a href="https://youtu.be/0y1WFYk78O0" target="_blank"><img src="https://i.imgur.com/tHX0DIA.png" alt="Showing Rocker Movement" height="300"></a>
  <br><b>Showing movement of Rockers and Differential bar</b>
</p>

The robot has 6 Johnson motors and 6 MG995 servos powered by a Lithium Polymer battery. The actuators are controlled by Arduino Mega2560 and L298N H-bridge motor drivers. For communication, FS-i6/FS-ia6b radio transmitter/receiver is used. 

## CAD Model
Designed in Fusion360. Files are included. The chassis is made of 3x2 (35mmx23mm) aluminium tube. The remaining parts are 3d printed. 

<p align="center">
  <img src="https://i.imgur.com/btrubIJl.png" alt="CAD perspective", height="250">
  <br>
  <b>Fig. 3 - Fusion360 Assembly</b>
</p>

## Hardware

1. Aluminium Tube Chassis.
2. 6 x MG995 metal gear servo with Disc metal horns
3. 6 x 12V Johnson Motors 200RPM with mounts
4. Arduino Mega2560
5. 3 x L298N motor drivers
6. DC-DC buck convertor
7. FS-i6/FS-ia6b Radio Transmitter/Receiver
8. Lipo Battery
9. Jumper Wires
10. Fasteners

## Instruction to Run

1. Make connections using jumper for motor drivers, servos and radio RX module (not required for serial). Pins are defined in the accompanying header file. Ensure that Enable pins are not using pins 44, 45 and 46 on Mega since they do not have pwm when Servo.h is used. 
2. Map the value to be written to servos such that wheel is in forward position when writing '90' as shown in fig. 1.
3. Upload code to Mega. 
4. Power the motors with 12v and Servos with 5v from a voltage regulator, preferably buck convertor. Ensure that microcontroller has a seperate power source as fluctuations can cause it to reboot.
5. On the Transmitter, configure channel 5 and 6 to swA and swC.
6. Two Methods to run:
    
    1. To run using Radio transmitter: Power the Mega, Turn on the radio Transmitter. Set swC to position 2. Now connect the battery. All servos should move to default position and can be steered using right stick. Now move swA to position 2 to arm the motors. 
    
        The bot is now mobile. Use right stick to give move forward/backward as well as steer. To switch to strafe mode, move swC to position 1 and use left stick to strafe. To switch to spot rotation mode, move swC to position 3 and use left stick to spot rotate right/left. swA can be used in any mode to arm or disarm the motors.

    2. To run using Serial, upload serial_ctrl code. Ensure jumpers are connected to pins specified in the code. A Standard USB or USB to TTL convertor can be used (Only RX pin on arduino required, unless debugging). Now launch the accompanying python script 'get_keys.py'. The keyboard library is required to run this script. Now standard WASD controls can be used to control the bot. Strafe mode not available yet.



## Videos

<p align="center">
  <a href="https://youtu.be/RismbZGRqcw" target="_blank"><img src="https://i.imgur.com/MRKLm68l.png" alt="Showing different drive modes" width="450"></a>
  <br><b>Different drive modes</b><br><br>
  <a href="https://youtu.be/btVR41I81UQ" target="_blank"><img src="https://i.imgur.com/gzSdu4Nl.png" alt="Showing going over some obstacle" width="450"></a>
  <br><b>Going over an obstacle</b><br>
</p>
  
