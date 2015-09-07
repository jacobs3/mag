#include "Router.hpp"


Router::Router(RouterId routerId)
{
    id=routerId;
    name="test";
}

Router::~Router()
{
    std::cout<<"Router destructor";
}

void Router::setName(std::string routerName)
{
    name=routerName;
}

std::string Router::getName()
{
    return name;
}

RouterId Router::getId()
{
    return id;
}

void Router::addInputPort(PortId id, std::shared_ptr<Port> port)
{
    inputPorts[id] = port;
}

void Router::addOutputPort(PortId id, std::shared_ptr<Port> port)
{
    outputPorts[id] = port;
}

void Router::removeInputPort(PortId id)
{
    auto it=inputPorts.find(id);
    inputPorts.erase(it);
}

void Router::removeOutputPort(PortId id)
{
     auto it=outputPorts.find(id);
     outputPorts.erase(it);
}

std::map<PortId, std::shared_ptr<Port> > Router::getInputPorts()
{
    return inputPorts;
}

std::map<PortId, std::shared_ptr<Port> > Router::getOutputPorts()
{
    return outputPorts;
}

std::map<PortId, std::shared_ptr<Port> > Router::getPorts()
{
    std::map<PortId, std::shared_ptr<Port> > ports;
    ports.insert(inputPorts.begin(), inputPorts.end());
    ports.insert(outputPorts.begin(), outputPorts.end());
    return ports;
}
