/**
 * @author Jannik Schmöle (@Jannled)
 * @date 2026-05-08
 */

#ifndef WEIGHT_SERVICE_H
#define WEIGHT_SERVICE_H

#include <EventEndpoint.h>
#include <StatefulService.h>
#include <WebSocketServer.h>

#include <HX711.h>

#include "Entity/ScaleState.hpp"
#include "Service/ScaleMqttSettingsService.hpp"

#define SCALE_SETTINGS_ENDPOINT_PATH "/rest/scaleState"
#define SCALE_SETTINGS_SOCKET_PATH "/ws/scaleState"
#define SCALE_SETTINGS_EVENT "scale"

class ScaleService : public StatefulService<ScaleState>
{
public:
	/**
	 * Init the core service of the SmartScale
	 *
	 * Long running inits and things that could go wrong like hardware
	 * access should be kept out of constructors.
	 */
    ScaleService(
		PsychicHttpServer* server,
        ESP32SvelteKit* sveltekit,
        ScaleMqttSettingsService* scaleMqttSettingsService,
		HX711* loadcell
	);

    void begin();

private:
    HttpEndpoint<ScaleState> httpEndpoint;
    EventEndpoint<ScaleState> eventEndpoint;
    MqttEndpoint<ScaleState> mqttEndpoint;
    WebSocketServer<ScaleState> webSocketServer;
    PsychicMqttClient* mqttClient;
    ScaleMqttSettingsService* scaleMqttSettingsService;

	HX711* loadcell;

    void registerConfig();
    void onConfigUpdated();
	void initHardware();
};

#endif // WEIGHT_SERVICE_H
