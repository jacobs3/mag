#include "Site.hpp"
#include <algorithm>
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
    std::cout<<"1"<<std::endl;
    PortId portId = portIdGenerator.getNewId();
    std::cout<<"2"<<std::endl;
    ports[portId] = std::make_shared<Port>(portId, name, isInput);
    std::cout<<"3"<<std::endl;
    std::cout<<"print "<<id<<" "<<name<<" "<<isInput<<std::endl;
    if(isInput) routers[id]->addInputPort(portId, ports[portId]);

    else routers[id]->addOutputPort(portId, ports[portId]);
    std::cout<<"5"<<std::endl;
    return portId;
}

void Site::deletePort(RouterId id, PortId portId)
{
    auto it=ports.find(portId);
    if(it->second->getIsInput()) routers[id]->removeInputPort(portId);
    else routers[id]->removeOutputPort(portId);
    ports.erase(it);
}

/*void Site::setBandwith(ConnectionId id)
{

}*/

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
    std::cout<<"id "<<id<<std::endl;
    std::cout<<"size "<<routers[id]->getIlmTable()->size()<<std::endl;
    std::cout<<"size "<<routers[id]->getNhlfeTable()->size()<<std::endl;

    auto result = calculator.calculateNextHop(labelStack, entryPort, routers[id]->getIlmTable(), routers[id]->getNhlfeTable());


    return result;
    /*
    std::shared_ptr<std::vector<IlmEntry> > ilmTable = routers[id]->getIlmTable();
    std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable = routers[id]->getNhlfeTable();
    std::vector<IlmEntry> ::iterator it;
    std::vector<Label> currentLabelStack = labelStack;
    std::reverse(currentLabelStack.begin(),currentLabelStack.end());
    NhlfeEntry* entry = nullptr;

    try
    {
        for(auto label : currentLabelStack)
        {
            it == std::find(ilmTable->begin(), ilmTable->end(), std::make_pair(label, nextHopPort));
            if(it == ilmTable->end())
            {
                throw CannotFindNextHop("Cannot find given FEC");
            }

            *entry = nhlfeTable->at(it->token);

            if(entry->operation == LabelOperation::push)
            {
                auto tmpStack = entry->outLabel;
                std::reverse(tmpStack.begin(),tmpStack.end());
                for(auto t : tmpStack)
                {
                    labelStack.push_back(t);
                }
                nextHopPort = getConnectedPort(entry->outPort);
                return std::make_tuple(labelStack, entry->outPort, nextHopPort);
            }
            else if(entry->operation == LabelOperation::swap)
            {
                labelStack.pop_back();
                auto tmpStack = entry->outLabel;
                std::reverse(tmpStack.begin(),tmpStack.end());
                for(auto t : tmpStack)
                {
                    labelStack.push_back(t);
                }
                nextHopPort = getConnectedPort(entry->outPort);
                return std::make_tuple(labelStack, entry->outPort, nextHopPort);
            }
            else //pop
            {
                labelStack.pop_back();
            }
        }
    }
    catch(std::out_of_range)
    {
        throw CannotFindNextHop("Given token doesn't exist");
    }
    return std::make_tuple(labelStack, entry->outPort,getConnectedPort(entry->outPort));*/
}

std::tuple<std::vector<Label>, PortId, PortId> Site::calculateNextHop(RouterId id, FEC fec)
{
    std::cout<<"calculateNextHopSite 0"<<std::endl;
    NextHopCalculator calculator(this);
    std::cout<<"calculateNextHopSite 1"<<std::endl;
    auto result = calculator.calculateNextHop(fec, routers[id]->getFtnTable(), routers[id]->getNhlfeTable());
    std::cout<<"calculateNextHopSite 2"<<std::endl;
    return result;
/*
    std::vector<Label> outLabels;
    std::shared_ptr<std::vector<FtnEntry> > ftnTable = routers[id]->getFtnTable();
    auto it = std::find(ftnTable->begin(), ftnTable->end(), fec);

    if(it == ftnTable->end())
    {
        throw CannotFindNextHop("Cannot find given FEC");
    }

    std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable = routers[id]->getNhlfeTable();
    try
    {
        NhlfeEntry entry = nhlfeTable->at(it->token);

        if(entry.operation == LabelOperation::push)
        {
            outLabels = entry.outLabel;
            nextHopPort = getConnectedPort(entry.outPort);
            labelStack = outLabels;
            return std::make_tuple(outLabels, entry.outPort, nextHopPort);
        }
        else
        {
            throw CannotFindNextHop("Push operation needed for Label Edge Router");
        }

    }
    catch(std::out_of_range)
    {
        throw CannotFindNextHop("Given token doesn't exist");
    }*/
}

PortId Site::getConnectedPort(PortId id)
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

RouterId Site::getRouterId(PortId portId)
{
    std::cout<<"getRouterId 1"<<std::endl;
    std::map<PortId, std::shared_ptr<Port> > ports;
    for(auto r : routers)
    {
        std::cout<<"getRouterId 2"<<std::endl;
        ports = r.second->getPorts();
        std::cout<<"getRouterId 3"<<std::endl;
        try
        {
            std::cout<<"getRouterId 4"<<std::endl;
            auto port =  ports.at(portId);
            std::cout<<"getRouterId 5"<<std::endl;
            return r.first;
        }
        catch(std::out_of_range)
        {
            std::cout<<"getRouterId 6"<<std::endl;
            continue;
        }

    }
    std::cout<<"getRouterId 7"<<std::endl;
    return 0;
}
