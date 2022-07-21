Note: Work in progress, sorry.

# LED Panel P10 Fight Timer DMD2 HUB12

## Features:
- Training (fight) time count and interval count between trainings.
- - Countdown time.
- - Panel brightness adjustment.
- - Allows user to see date and time while counting (makes automatic return).
- - Automatically return when idle within the menu tree.
- - Audio visual signaling, beep and voice (mp3 files)
- - Independent volume control for beep and voice (player has its volume control).
- - Playing music on USB flash drive and SD card (player also has FM and Bluetooth).
- - Panel operation is independent of mp3 player (shares audio output).
- - Displayed temperature range from 2 to 150 °C (Lower temperature needs different hardware).


- Notes:
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
- Power bank Pineng PN-951 (Good current capacity (up to 2 Amps), low standby consumption, charge indicator via LED, slim body, 10000mA 3.7V battery, CI IP5306)
- ESP8266 (Firmware esp-link v3.2.47-g9c6530d)
- ENC28J60 (Not implemented yet)
- DFPlayer Mini (It may be interesting to normalize the audio before playing)
- - See this app: replaygain
- - sudo apt-get install replaygain
- - - replaygain -r -1 *.mp3
- - https://en.wikipedia.org/wiki/ReplayGain
- - ---> ![Audacity](https://www.audacityteam.org/) software performs better <---

## Licence:

#### Hardware:
Released under CERN OHL 1.2: https://ohwr.org/cernohl

#### Software:
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
