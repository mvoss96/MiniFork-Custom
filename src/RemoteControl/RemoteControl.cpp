#include "RemoteControl.hpp"

const RemoteControlState &RemoteControl::getState() const
{
    return state;
}

bool RemoteControl::isConnected() const
{
    return connected;
}