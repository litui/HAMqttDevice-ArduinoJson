#ifndef HA_MQTT_DEVICE_H
#define HA_MQTT_DEVICE_H

#include "Arduino.h"
#include "ArduinoJson.h"

class HAMqttDevice
{
public:
    enum DeviceType
    {
        ALARM_CONTROL_PANEL,
        BINARY_SENSOR,
        CAMERA,
        COVER,
        FAN,
        LIGHT,
        LOCK,
        SELECT,
        SENSOR,
        SWITCH,
        CLIMATE,
        VACUUM,
        NUMBER,
        BUTTON
    };

private:
    // Device proprieties
    const String _name;
    const DeviceType _type;

    String _identifier;
    String _topic;

    DynamicJsonDocument _configVars = DynamicJsonDocument(2048);
    DynamicJsonDocument _attributes = DynamicJsonDocument(2048);

public:
    HAMqttDevice(
        const String &name,
        const DeviceType type,
        const String &haMQTTPrefix = "ha");

    ~HAMqttDevice();

    HAMqttDevice &enableCommandTopic();
    HAMqttDevice &enableStateTopic();
    HAMqttDevice &enableAttributesTopic();

    HAMqttDevice &addConfigVar(const String &key, const String &value);
    HAMqttDevice &addConfigVar(const String &key, JsonDocument &value);
    HAMqttDevice &addConfigVarRaw(const String &key, const String &value);
    HAMqttDevice &addAttribute(const String &key, const String &value);
    HAMqttDevice &clearAttributes();

    const String getConfigPayload() const;
    const String getAttributesPayload() const;

    inline const String getTopic() const { return _topic; }
    inline const String getStateTopic() const { return _topic + "/state"; }
    inline const String getConfigTopic() const { return _topic + "/config"; }
    inline const String getAttributesTopic() const { return _topic + "/attr"; }
    inline const String getCommandTopic() const { return _topic + "/cmd"; }

private:
    static String deviceTypeToStr(DeviceType type);
};

#endif