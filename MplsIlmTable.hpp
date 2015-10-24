#pragma once

#include <string>
#include "Typedefs.hpp"

struct IlmEntry
{
    bool operator==(const IlmEntry& entry)
    {
        return this->inLabels == entry.inLabels && this->inPort == entry.inPort;
    }



    IlmEntry(std::vector<Label> pinLabel, PortId pinPort,Token ptoken)
    {
        inLabels = pinLabel;
        inPort = pinPort;
        token = ptoken;
    }

    std::vector<Label> inLabels;
    PortId inPort;
    Token token;
};
