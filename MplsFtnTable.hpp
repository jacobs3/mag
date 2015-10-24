#pragma once

#include "Typedefs.hpp"

struct FtnEntry
{
    bool operator==(const FtnEntry& entry)
    {
        return this->fec == entry.fec;
    }

    bool operator==(FEC fec)
    {
        return this->fec == fec;
    }

    FtnEntry(std::string pfec,Token ptoken)
    {
        fec = pfec;
        token = ptoken;
    }
    FEC fec;
    Token token;
};
