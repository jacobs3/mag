#include "Site.hpp"


Site::Site()
{
    
}

RouterId Site::addRouter()
{
    RouterId id = routerIdGenerator.getNewId();
    routers[id] = std::make_shared<Router>(id);
    return id;
}

void Site::deleteRouter(RouterId id)
{
    auto it=routers.find(id);
    routers.erase(it);
}

ConnectionId Site::initializeConnection()
{
    ConnectionId id = connectionIdGenerator.getNewId();
    return id;
}

ConnectionId Site::addConnection(ConnectionId id, PortId p1, PortId p2)
{
    connections[id] = std::make_shared<Connection>(id, p1, p2);
    return id;
}

void Site::deleteConnection(ConnectionId id)
{
    auto it=connections.find(id);
    connections.erase(it);
}

PortId Site::addPort(RouterId id, std::string name, bool isInput)
{
    PortId portId = portIdGenerator.getNewId();
    ports[portId] = std::make_shared<Port>(portId, name, isInput);
    if(isInput) routers[id]->addInputPort(portId, ports[portId]);
    else routers[id]->addOutputPort(portId, ports[portId]);
    return portId;
}

void Site::deletePort(RouterId id, PortId portId)
{
    auto it=ports.find(portId);
    if(it->second->getIsInput()) routers[id]->removeInputPort(portId);
    else routers[id]->removeOutputPort(portId);
    ports.erase(it);
}

void Site::setBandwith(ConnectionId id)
{

}

std::shared_ptr<Router> Site::getRouter(RouterId id)
{
    return routers[id];
}

std::shared_ptr<Connection> Site::getConnection(ConnectionId id)
{
    return connections[id];
}

std::shared_ptr<Port> Site::getPort(PortId id)
{
    return ports[id];
}
