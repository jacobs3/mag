#pragma once

#include "Typedefs.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "Port.hpp"

class Router
{
 public:
    Router(RouterId);
    ~Router();
    void setName(std::string);
    std::string getName();
    RouterId getId();
    void addInputPort(PortId, std::shared_ptr<Port>);
    void addOutputPort(PortId, std::shared_ptr<Port>);
    void removeInputPort(PortId);
    void removeOutputPort(PortId);
    std::map<PortId, std::shared_ptr<Port> > getInputPorts();
    std::map<PortId, std::shared_ptr<Port> > getOutputPorts();
    std::map<PortId, std::shared_ptr<Port> > getPorts();

private:
    std::map<PortId, std::shared_ptr<Port> > inputPorts;
    std::map<PortId, std::shared_ptr<Port> > outputPorts;
    RouterId id;
    std::string name;
 //   RouterId id;
};
