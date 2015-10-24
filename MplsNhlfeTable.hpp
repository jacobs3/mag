#pragma once

#include <string>
#include <vector>
#include "Typedefs.hpp"

enum class LabelOperation  : int
{
    pop,
    push,
    swap
};

struct NhlfeEntry
{
    bool operator==(const NhlfeEntry& entry)
    {
        return this->token == entry.token;
    }

    bool operator==(Token token)
    {
        return this->token == token;
    }

    NhlfeEntry(PortId poutPort, Token ptoken, LabelOperation poperation, std::vector<Label> poutLabel)
    {
        outPort = poutPort;
        token = ptoken;
        operation = poperation;
        outLabel = poutLabel;
    }

    NhlfeEntry()
    {
        outPort = 0;
        token = 0;
    }

    PortId outPort;
    Token token;
    LabelOperation operation;
    std::vector<Label> outLabel;
};
