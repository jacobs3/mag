#include "NextHopCalculator.hpp"


NextHopCalculator::NextHopCalculator(Site* psite)
    :site(psite)
{
}

int NextHopCalculator::getNumberOfSameEntriesOnStack(std::vector<Label> labelStack, std::vector<Label> labelStack2) const
{
    auto it1=--labelStack.end();
    auto it2=--labelStack2.end();
    int numberOfSameEntries = 0;

    while(it1>=labelStack.begin() && it2>=labelStack2.begin())
    {
      if(*it1 == *it2)
      {
          numberOfSameEntries++;
          it1--;
          it2--;
      }
      else
      {
          return numberOfSameEntries;
      }
    }

    return numberOfSameEntries;
}

std::tuple<std::vector<Label>, PortId, PortId> NextHopCalculator::getLabelStackFromNhleEntry(std::vector<Label> labelStack,NhlfeEntry entry, int numberOfLabelsToDismiss) const
{
    if(entry.operation == LabelOperation::push)
    {
        auto tmpStack = entry.outLabel;
        std::reverse(tmpStack.begin(),tmpStack.end());
        for(auto t : tmpStack)
        {
            labelStack.push_back(t);
        }
        return std::make_tuple(labelStack, entry.outPort, site->getConnectedPort(entry.outPort));
    }
    else if(entry.operation == LabelOperation::swap)
    {
        while(numberOfLabelsToDismiss--)
        {
            labelStack.pop_back();
        }

        auto tmpStack = entry.outLabel;
        std::reverse(tmpStack.begin(),tmpStack.end());
        for(auto t : tmpStack)
        {
            labelStack.push_back(t);
        }
        return std::make_tuple(labelStack, entry.outPort, site->getConnectedPort(entry.outPort));
    }
    else //pop
    {
        while(numberOfLabelsToDismiss--)
        {
            labelStack.pop_back();
        }
        return std::make_tuple(labelStack, entry.outPort, site->getConnectedPort(entry.outPort));
    }
}

std::tuple<std::vector<Label>, PortId, PortId> NextHopCalculator::calculateNextHop(std::vector<Label> labelStack, PortId inPort,
                                                  std::shared_ptr<std::vector<IlmEntry> > ilmTable, std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable) const
{
    for(auto i : *ilmTable)
    {
        if(i.inPort == inPort)
        {
            int numberOfMatchingLabels = getNumberOfSameEntriesOnStack(labelStack, i.inLabels);
            if(numberOfMatchingLabels > 0)
            {
                auto nhlfeIt = std::find(nhlfeTable->begin(), nhlfeTable->end(), i.token);
                if(nhlfeIt == nhlfeTable->end())
                {
                    throw CannotFindNextHop("Given token doesn't exist");
                }
                else
                {
                    return getLabelStackFromNhleEntry(labelStack, *nhlfeIt, numberOfMatchingLabels);
                }
            }
        }
    }
    throw CannotFindNextHop("Cannot find given entry in ILM table");
}

std::tuple<std::vector<Label>, PortId, PortId> NextHopCalculator::calculateNextHop(FEC fec, std::shared_ptr<std::vector<FtnEntry> > ftnTable,
                                                                std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable) const
{
    std::vector<Label> outLabels;
    auto it = std::find(ftnTable->begin(), ftnTable->end(), fec);
    if(it == ftnTable->end())
    {
        throw CannotFindNextHop("Cannot find given FEC");
    }

    auto nhlfeIt = std::find(nhlfeTable->begin(), nhlfeTable->end(), it->token);
    if(nhlfeIt == nhlfeTable->end())
    {
        throw CannotFindNextHop("Given token in NHLFE table doesn't exist");
    }
    else
    {
        NhlfeEntry entry = *nhlfeIt;
        if(entry.operation == LabelOperation::push)
        {
            outLabels = entry.outLabel;
            return std::make_tuple(outLabels, entry.outPort, site->getConnectedPort(entry.outPort));
        }
        else
        {
            throw CannotFindNextHop("Push operation needed for Label Edge Router");
        }
    }
}

