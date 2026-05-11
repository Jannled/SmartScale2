/**
 * @author Jannik Schmöle (@Jannled)
 * @date 2026-05-08
 */

#include "ScaleService.hpp"
#include "Constants.hpp"


ScaleService::ScaleService(
	PsychicHttpServer* server,
	ESP32SvelteKit* svelteKit,
	ScaleMqttSettingsService* scaleMqttSettingsService,
	HX711* loadcell
) :
	httpEndpoint(
		ScaleState::read,
		ScaleState::update,
		this,
		server,
		SCALE_SETTINGS_ENDPOINT_PATH,
		svelteKit->getSecurityManager(),
		AuthenticationPredicates::NONE_REQUIRED
	),
	eventEndpoint(
		ScaleState::read,
		ScaleState::update,
		this,
		svelteKit->getSocket(),
		SCALE_SETTINGS_EVENT
	),
	mqttEndpoint(
		ScaleState::mqttRead,
		ScaleState::mqttUpdate,
		this,
		svelteKit->getMqttClient()
	),
	webSocketServer(
		ScaleState::read,
		ScaleState::update,
		this,
		server,
		SCALE_SETTINGS_SOCKET_PATH,
		svelteKit->getSecurityManager(),
		AuthenticationPredicates::IS_AUTHENTICATED
	),
	mqttClient(svelteKit->getMqttClient()),
	scaleMqttSettingsService(scaleMqttSettingsService),
	loadcell(loadcell)
{
	// configure MQTT callback
	mqttClient->onConnect(std::bind(&ScaleService::registerConfig, this));

	// configure update handler for when the scale settings change
	scaleMqttSettingsService->addUpdateHandler([&](const String &originId) {
		registerConfig();
	}, false);

	// configure settings service update handler to update scale state
	addUpdateHandler([&](const String &originId) {
		onConfigUpdated();
	}, false);
}

void ScaleService::begin()
{
	httpEndpoint.begin();
	eventEndpoint.begin();

	initHardware();

	onConfigUpdated();
}

void ScaleService::onConfigUpdated()
{
	// TODO
}

void ScaleService::registerConfig()
{
	if (!mqttClient->connected())
		return;

	String configTopic;
    String subTopic;
    String pubTopic;

    JsonDocument doc;
    scaleMqttSettingsService->read([&](ScaleMqttSettings &settings) {
		configTopic = settings.mqttPath + HAS_TOPIC_CONFIG;
		subTopic = settings.mqttPath + HAS_TOPIC_SET;
		pubTopic = settings.mqttPath + HAS_TOPIC_STATE;
		doc["~"] = settings.mqttPath;
		doc["name"] = settings.name;
		doc["unique_id"] = settings.uniqueId;
	});
	doc["cmd_t"] = "~/set";
    doc["stat_t"] = "~/state";
    doc["schema"] = "json";
    doc["brightness"] = false;

    String payload;
    serializeJson(doc, payload);
    mqttClient->publish(configTopic.c_str(), 0, false, payload.c_str());

    mqttEndpoint.configureTopics(pubTopic, subTopic);
}

void ScaleService::initHardware()
{

}
