#include "XboxController.hpp"
#include <limits>

XboxController::XboxController()
{
    controller = std::make_unique<XboxSeriesXControllerESP32_asukiaaa::Core>();
    controller->begin();
}

int16_t XboxController::applyDeadzone(int16_t value) const
{
    if (std::abs(value) < this->deadzone)
    {
        return 0;
    }
    constexpr int32_t int16_min = std::numeric_limits<int16_t>::min();
    constexpr int32_t int16_max = std::numeric_limits<int16_t>::max();
    // Scale the value outside the deadzone to the full range of int16_t
    int16_t sign = (value > 0) ? 1 : -1;
    int32_t scaled = (std::abs(value) - this->deadzone) * int16_max / (int16_max - this->deadzone);
    return static_cast<int16_t>(sign * scaled);
}

void XboxController::update()
{
    controller->onLoop();
    connected = controller->isConnected();
    state = {};
    if (connected)
    {
        constexpr uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
        constexpr int16_t int16_min = std::numeric_limits<int16_t>::min();
        constexpr int16_t int16_max = std::numeric_limits<int16_t>::max();
        constexpr int32_t int16_range = int16_max - int16_min;

        const uint16_t LV = controller->xboxNotif.joyLVert; // Left joystick vertical
        const uint16_t LH = controller->xboxNotif.joyLHori; // Left joystick horizontal
        const uint16_t RV = controller->xboxNotif.joyRVert; // Right joystick vertical
        const uint16_t RH = controller->xboxNotif.joyRHori; // Right joystick horizontal
        const uint16_t RT = controller->xboxNotif.trigRT;   // Right trigger
        const uint16_t LT = controller->xboxNotif.trigLT;   // Left trigger

        // Steering
        state.steering = applyDeadzone(static_cast<int16_t>(((static_cast<int32_t>(LH) * int16_range) / joystickMax) + int16_min));
        
        // Throttle
        if (RT > 0 && LT == 0) // Right trigger is pressed more than left trigger
        {
            state.throttle = applyDeadzone(static_cast<int16_t>((static_cast<int32_t>(RT) * int16_max) / 1023));
        }
        else if (LT > 0 && RT == 0) // Both triggers are pressed
        {
            state.throttle = applyDeadzone(static_cast<int16_t>((static_cast<int32_t>(RT - LT) * int16_max) / 1023));
        }

        // Lift Angle
        if (controller->xboxNotif.btnDirLeft) // Button LEFT
        {
            lastTilt = min(static_cast<int>(int16_max), lastTilt + 30);
        }
        else if (controller->xboxNotif.btnDirRight) // Button RIGHT
        {
            lastTilt = max(static_cast<int>(int16_min), lastTilt - 30);
        }
        state.liftTilt = lastTilt;

        // Lift Height
        if (controller->xboxNotif.btnDirUp) // Button UP
        {
            state.liftHeight = int16_max;
        }
        else if (controller->xboxNotif.btnDirDown) // Button DOWN
        {
            state.liftHeight = int16_min;
        }
    }
}
