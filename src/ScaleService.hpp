/**
 * @author Jannik Schmöle (@Jannled)
 * @date 2026-05-08
 */

#ifndef WEIGHT_SERVICE_H
#define WEIGHT_SERVICE_H

#include <EventEndpoint.h>
#include <StatefulService.h>
#include <WebSocketServer.h>

#include "ScaleState.hpp"
#include "ScaleMqttSettingsService.hpp"

#define SCALE_SETTINGS_ENDPOINT_PATH "/rest/scaleState"
#define SCALE_SETTINGS_SOCKET_PATH "/ws/scaleState"
#define SCALE_SETTINGS_EVENT "TODO"


class ScaleService : public StatefulService<ScaleState>
{
public:
    ScaleService(
		PsychicHttpServer *server,
        ESP32SvelteKit *sveltekit,
        ScaleMqttSettingsService *scaleMqttSettingsService
	);

    void begin();

private:
    HttpEndpoint<ScaleState> httpEndpoint;
    EventEndpoint<ScaleState> eventEndpoint;
    MqttEndpoint<ScaleState> mqttEndpoint;
    WebSocketServer<ScaleState> webSocketServer;
    PsychicMqttClient *mqttClient;
    ScaleMqttSettingsService *scaleMqttSettingsService;

    void registerConfig();
    void onConfigUpdated();
};

#endif // WEIGHT_SERVICE_H
