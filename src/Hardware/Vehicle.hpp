#pragma once
#include <cstdint>

#include "RemoteControl/RemoteControl.hpp"

void setupVehicle();
void controlVehicle(const RemoteControlState& state);

