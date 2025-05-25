#include "Vehicle.hpp"

#include <ESP32Servo.h>
#include <limits>
#include <ArduinoLog.h>

constexpr int PIN_STEERING_SERVO = 4;
constexpr int PIN_LIFT_SERVO = 3;
constexpr int PIN_MOTOR_LIFT_A = 0;
constexpr int PIN_MOTOR_LIFT_B = 1;
constexpr int PIN_MOTOR_1_A = 7;
constexpr int PIN_MOTOR_1_B = 5;
constexpr int PIN_MOTOR_2_A = 6;
constexpr int PIN_MOTOR_2_B = 8;

constexpr int STEERING_SERVO_MIN_MS = 1315;
constexpr int STEERING_SERVO_MAX_MS = 1925;
constexpr int LIFT_SERVO_MIN_MS = 1000;
constexpr int LIFT_SERVO_MAX_MS = 1600;

Servo steeringServo;
Servo liftServo;

int16_t steeringTrim = 0;
int16_t liftTiltTrim = 0;

void setupVehicle()
{
    // Initialize the servos
    liftServo.setPeriodHertz(50);
    steeringServo.setPeriodHertz(50);
    liftServo.attach(PIN_LIFT_SERVO, LIFT_SERVO_MIN_MS, LIFT_SERVO_MAX_MS);
    steeringServo.attach(PIN_STEERING_SERVO, STEERING_SERVO_MIN_MS, STEERING_SERVO_MAX_MS);

    // Initialize drv8833
    ledcAttach(PIN_MOTOR_1_A, 5000, 8);
    ledcAttach(PIN_MOTOR_1_B, 5000, 8);
    ledcAttach(PIN_MOTOR_2_A, 5000, 8);
    ledcAttach(PIN_MOTOR_2_B, 5000, 8);
    pinMode(PIN_MOTOR_LIFT_A, OUTPUT);
    pinMode(PIN_MOTOR_LIFT_B, OUTPUT);
}

void setLiftMotorSpeed(int16_t speed)
{
    digitalWrite(PIN_MOTOR_LIFT_A, speed < 0 ? HIGH : LOW);
    digitalWrite(PIN_MOTOR_LIFT_B, speed > 0 ? HIGH : LOW);
}

void setThrottle(int16_t speed)
{
    uint8_t dutyCycle = map(abs(speed), 0, std::numeric_limits<int16_t>::max(), 0, 255);
    ledcWrite(PIN_MOTOR_1_A, speed > 0 ? dutyCycle : 0);
    ledcWrite(PIN_MOTOR_1_B, speed < 0 ? dutyCycle : 0);
    ledcWrite(PIN_MOTOR_2_A, speed > 0 ? dutyCycle : 0);
    ledcWrite(PIN_MOTOR_2_B, speed < 0 ? dutyCycle : 0);
}

void controlVehicle(const RemoteControlState &state)
{
    uint16_t steeringValue = map(state.steering + steeringTrim, std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::min(), STEERING_SERVO_MIN_MS, STEERING_SERVO_MAX_MS);
    uint16_t liftTiltValue = map(state.liftTilt + liftTiltTrim, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max(), LIFT_SERVO_MIN_MS, LIFT_SERVO_MAX_MS);

    steeringServo.writeMicroseconds(steeringValue);
    liftServo.writeMicroseconds(liftTiltValue);
    setLiftMotorSpeed(state.liftHeight);
    setThrottle(state.throttle);
    Log.verbose("Control Vehicle: Steering: %d (%d), LiftTilt: %d (%d) LiftMotor: %d Throttle: %d\n", state.steering, steeringValue, state.liftTilt, liftTiltValue, state.liftHeight, state.throttle);
}