#include "Port.hpp"

Port::Port(PortId portId, std::string portName, bool isIn)
    :id(portId), name(portName), isInput(isIn)
{
}

std::string Port::getName()
{
    return name;
}

void Port::setName(std::string portName)
{
    name=portName;
    update();
}

PortId Port::getId() const
{
    return id;
}

bool Port::getIsInput() const
{
    return isInput;
}
