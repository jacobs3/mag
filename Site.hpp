#pragma once

#include <map>
#include <memory>
#include <tuple>

#include "Typedefs.hpp"
#include "IdGenerator.hpp"
#include "Router.hpp"
#include "Port.hpp"
#include "Connection.hpp"


struct CannotFindNextHop: public std::runtime_error
{
    CannotFindNextHop(std::string const& message)
        : std::runtime_error(message)
    {}
};

struct ConnectedPortNotFound: public std::runtime_error
{
    ConnectedPortNotFound()
        : std::runtime_error("Connected port not found")
    {}
};

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
    //void setBandwith(ConnectionId);
    std::shared_ptr<Router> getRouter(RouterId);
    std::shared_ptr<Connection> getConnection(ConnectionId);
    std::shared_ptr<Port> getPort(PortId);
    bool setRouterName(RouterId id, std::string name);
    std::tuple<std::vector<Label>, PortId, PortId> calculateNextHop(PortId entryPort);
    std::tuple<std::vector<Label>, PortId, PortId> calculateNextHop(RouterId id, FEC fec);
    PortId getConnectedPort(PortId id);
    RouterId getRouterId(PortId portId);

   private:
    IdGenerator<RouterId> routerIdGenerator;
    IdGenerator<ConnectionId> connectionIdGenerator;
    IdGenerator<PortId> portIdGenerator;
    std::map<RouterId, std::shared_ptr<Router> > routers;
    std::map<ConnectionId, std::shared_ptr<Connection> > connections;
    std::map<PortId, std::shared_ptr<Port> > ports;
    std::vector<Label> labelStack;

    
};
