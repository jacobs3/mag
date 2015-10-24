#include "Connection.hpp"

Connection::Connection(ConnectionId connId, PortId id1, PortId id2)
    :id(connId), p1(id1), p2(id2)

{
}

ConnectionId Connection::getId() const
{
 return id;
}

PortId Connection::getPort1() const
{
    return p1;
}

PortId Connection::getPort2() const
{
    return p2;
}
