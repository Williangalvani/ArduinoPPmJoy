# ArduinoPPMJoy

This software allows you to use a regular RC radio transmitter PPM output as a HID Joystick using an Arduino.

# Setup

## Linux (Ubuntu)
Make sure to use arduino Uno with atmega8u2 or atmega16u2 as usb interface
First, upload via Arduino the ArduinoPPMJoy.ino sketch, then, to change the atmega8u2 or atmega16u2 firmware, short the ISCP header reset and gnd pins, as described [here](https://www.arduino.cc/en/Hacking/DFUProgramming8U2). Then, to flash the HID firmware:
  
  sudo apt-get install dfu-programmmer
  git clone [this repo]
  cd ArduinoPPmJoy/firmware
  make
  sudo dfu-programmer atmega16u2 erase
  sudo dfu-programmer atmega16u2 flash Arduino-big-joystick.hex

now reconnect the arduino to the computer.

## Windows
On windows, you can use the  Atmel's flip programmer.


## Using
Connect PPM signal to pin 3 and GND, plug into the PC, and it should work.
