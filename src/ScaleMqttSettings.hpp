/**
 * @author Jannik Schmöle (@Jannled)
 * @date 2026-05-09
 */

#include <MqttSettingsService.h>

class ScaleMqttSettings
{
public:
    String mqttPath;
    String name;
    String uniqueId;
    String stateTopic;

    static void read(ScaleMqttSettings &settings, JsonObject &root);

    static StateUpdateResult update(JsonObject &root, ScaleMqttSettings &settings, const String& originID);
};
