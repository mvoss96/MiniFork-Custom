#pragma once
#include <cstdint>

struct RemoteControlState
{
    int16_t throttle;
    int16_t steering;
    int16_t liftTilt;
    int16_t liftHeight;
};

class RemoteControl
{
protected:
    RemoteControlState state{};
    bool connected;

public:
    const RemoteControlState &getState() const;
    bool isConnected() const;
    virtual void update() = 0;
};