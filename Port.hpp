#pragma once

#include <string>
#include "Typedefs.hpp"
#include "IObservable.hpp"

class Port : public IObservable
{
public:
    Port(PortId portId, std::string portName, bool isIn);
    std::string getName();
    void setName(std::string portName);
    PortId getId() const;
    bool getIsInput() const;
private:
    PortId id;
    std::string name;
    bool isInput;
};
