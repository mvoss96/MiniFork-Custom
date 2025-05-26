#include "RemoteControl.hpp"

const RemoteControlState &RemoteControl::getState() const
{
    return state;
}

bool RemoteControl::isConnected() const
{
    return connected;
}

void RemoteControl::addTelemetry(TelemetryData &tel)
{
    telemetry.vcc = tel.vcc;
    telemetry.underVoltageWarning = tel.underVoltageWarning;
}
