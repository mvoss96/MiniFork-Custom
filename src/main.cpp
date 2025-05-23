#include <Arduino.h>
#include "RemoteControl/XboxController.hpp"

std::unique_ptr<RemoteControl> remote;

void setup() {
  Serial.begin(115200);
  remote = std::make_unique<XboxController>();
 
}

void loop() {
  remote->update();
  if (remote->isConnected())
  {
    const RemoteControlState &state = remote->getState();
    Serial.printf("Throttle: %d, Steering: %d, LiftTilt: %d, LiftHeight: %d\n", state.throttle, state.steering, state.liftTilt, state.liftHeight);
    delay(500);
  }
  else
  {
    Serial.println("Remote control not connected");
    delay(1000);
  }
}