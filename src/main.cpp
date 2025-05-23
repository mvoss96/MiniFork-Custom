#include <Arduino.h>
#include <ArduinoLog.h>

#include "RemoteControl/XboxController.hpp"
#include "Hardware/Vehicle.hpp"

std::unique_ptr<RemoteControl> remote;

void setup() {
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  setupVehicle();
  remote = std::make_unique<XboxController>();
}

void loop() {
  remote->update();
  if (remote->isConnected())
  {
    const RemoteControlState &state = remote->getState();
    controlVehicle(state);
  }
  else
  {
    Serial.println("Remote control not connected");
    delay(1000);
  }
}