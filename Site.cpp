#include "Site.hpp"

Site::Site()
{
    
}

RouterId Site::addRouter()
{
    RouterId id = routerIdGenerator.getNewId();
    routers[id] = std::make_shared<Router>();
    return id;
}

void Site::deleteRouter(RouterId)
{

}

ConnectionId Site::addConnection()
{
    //dodać wskaźniki na porty, dodać konstruktor
    ConnectionId id = connectionIdGenerator.getNewId();
    connections[id] = std::make_shared<ConnectionView>();
    return id;
}

PortId Site::addPort()
{
    PortId id = portIdGenerator.getNewId();
    return id;
}

void Site::deletePort(PortId)
{

}

void Site::setBandwith(ConnectionId id)
{

}

