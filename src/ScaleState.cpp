#include "ScaleState.hpp"

void ScaleState::read(ScaleState &settings, JsonObject &root)
{
	root["weight"] = settings.weight;
}

StateUpdateResult ScaleState::update(JsonObject &root, ScaleState &scaleState,
	const String& originID)
{
	// TODO
	return StateUpdateResult::ERROR;
}

void ScaleState::mqttRead(ScaleState &settings, JsonObject &root)
{
	// TODO
}

StateUpdateResult ScaleState::mqttUpdate(JsonObject &root, ScaleState &scaleState,
	const String& originID)
{
	// TODO
	return update(root, scaleState, originID);
}

void ScaleState::tare()
{
	// TODO
}
