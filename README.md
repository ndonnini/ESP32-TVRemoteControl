# ESP32-TVRemoteControl
Wifi Remote Control for TV with ESP32 and IR Transmitter

Wifi Remote Control developed for a Samsung TV model UN50RU7100GXZD, to be accessed through any device connected to the same Wifi network as the ESP32 with the IR transmitter.

Materials needed:
1 ESP32
1 IR Led transmitter
1 2N3904 NPN transistor
1 68 ohm resistor for the IR Led
1 330 ohm resistor for the transistor emitter pin

Arduino libraries used
https://github.com/Arduino-IRremote/Arduino-IRremote (first attempts with an Arduino Nano for learning, not applicable for ESP32)
https://github.com/crankyoldgit/IRremoteESP8266 (actual controls used for ESP32)
