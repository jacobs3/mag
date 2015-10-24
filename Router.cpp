#include "Router.hpp"

#include <algorithm>
#include <list>

Router::Router(RouterId routerId)
{
    id=routerId;
    name="Router" + std::to_string(id);
    ilmTable = std::make_shared<std::vector<IlmEntry> >();
    ftnTable = std::make_shared<std::vector<FtnEntry> >();
    nhlfeTable = std::make_shared<std::vector<NhlfeEntry> >();
}

Router::~Router()
{
}

void Router::setName(std::string routerName)
{
    name=routerName;
    update();
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
    update();
}

void Router::addOutputPort(PortId id, std::shared_ptr<Port> port)
{
    std::cout<<"add port 1"<<std::endl;
    outputPorts[id] = port;
    std::cout<<"add port 2"<<std::endl;
    update();
    std::cout<<"add port 3"<<std::endl;
}

void Router::removeInputPort(PortId id)
{
    auto it=inputPorts.find(id);
    inputPorts.erase(it);
    std::list<int> toRemove;
    int counter = 0;
    for(auto it = ilmTable->begin(); it != ilmTable->end(); it++)
    {
        try
        {
            inputPorts.at(it->inPort);
        }
        catch(std::out_of_range)
        {
            toRemove.push_back(counter);
        }
     counter++;
    }

    auto iter = ilmTable->begin();
    for(auto i : toRemove)
    {
        std::advance(iter,i);
        ilmTable->erase (iter);
        iter = ilmTable->begin();
    }
    update();
}

void Router::removeOutputPort(PortId id)
{
     auto it=outputPorts.find(id);
     outputPorts.erase(it);
     std::list<int> toRemove;
     int counter = 0;
     for(auto it = nhlfeTable->begin(); it != nhlfeTable->end(); it++)
     {
         try
         {
             outputPorts.at(it->outPort);
         }
         catch(std::out_of_range)
         {
             toRemove.push_back(counter);
         }
         counter++;
     }
     auto iter = nhlfeTable->begin();
     for(auto i : toRemove)
     {
         std::advance(iter,i);
         nhlfeTable->erase (iter);
         iter = nhlfeTable->begin();
     }
     update();
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

std::shared_ptr<std::vector<IlmEntry> > Router::getIlmTable()
{
    return ilmTable;
}

std::shared_ptr<std::vector<FtnEntry> > Router::getFtnTable()
{
    return ftnTable;
}

std::shared_ptr<std::vector<NhlfeEntry> > Router::getNhlfeTable()
{
    return nhlfeTable;
}

std::string Router::addNhlfeEntry(NhlfeEntry entry)
{
    auto findIt = std::find(nhlfeTable->begin(),nhlfeTable->end(), entry);
    if (findIt != nhlfeTable->end())
    {
        return "Given token exists.";
    }
    else
    {
        nhlfeTable->push_back(entry);
        std::sort(nhlfeTable->begin(),nhlfeTable->end(),[](NhlfeEntry a, NhlfeEntry b)
        {
          return a.token<b.token;
        });
        update();
        return "";
    }
}

void Router::removeNhlfeEntry(NhlfeEntry entry)
{
    auto findIt = std::find(nhlfeTable->begin(),nhlfeTable->end(), entry);
    if (findIt != nhlfeTable->end())
    {
        nhlfeTable->erase(findIt);
    }
    update();
}

std::string Router::addIlmEntry(IlmEntry entry)
{
    auto findIt = std::find(ilmTable->begin(),ilmTable->end(), entry);
    if (findIt != ilmTable->end())
    {
        return "Given inLabel, inPort pair exists.";
    }
    else
    {
        ilmTable->push_back(entry);
        std::sort(ilmTable->begin(),ilmTable->end(),[](IlmEntry a, IlmEntry b)
        {
          if(a.inLabel<b.inLabel)
          {
              return true;
          }
          else if(a.inLabel>b.inLabel)
          {
              return false;
          }
          else
          {
              if(a.inPort<=b.inPort)
              {
                  return true;
              }
              else
              {
                  return false;
              }
          }
        });
        update();
        return "";
    }
}

void Router::removeIlmEntry(IlmEntry entry)
{
    auto findIt = std::find(ilmTable->begin(),ilmTable->end(), entry);
    if (findIt != ilmTable->end())
    {
        ilmTable->erase(findIt);
    }
    update();
}

std::string Router::addFtnEntry(FtnEntry entry)
{
    std::cout<<"addFtn"<<std::endl;
    auto findIt = std::find(ftnTable->begin(),ftnTable->end(), entry);
    if (findIt != ftnTable->end())
    {
        return "Given FEC exists.";
    }
    else
    {
        ftnTable->push_back(entry);
        std::sort(ftnTable->begin(),ftnTable->end(),[](FtnEntry a, FtnEntry b)
        {
          return a.fec<b.fec;
        });
        update();
        return "";
    }
}

void Router::removeFtnEntry(FtnEntry entry)
{
    auto findIt = std::find(ftnTable->begin(),ftnTable->end(), entry);
    if (findIt != ftnTable->end())
    {
        ftnTable->erase(findIt);
    }
    update();
}
