#include "Connection.hpp"


Connection::Connection(ConnectionId connId, PortId id1, PortId id2)
    :id(connId), p1(id1), p2(id2)

{
}

ConnectionId Connection::getId()
{
 return id;
}
