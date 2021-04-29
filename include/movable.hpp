#pragma once

#include <vector>

class Movable
{
public:
    virtual void turn(float yaw, float pitch) = 0;
    virtual void move(float right, float front) = 0;
};
