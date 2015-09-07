#pragma once

#include "Port.hpp"
#include "Typedefs.hpp"

class Connection
{
public:
    Connection(ConnectionId, PortId, PortId);
    ConnectionId getId();
private:
    ConnectionId id;
    PortId p1;
    PortId p2;

};
