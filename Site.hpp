#pragma once

#include <map>
#include <memory>

#include "Typedefs.hpp"
#include "IdGenerator.hpp"
#include "Router.hpp"
#include "Port.hpp"
#include "Connection.hpp"

class Site
{
public:
    Site();
    RouterId addRouter();
    void deleteRouter(RouterId);
    ConnectionId initializeConnection();
    ConnectionId addConnection(ConnectionId, PortId, PortId);
    void deleteConnection(ConnectionId);
    PortId addPort(RouterId, std::string, bool);
    void deletePort(RouterId, PortId);
    void setBandwith(ConnectionId);
    std::shared_ptr<Router> getRouter(RouterId);
    std::shared_ptr<Connection> getConnection(ConnectionId);
    std::shared_ptr<Port> getPort(PortId);
   private:
    IdGenerator<RouterId> routerIdGenerator;
    IdGenerator<ConnectionId> connectionIdGenerator;
    IdGenerator<PortId> portIdGenerator;
    std::map<RouterId, std::shared_ptr<Router> > routers;
    std::map<ConnectionId, std::shared_ptr<Connection> > connections;
    std::map<PortId, std::shared_ptr<Port> > ports;
    
};
