/**
 * @author Jannik Schmöle (@Jannled)
 * @date 2026-05-08
 */

#ifndef SCALE_MQTT_SETTINGS_SERVICE_H
#define SCALE_MQTT_SETTINGS_SERVICE_H

#include <ESP32SvelteKit.h>
#include <FSPersistence.h>
#include <HttpEndpoint.h>

#include "Entity/ScaleMqttSettings.hpp"

#define SCALE_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define SCALE_BROKER_SETTINGS_PATH "/rest/brokerSettings"

class ScaleMqttSettingsService : public StatefulService<ScaleMqttSettings>
{
public:
    ScaleMqttSettingsService(PsychicHttpServer *server, ESP32SvelteKit *sveltekit);
    void begin();
    void onConfigUpdated();

private:
    HttpEndpoint<ScaleMqttSettings> httpEndpoint;
    FSPersistence<ScaleMqttSettings> fsPersistence;
    MqttSettingsService *mqttSettingsService;
};

#endif // SCALE_MQTT_SETTINGS_SERVICE_H
