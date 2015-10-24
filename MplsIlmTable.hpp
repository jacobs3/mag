#pragma once

#include <string>
#include "Typedefs.hpp"

struct IlmEntry
{
    bool operator==(const IlmEntry& entry)
    {
        return this->inLabel == entry.inLabel && this->inPort == entry.inPort;
    }

    bool operator==(std::pair<Label, PortId> p)
    {
        return this->inLabel == p.first && this->inPort == p.second;
    }

    IlmEntry(Label pinLabel, PortId pinPort,Token ptoken)
    {
        inLabel = pinLabel;
        inPort = pinPort;
        token = ptoken;
    }

    Label inLabel;
    PortId inPort;
    Token token;
};
