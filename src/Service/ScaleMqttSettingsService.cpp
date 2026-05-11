#include "ScaleMqttSettingsService.hpp"

ScaleMqttSettingsService::ScaleMqttSettingsService(
	PsychicHttpServer *server,
	ESP32SvelteKit *sveltekit
) :
	httpEndpoint(
		ScaleMqttSettings::read,
		ScaleMqttSettings::update,
		this,
		server,
		SCALE_BROKER_SETTINGS_PATH,
		sveltekit->getSecurityManager(),
		AuthenticationPredicates::IS_AUTHENTICATED
	),
	fsPersistence(
		ScaleMqttSettings::read,
		ScaleMqttSettings::update,
		this,
		sveltekit->getFS(),
		SCALE_BROKER_SETTINGS_FILE
	),
	mqttSettingsService(sveltekit->getMqttSettingsService())
{
	// configure settings service update handler to update LED state
    addUpdateHandler([&](const String &originId) {
		onConfigUpdated();
	}, false);
}

void ScaleMqttSettingsService::begin()
{
	httpEndpoint.begin();
	fsPersistence.readFromFS();
}

void ScaleMqttSettingsService::onConfigUpdated()
{
	// Notify the MQTT client about the updated configuration
    mqttSettingsService->setStatusTopic(_state.stateTopic);

    // Optionally, you can also log or handle the updated configuration here
    ESP_LOGI("", "MQTT Configuration updated");
}
