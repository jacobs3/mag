#pragma once

#include "Site.hpp"

class NextHopCalculator
{
public:
    NextHopCalculator(Site* psite);

    std::tuple<std::vector<Label>, PortId, PortId> calculateNextHop(std::vector<Label> labelStack, PortId inPort,
                          std::shared_ptr<std::vector<IlmEntry> > ilmTable, std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable);
    std::tuple<std::vector<Label>, PortId, PortId> calculateNextHop(FEC fec, std::shared_ptr<std::vector<FtnEntry> > ftnTable,
                                                                    std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable);

private:
    Site* site;

};
