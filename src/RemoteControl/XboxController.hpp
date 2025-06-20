#pragma once
#include "RemoteControl.hpp"
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

#include <memory>

class XboxController : public RemoteControl
{
private:
    std::unique_ptr<XboxSeriesXControllerESP32_asukiaaa::Core> controller;
    const uint16_t deadzone = 4000;
    int16_t lastTiltAngleValue = 0;
    const unsigned long vibrationInterval = 10000; // 10 seconds
    unsigned long lastVibrationTime = 0;

public:
    XboxController();
    void update() override;
    int16_t applyDeadzone(int16_t value) const;
};