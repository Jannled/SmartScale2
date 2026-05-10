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

#include "ScaleService.hpp"
#include "ScaleMqttSettingsService.hpp"

#define SERIAL_BAUD_RATE 115200

PsychicHttpServer server;

ESP32SvelteKit esp32sveltekit(&server, 70);

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const long LOADCELL_OFFSET = 50682624;
const long LOADCELL_DIVIDER = 5895655;
HX711 loadcell = HX711();

ScaleMqttSettingsService scaleMqttSettingsService = ScaleMqttSettingsService(
	&server,
	&esp32sveltekit
);

ScaleService scaleService = ScaleService(
	&server,
	&esp32sveltekit,
	&scaleMqttSettingsService,
	&loadcell
);

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

    // start ESP32-SvelteKit
    esp32sveltekit.begin();

	// Launch the Scale Service
	scaleService.begin();
	scaleMqttSettingsService.begin();
}

void loop()
{
    // Delete Arduino loop task, as it is not needed in this example
    vTaskDelete(NULL);
}
