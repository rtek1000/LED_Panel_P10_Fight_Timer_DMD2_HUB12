Note: Work in progress, sorry.
- There is a bug that leaves the screen blank related to the show_clock() subroutine

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

## Notes:
https://github.com/jeelabs/esp-link/blob/master/FLASHING.md#initial-serial-flashing

4Mbit / 512Kbyte module

For Release 3.2.47 alpha Pre-release:
esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash -fs 4m -ff 40m 0x00000 boot_v1.7.bin 0x1000 user1.bin 0x7C000 esp_init_data_default.bin 0x7E000 blank.bin

The esp-link firmware connects a micro-controller to the internet using an ESP8266 Wifi module. It implements a number of features:

- transparent bridge between Wifi and serial, useful for debugging or inputting into a uC
- flash-programming attached Arduino/AVR microcontrollers and LPC800-series and other ARM microcontrollers via Wifi
- built-in stk500v1 programmer for AVR uC's: program using HTTP upload of hex file
- outbound REST HTTP requests from the attached micro-controller to the internet
- MQTT client pub/sub from the attached micro-controller to the internet
- serve custom web pages containing data that is dynamically pulled from the attached uC and that contain buttons and fields that are transmitted to the attached uC (feature not fully ready yet)

For flash-programming (OTA):
Service mDNS (enabled): arduino

Arduino IDE board: Arduino Uno Wifi

Hardware configuration for normal operation
This firmware is designed for any esp8266 module. The recommended connections for an esp-01 module are:

- URXD: connect to TX of microcontroller
- UTXD: connect to RX of microcontroller
- GPIO0: connect to RESET of microcontroller
- GPIO2: optionally connect green LED to 3.3V (indicates wifi status)

## Licence:

#### Hardware:
Released under CERN OHL 1.2: https://ohwr.org/cernohl

#### Software:
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
