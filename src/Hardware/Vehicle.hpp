#pragma once
#include <cstdint>

#include "RemoteControl/RemoteControl.hpp"

void setupVehicle();
void readVcc();
void controlVehicle(const RemoteControlState& state);

