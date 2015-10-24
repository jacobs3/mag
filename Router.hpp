#pragma once

#include "Typedefs.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include "IObservable.hpp"
#include "Port.hpp"
#include <MplsFtnTable.hpp>
#include <MplsIlmTable.hpp>
#include <MplsNhlfeTable.hpp>

class Router : public IObservable
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
    void subscribe();

    //void setIlmTable(std::shared_ptr<std::vector<IlmEntry> >);
    //void setFtnTable(std::shared_ptr<std::vector<FtnEntry> >);
    //void setNhlfeTable(std::shared_ptr<std::vector<NhlfeEntry> >);

    std::string addNhlfeEntry(NhlfeEntry entry);
    void removeNhlfeEntry(NhlfeEntry entry);
    std::string addIlmEntry(IlmEntry entry);
    void removeIlmEntry(IlmEntry entry);
    std::string addFtnEntry(FtnEntry entry);
    void removeFtnEntry(FtnEntry entry);

    std::shared_ptr<std::vector<IlmEntry> > getIlmTable();
    std::shared_ptr<std::vector<FtnEntry> > getFtnTable();
    std::shared_ptr<std::vector<NhlfeEntry> > getNhlfeTable();

private:
    std::map<PortId, std::shared_ptr<Port> > inputPorts;
    std::map<PortId, std::shared_ptr<Port> > outputPorts;
    std::shared_ptr<std::vector<IlmEntry> > ilmTable;
    std::shared_ptr<std::vector<FtnEntry> > ftnTable;
    std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable;
    RouterId id;
    std::string name;

};
