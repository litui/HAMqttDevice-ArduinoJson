#include "HAMqttDevice.h"

DynamicJsonDocument jsonPayload(2048);

HAMqttDevice::HAMqttDevice(
    const String &name,
    const DeviceType type,
    const String &haMQTTPrefix) : _name(name),
                                  _type(type)
{
    // Id = name to lower case replacing spaces by underscore (ex: name="Kitchen Light" -> id="kitchen_light")
    _identifier = name;
    _identifier.replace(' ', '_');
    _identifier.toLowerCase();

    // Define the MQTT topic of the device
    _topic = haMQTTPrefix + '/' + deviceTypeToStr(_type) + '/' + _identifier;

    // Preconfigure mandatory config vars that we already know
    addConfigVar("~", _topic);
    addConfigVar("name", _name);
    addConfigVar("unique_id", _identifier);

    // When the command topic is mandatory, enable it.
    switch (type)
    {
    case DeviceType::ALARM_CONTROL_PANEL:
    case DeviceType::FAN:
    case DeviceType::LIGHT:
    case DeviceType::LOCK:
    case DeviceType::NUMBER:
    case DeviceType::SELECT:
    case DeviceType::SWITCH:
    case DeviceType::BUTTON:
        enableCommandTopic();
    default:
        break;
    }

    // When the state topic is mandatory, enable it.
    switch (type)
    {
    case DeviceType::ALARM_CONTROL_PANEL:
    case DeviceType::BINARY_SENSOR:
    case DeviceType::FAN:
    case DeviceType::LIGHT:
    case DeviceType::LOCK:
    case DeviceType::NUMBER:
    case DeviceType::SENSOR:
    case DeviceType::SWITCH:
        enableStateTopic();
    default:
        break;
    }
}

HAMqttDevice::~HAMqttDevice() {}

HAMqttDevice &HAMqttDevice::enableCommandTopic()
{
    addConfigVar("cmd_t", "~/cmd");
    return *this;
}

HAMqttDevice &HAMqttDevice::enableStateTopic()
{
    addConfigVar("stat_t", "~/state");
    return *this;
}

HAMqttDevice &HAMqttDevice::enableAttributesTopic()
{
    addConfigVar("json_attr_t", "~/attr");
    return *this;
}

HAMqttDevice &HAMqttDevice::addConfigVar(const String &name, const String &value)
{
    if (value[0] == '{') {
        DynamicJsonDocument dest(256);
        deserializeJson(dest, value);
        _configVars[name] = dest;
    } else if (value[0] == '[') {
        DynamicJsonDocument dest(256);
        deserializeJson(dest, value);
        _configVars[name] = dest;
    } else {
        _configVars[name] = value;
    }
    return *this;
}

HAMqttDevice &HAMqttDevice::addConfigVar(const String &name, JsonDocument &value)
{
    _configVars[name] = value;
    return *this;
}

HAMqttDevice &HAMqttDevice::addConfigVarRaw(const String &name, const String &value)
{
    _configVars[name] = value;
    return *this;
}

HAMqttDevice &HAMqttDevice::addAttribute(const String &name, const String &value)
{
    _attributes[name] = value;
    return *this;
}

HAMqttDevice &HAMqttDevice::clearAttributes()
{
    _attributes.clear();
    return *this;
}

const String HAMqttDevice::getConfigPayload() const
{
    char retPayload[1024] = "";
    size_t json_size = serializeJson(_configVars, retPayload);

    return retPayload;
}

const String HAMqttDevice::getAttributesPayload() const
{
    char retPayload[1024] = "";
    size_t json_size = serializeJson(_attributes, retPayload);

    return retPayload;
}

String HAMqttDevice::deviceTypeToStr(DeviceType type)
{
    switch (type)
    {
    case DeviceType::ALARM_CONTROL_PANEL:
        return "alarm_control_panel";
    case DeviceType::BINARY_SENSOR:
        return "binary_sensor";
    case DeviceType::CAMERA:
        return "camera";
    case DeviceType::COVER:
        return "cover";
    case DeviceType::FAN:
        return "fan";
    case DeviceType::LIGHT:
        return "light";
    case DeviceType::LOCK:
        return "lock";
    case DeviceType::SELECT:
        return "select";
    case DeviceType::SENSOR:
        return "sensor";
    case DeviceType::SWITCH:
        return "switch";
    case DeviceType::CLIMATE:
        return "climate";
    case DeviceType::VACUUM:
        return "vacuum";
    case DeviceType::NUMBER:
        return "number";
    case DeviceType::BUTTON:
        return "button";
    default:
        return "[Unknown DeviceType]";
    }
}
