#pragma once
#include <cstdint>

struct RemoteControlState
{
    int16_t throttle;
    int16_t steering;
    int16_t liftTilt;
    int16_t liftHeight;
};

struct TelemetryData
{
    uint16_t vcc;
    bool underVoltageWarning;
};

class RemoteControl
{
protected:
    RemoteControlState state{};
    TelemetryData telemetry{};
    bool connected;

public:
    const RemoteControlState &getState() const;
    bool isConnected() const;
    void addTelemetry(TelemetryData &tel);
    virtual void update() = 0;
};