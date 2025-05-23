#include "Vehicle.hpp"

#include <ESP32Servo.h>
#include <limits>
#include <ArduinoLog.h>

constexpr int PIN_STEERING_SERVO = 4;
constexpr int PIN_LIFT_SERVO = 3;
constexpr int STEERING_SERVO_MIN_MS = 1115;
constexpr int STEERING_SERVO_MAX_MS = 2125;
constexpr int LIFT_SERVO_MIN_MS = 500;
constexpr int LIFT_SERVO_MAX_MS = 2400;

Servo steeringServo;
Servo liftServo;

int16_t steeringTrim = 0;
int16_t liftTiltTrim = 0;

void setupVehicle()
{
    // Initialize the servos
    ESP32PWM::allocateTimer(0);
    liftServo.setPeriodHertz(50);
    steeringServo.setPeriodHertz(50);
    liftServo.attach(PIN_LIFT_SERVO, STEERING_SERVO_MIN_MS, STEERING_SERVO_MAX_MS);
    steeringServo.attach(PIN_STEERING_SERVO, LIFT_SERVO_MIN_MS, LIFT_SERVO_MAX_MS);
}

void controlVehicle(const RemoteControlState& state)
{
    uint16_t steeringValue = map(state.steering + steeringTrim, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max(), STEERING_SERVO_MIN_MS, STEERING_SERVO_MAX_MS);
    uint16_t liftTiltValue = map(state.liftTilt + liftTiltTrim, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max(), LIFT_SERVO_MIN_MS, LIFT_SERVO_MAX_MS);
    
    Log.verbose("Steering: %d, Lift Tilt: %d", steeringValue, liftTiltValue);
    steeringServo.writeMicroseconds(steeringValue);
    liftServo.writeMicroseconds(liftTiltValue);
}