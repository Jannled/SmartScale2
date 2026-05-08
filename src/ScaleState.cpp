#include "ScaleState.hpp"

void ScaleState::read(ScaleState &settings, JsonObject &root)
{
	root["weight"] = settings.weight;
}

StateUpdateResult ScaleState::update(JsonObject &root, ScaleState &lightState,
	const String& originID)
{

}

void ScaleState::homeAssistRead(ScaleState &settings, JsonObject &root)
{

}

StateUpdateResult ScaleState::homeAssistUpdate(JsonObject &root, ScaleState &lightState,
	const String& originID)
{

}

void ScaleState::tare()
{

}
