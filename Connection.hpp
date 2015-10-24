#pragma once

#include "Port.hpp"
#include "Typedefs.hpp"
#include "IObservable.hpp"

class Connection : public IObservable
{
public:
    Connection(ConnectionId connId, PortId id1, PortId id2);
    PortId getPort1() const;
    PortId getPort2() const;
    ConnectionId getId() const;
private:
    ConnectionId id;
    PortId p1;
    PortId p2;
};
