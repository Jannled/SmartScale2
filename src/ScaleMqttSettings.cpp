/**
 * @author Jannik Schmöle (@Jannled)
 * @date 2026-05-09
 */

#include "ScaleMqttSettings.hpp"

void ScaleMqttSettings::read(ScaleMqttSettings &settings, JsonObject &root)
{
	root["mqtt_path"] = settings.mqttPath;
	root["name"] = settings.name;
	root["unique_id"] = settings.uniqueId;
	root["status_topic"] = settings.stateTopic;
}

StateUpdateResult ScaleMqttSettings::update(JsonObject &root, ScaleMqttSettings &settings, const String& originID)
{
	settings.mqttPath = root["mqtt_path"] | SettingValue::format("homeassistant/light/#{unique_id}");
	settings.name = root["name"] | SettingValue::format("light-#{unique_id}");
	settings.uniqueId = root["unique_id"] | SettingValue::format("light-#{unique_id}");
	settings.stateTopic = root["status_topic"] | SettingValue::format(FACTORY_MQTT_STATUS_TOPIC);
	return StateUpdateResult::CHANGED;
}
