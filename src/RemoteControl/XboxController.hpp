#pragma once
#include "RemoteControl.hpp"
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

#include <memory>

class XboxController : public RemoteControl
{
    private:
        std::unique_ptr<XboxSeriesXControllerESP32_asukiaaa::Core> controller;
        const uint16_t deadzone = 4000;
        
    public:
        XboxController();     
        void update() override;

};