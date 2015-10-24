#include <algorithm>

#include "Site.hpp"
#include "NextHopCalculator.hpp"

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
    if(isInput)
        routers[id]->addInputPort(portId, ports[portId]);
    else
        routers[id]->addOutputPort(portId, ports[portId]);
    return portId;
}

void Site::deletePort(RouterId id, PortId portId)
{
    auto it=ports.find(portId);
    if(it->second->getIsInput()) routers[id]->removeInputPort(portId);
    else routers[id]->removeOutputPort(portId);
    ports.erase(it);
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

bool Site::setRouterName(RouterId id, std::string name)
{
    auto foundItem = std::find_if(routers.begin(), routers.end(), [name]( std::pair<RouterId, std::shared_ptr<Router> > router)
    { return name == router.second->getName(); });
    if(foundItem != routers.end()) return false;
    routers[id]->setName(name);
    return true;
}

std::tuple<std::vector<Label>, PortId, PortId> Site::calculateNextHop(std::vector<Label> labelStack, PortId entryPort)
{
    NextHopCalculator calculator(this);
    RouterId id = getRouterId(entryPort);
    auto result = calculator.calculateNextHop(labelStack, entryPort, routers[id]->getIlmTable(), routers[id]->getNhlfeTable());
    return result;
}

std::tuple<std::vector<Label>, PortId, PortId> Site::calculateNextHop(RouterId id, FEC fec)
{
    NextHopCalculator calculator(this);
    auto result = calculator.calculateNextHop(fec, routers[id]->getFtnTable(), routers[id]->getNhlfeTable());
    return result;
}

PortId Site::getConnectedPort(PortId id) const
{
   for(auto c : connections)
   {
       if(c.second->getPort1() == id)
       {
           return c.second->getPort2();
       }
       else if(c.second->getPort2() == id)
       {
           return c.second->getPort1();
       }
   }
   throw ConnectedPortNotFound();
}

RouterId Site::getRouterId(PortId portId) const
{
    std::map<PortId, std::shared_ptr<Port> > ports;
    for(auto r : routers)
    {
        ports = r.second->getPorts();
        try
        {
            auto port =  ports.at(portId);
            return r.first;
        }
        catch(std::out_of_range)
        {
            continue;
        }
    }
    return 0;
}
