# SmartScale 2
Modification of a kitchen scale with an ESP32 and a HX711 on a custom PCB. But you can use
this project for other usecases as well, such as measuring the weight of your bee hive, to
measure things you put in your inventory system, to track the weight of the food bowl for
your pets etc.

This project is powered by the amazing [ESP32 SvelteKit](https://github.com/theelims/ESP32-sveltekit) framework.

## Software Features
- Automatic hotspot with captive portal if no WiFi is found
- MQTT with auto discovery for HomeAssistant

> [!NOTE]
> Be sure to select `env:esp32-c6-devkit` as the PlatformIO project environment/target
> when using the provided PCB.



## Hardware Features
- HX711 as the loadcell measurement ic with additional filtering
- Builtin battery charger for single cell LiPo batteries with status led
- Powered via USB or battery
- Better power efficiency due to buck switching regulator instead of LDO
- WiFi, Bluetooth and Zigbee compatible due to the ESP32-C6
- touch ic for one button
- easily expandable with qwiic connector (I²C)
- debug with the UART port or the JTAG header
- ESD protection for the USB connector

> [!IMPORTANT]
> The **qwiic and UART** port (J5, J6) have no level shifting and are therefore
> **not 5V tolerant** (3.3V only)!
>
> Also the **battery** connector has **no reverse polarity protection**. Check the pinout of your
> Molex PicoBlade connector (sometimes called JST 1.25 by sketchy online stores) before
> you connect the battery!

You can find the [schematic here](/hardware/SmartScale2-Schematic.pdf)
