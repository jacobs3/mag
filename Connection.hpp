#pragma once

#include "Port.hpp"
#include "Typedefs.hpp"
#include "IObservable.hpp"

class Connection : public IObservable
{
public:
    Connection(ConnectionId, PortId, PortId);
    PortId getPort1();
    PortId getPort2();
    ConnectionId getId();
private:
    ConnectionId id;
    PortId p1;
    PortId p2;

};
