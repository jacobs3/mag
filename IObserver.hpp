#pragma once

#include "Typedefs.hpp"

class IObserver
{
public:
    virtual void update() = 0;
};
