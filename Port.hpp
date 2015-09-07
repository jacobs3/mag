


#pragma once

#include <string>
#include "Typedefs.hpp"

class Port
{
public:
    Port(PortId, std::string, bool);
    std::string getName();
    void setName(std::string );
    PortId getId();
    bool getIsInput();
private:
    PortId id;
    std::string name;
    bool isInput;
};
