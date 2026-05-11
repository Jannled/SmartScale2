#include "ScaleState.hpp"

void ScaleState::read(ScaleState &state, JsonObject &root)
{
	root["weight"] = state.weight;
}

StateUpdateResult ScaleState::update(JsonObject &root, ScaleState &state,
	const String& originID)
{
	if(!root["weight"].is<float>())
		return StateUpdateResult::ERROR;

	const float newValue = root["weight"].as<float>();

	if(newValue != state.weight)
	{
		state.weight = newValue;
		return StateUpdateResult::CHANGED;
	}

	return StateUpdateResult::UNCHANGED;
}

void ScaleState::mqttRead(ScaleState &state, JsonObject &root)
{
	// TODO
	return read(state, root);
}

StateUpdateResult ScaleState::mqttUpdate(JsonObject &root, ScaleState &state,
	const String& originID)
{
	// TODO
	return update(root, state, originID);
}

void ScaleState::tare()
{
	// TODO
}
