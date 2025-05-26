#include <Arduino.h>
#include <ArduinoLog.h>

#include "RemoteControl/XboxController.hpp"
#include "Hardware/Vehicle.hpp"

std::unique_ptr<RemoteControl> remote;

void setup()
{
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.info("Starting Remote Control Vehicle\n");

  setupVehicle();
  remote = std::make_unique<XboxController>();
}

void loop()
{
  TelemetryData tel;
  tel.vcc = readVcc();
  tel.underVoltageWarning = checkUnderVoltage();
  remote->addTelemetry(tel);
  remote->update();

  if (remote->isConnected())
  {
    controlVehicle(remote->getState());
  }
  else
  {
    Log.warning("Remote control not connected\n");
    delay(1000);
  }
}