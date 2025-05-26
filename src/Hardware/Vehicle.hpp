#pragma once
#include <cstdint>

#include "RemoteControl/RemoteControl.hpp"

void setupVehicle();
uint16_t readVcc();
bool checkUnderVoltage();
void controlVehicle(const RemoteControlState& state);

