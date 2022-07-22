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
- - Battery charge level indication (Controlled by the PN951 power bank).

- Usage temperature up to 45°C maximum due to PN-951 power bank battery.

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
-  ̶E̶S̶P̶8̶2̶6̶6̶ ̶(̶F̶i̶r̶m̶w̶a̶r̶e̶ ̶e̶s̶p̶-̶l̶i̶n̶k̶ ̶v̶3̶.̶2̶.̶4̶7̶-̶g̶9̶c̶6̶5̶3̶0̶d̶)̶
-  ̶E̶N̶C̶2̶8̶J̶6̶0̶ ̶(̶N̶o̶t̶ ̶i̶m̶p̶l̶e̶m̶e̶n̶t̶e̶d̶ ̶y̶e̶t̶)̶
- DFPlayer Mini (It may be interesting to normalize the audio before playing)
- -  ̶S̶e̶e̶ ̶t̶h̶i̶s̶ ̶a̶p̶p̶:̶ ̶r̶e̶p̶l̶a̶y̶g̶a̶i̶n̶
- -  ̶s̶u̶d̶o̶ ̶a̶p̶t̶-̶g̶e̶t̶ ̶i̶n̶s̶t̶a̶l̶l̶ ̶r̶e̶p̶l̶a̶y̶g̶a̶i̶n̶
- - -  ̶r̶e̶p̶l̶a̶y̶g̶a̶i̶n̶ ̶-̶r̶ ̶-̶1̶ ̶*̶.̶m̶p̶3̶
- -  ̶h̶t̶t̶p̶s̶:̶/̶/̶e̶n̶.̶w̶i̶k̶i̶p̶e̶d̶i̶a̶.̶o̶r̶g̶/̶w̶i̶k̶i̶/̶R̶e̶p̶l̶a̶y̶G̶a̶i̶n̶
- - ---> ![Audacity](https://www.audacityteam.org/) software performs better <---

## Licence:

#### Hardware:
Released under CERN OHL 1.2: https://ohwr.org/cernohl

#### Software:
This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
