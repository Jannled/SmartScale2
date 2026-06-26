/**
 *   ESP32 SvelteKit
 *
 *   A simple, secure and extensible framework for IoT projects for ESP32 platforms
 *   with responsive Sveltekit front-end built with TailwindCSS and DaisyUI.
 *   https://github.com/theelims/ESP32-sveltekit
 *
 *   Copyright (C) 2018 - 2023 rjwats
 *   Copyright (C) 2023 - 2025 theelims
 *
 *   All Rights Reserved. This software may be modified and distributed under
 *   the terms of the LGPL v3 license. See the LICENSE file for details.
 **/

#include <ESP32SvelteKit.h>
#include <PsychicHttpServer.h>

#include <U8g2lib.h>

#include "Service/ScaleService.hpp"
#include "Service/ScaleMqttSettingsService.hpp"

#define SERIAL_BAUD_RATE 115200

PsychicHttpServer server;

ESP32SvelteKit esp32sveltekit(&server, 70);

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const long LOADCELL_OFFSET = 50682624;
const long LOADCELL_DIVIDER = 5895655;
HX711 loadcell = HX711();

#define OLED_DISPLAY_SDA 22
#define OLED_DISPLAY_SCL 23
#define DISPLAY_HEADER_HEIGHT 10
U8G2_SSD1306_128X64_NONAME_2_HW_I2C u8g2(
	U8G2_R0, 255, OLED_DISPLAY_SCL, OLED_DISPLAY_SDA // rotation, reset, scl, sda
);

ScaleMqttSettingsService scaleMqttSettingsService = ScaleMqttSettingsService(
	&server,
	&esp32sveltekit);

ScaleService scaleService = ScaleService(
	&server,
	&esp32sveltekit,
	&scaleMqttSettingsService,
	&loadcell);

void setup()
{
	// start serial and filesystem
	Serial.begin(SERIAL_BAUD_RATE);

	// Initialize the load cell hardware
	loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
	loadcell.set_scale(LOADCELL_DIVIDER);
	loadcell.set_offset(LOADCELL_OFFSET);

	Serial.print("Weight: ");
	Serial.println(loadcell.get_units(10), 2);

	// Init the display
	u8g2.begin();

	// start ESP32-SvelteKit
	esp32sveltekit.begin();

	// Launch the Scale Service
	scaleService.begin();
	scaleMqttSettingsService.begin();
}

void updateOLED(float reading)
{
	u8g2.firstPage();
	do
	{
		// Print the weight in gram/kilogram
		static char text[8];
		if(reading < 10000)
			snprintf(text, sizeof(text), "%ldg", (int) reading);
		else
			snprintf(text, sizeof(text), "%.1fkg", (float) reading/1000);

		// https://github.com/olikraus/u8g2/wiki/fnticons#siji-pixel-icons
		u8g2.setFont(u8g2_font_siji_t_6x10); // 11px

		// Draw signal strength
		if(WiFi.getMode() == WIFI_MODE_STA || WiFi.getMode() == WIFI_MODE_APSTA)
		{
			if(WiFi.status() == WL_CONNECTED)
			{
				if(WiFi.RSSI() > -2) // implausible signal
					u8g2.drawUTF8(0, DISPLAY_HEADER_HEIGHT, "\uE21B");
				else if(WiFi.RSSI() > -55) // > -55dBm strong signal
					u8g2.drawUTF8(0, DISPLAY_HEADER_HEIGHT, "\uE218");
				else if(WiFi.RSSI() > -70) // > -70dBm medium signal
					u8g2.drawUTF8(0, DISPLAY_HEADER_HEIGHT, "\uE219");
				else // < -70 weak signal
					u8g2.drawUTF8(0, DISPLAY_HEADER_HEIGHT, "\uE21A");
			}
			else
				u8g2.drawUTF8(0, DISPLAY_HEADER_HEIGHT, "\uE217"); // Not connected icon
		}
		else
		{
			u8g2.drawUTF8(0, DISPLAY_HEADER_HEIGHT, "\uE02D"); // "\uE0AC"
		}

		// Draw battery icon
		u8g2.drawUTF8(u8g2.getDisplayWidth() - 12, DISPLAY_HEADER_HEIGHT, "\uE24B"); // \ue242 - \ue24B

		// Draw IP-address
		u8g2.setFont(u8g2_font_littlemissloudonbold_tr); // 8px
		if(WiFi.getMode() == WIFI_MODE_STA || WiFi.getMode() == WIFI_MODE_APSTA)
		{
			u8g2.drawStr(
				12 + 4,
				u8g2.getMaxCharHeight(),
				WiFi.localIP().toString().c_str() // client ip
			);
		}
		else
		{
			u8g2.drawStr(
				12 + 4,
				u8g2.getMaxCharHeight(),
				WiFi.softAPIP().toString().c_str() // access point ip
			);
		}

		// Draw current measured weight
		u8g2.setFont(u8g2_font_t0_40b_tr); // 23px
		u8g2.drawStr(
			u8g2.getDisplayWidth() - u8g2.getStrWidth(text), // x
			DISPLAY_HEADER_HEIGHT + 1 + u8g2.getMaxCharHeight(), // y
			text
		);
	} while (u8g2.nextPage());
}

void loop()
{
	static unsigned int counter = 0;

	// Read from HX711 ic and put it in JSON
	float reading = loadcell.get_units(10);

	// Push update to state manager
	scaleService.update([&](ScaleState &state) {
		if(state.weight != reading)
		{
			state.weight = reading;
			return StateUpdateResult::CHANGED;
		}

		return StateUpdateResult::UNCHANGED;
	}, "loop");

	// Update the OLED display
	updateOLED(counter);
	counter++;

	// No need to sample at max speed
	delay(500);
}
