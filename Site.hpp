#pragma once

#include <map>
#include <memory>

#include "Typedefs.hpp"
#include "IdGenerator.hpp"
#include "Router.hpp"
#include "PortView.hpp"
#include "ConnectionView.hpp"

class Site
{
public:
    Site();
    RouterId addRouter();
    void deleteRouter(RouterId);
    ConnectionId addConnection();
    PortId addPort();
    void deletePort(PortId);
    void setBandwith(ConnectionId);
   private:
    IdGenerator<RouterId> routerIdGenerator;
    IdGenerator<ConnectionId> connectionIdGenerator;
    IdGenerator<PortId> portIdGenerator;
    std::map<RouterId, std::shared_ptr<Router> > routers;
    std::map<ConnectionId, std::shared_ptr<ConnectionView> > connections;
    
};
