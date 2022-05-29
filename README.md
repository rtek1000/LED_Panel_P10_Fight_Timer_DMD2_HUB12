Note: Work in progress, sorry.
- There is a bug that makes the screen blank related to the input_ctrl_left command (serial command: 2)

# Fight_Timer_DMD_HUB12

## Features:
- Training (fight) time count and interval count between trainings.
- - Initially done using remote to learn code (RF 433MHz), but when researching about security, it doesn't seem like a good choice.
- - Researched about the rolling code RF remote control, but it is complicated and the mid-range model with HCSxx is already being considered unsafe.
    (Search for MCS3142 if you want something safer than the HCS family)
- - The code was adapted to receive commands via the serial port, to be controlled via TCP connection using ESP-Link (ESP8266) transparently.
- - Remote control would be performed via Android smartphone App.

## Software:
- Code written in Arduino IDE
- - Using the ESP link allows you to use an ATmega328 (Uno/Pro Mini/Nano) as if it were the Arduino Uno Wifi model.

## Hardware:
- Arduino Pro Mini 5V 16MHz
- P10 Panel 32x16 (Red led)
- Power bank Pineng PN-951 (Good current capacity (up to 2 Amps), low standby consumption, charge indicator via LED, slim body)
- ESP8266 (Firmware esp-link v3.2.47-g9c6530d)
- ENC28J60 (Not implemented yet)

## Licence:

#### Hardware:
Released under CERN OHL 1.2: https://ohwr.org/cernohl

#### Software:
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
