# SmartScale 2
Modification of a kitchen scale with an ESP32 and a HX711 on a custom PCB.

This project is powered by the amazing [ESP32 SvelteKit](https://github.com/theelims/ESP32-sveltekit) framework.

## Software Features
- Automatic hotspot with captive portal if no WiFi is found
- MQTT with auto discovery for HomeAssistant

Be sure to select ESP32-C6 as the target in PlatformIO when using the provided PCBd

## Hardware Features
- HX711 as the loadcell measurement ic with additional filtering
- Builtin battery charger for single cell LiPo batteries with status led
- Powered via USB or battery
- Better power efficiency due to buck switching regulator instead of LDO
- WiFi, Bluetooth and Zigbee compatible due to the ESP32-C6
- touch ic for one button
- easily expandable with qwiic connector (I2C)
- debug with the UART port or the JTAG header
- ESD protection for the USB connector

> [!IMPORTANT]
> The **qwiic and UART** port (J5, J6) have no level shifting and are therefore **not 5V tolerant** (3.3V only)!

You can find the [schematic here](/hardware/SmartScale2-Schematic.pdf)
