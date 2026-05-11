#ifndef SCALE_STATE_H
#define SCALE_STATE_H

#include <SettingValue.h>
#include <ESP32SvelteKit.h>
#include <MqttSettingsService.h>


class ScaleState
{
public:
	/** The measured weight in Kilogram */
    float weight;

    static void read(ScaleState &state, JsonObject &root);

    static StateUpdateResult update(JsonObject &root, ScaleState &state,
		const String& originID
	);

    static void mqttRead(ScaleState &state, JsonObject &root);

    static StateUpdateResult mqttUpdate(JsonObject &root, ScaleState &state,
		const String& originID
	);

    void tare();
};

#endif // SCALE_STATE_H
