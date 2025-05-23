#include "XboxController.hpp"
#include <limits>

XboxController::XboxController()
{
    controller = std::make_unique<XboxSeriesXControllerESP32_asukiaaa::Core>();
    controller->begin();
}

void XboxController::update()
{
    controller->onLoop();
    connected = controller->isConnected();
    state = {};
    if (connected)
    {
        constexpr uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
        constexpr int32_t int16_min = std::numeric_limits<int16_t>::min();
        constexpr int32_t int16_max = std::numeric_limits<int16_t>::max();
        constexpr int32_t int16_range = int16_max - int16_min;

        auto applyDeadzone = [this, int16_min, int16_max](int16_t value) -> int16_t
        {
            if (std::abs(value) < this->deadzone)
            {
                return 0;
            }
            // Werte außerhalb der Deadzone runterskalieren
            int16_t sign = (value > 0) ? 1 : -1;
            int32_t scaled = (std::abs(value) - this->deadzone) * int16_max / (int16_max - this->deadzone);
            return static_cast<int16_t>(sign * scaled);
        };

        const uint16_t LV = controller->xboxNotif.joyLVert;
        const uint16_t LH = controller->xboxNotif.joyLHori;
        const uint16_t RV = controller->xboxNotif.joyRVert;
        const uint16_t RH = controller->xboxNotif.joyRHori;

        state.steering = applyDeadzone(static_cast<int16_t>(((static_cast<int32_t>(LH) * int16_range) / joystickMax) + int16_min));
    }
}
