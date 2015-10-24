

#include "NextHopCalculator.hpp"


NextHopCalculator::NextHopCalculator(Site* psite)
    :site(psite)
{

}

int NextHopCalculator::getNumberOfSameEntriesOnStack(std::vector<Label> labelStack, std::vector<Label> labelStack2)
{
    auto it1=--labelStack.end();
    auto it2=--labelStack2.end();
    int numberOfSameEntries = 0;

    while(it1>=labelStack.begin() && it2>=labelStack2.begin())
    {
        std::cout<<"labels"<<*it1<<" "<<*it2<<std::endl;
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

std::tuple<std::vector<Label>, PortId, PortId> NextHopCalculator::getLabelStackFromNhleEntry(std::vector<Label> labelStack,NhlfeEntry entry, int numberOfLabelsToDismiss)
{
    if(entry.operation == LabelOperation::push)
    {
        std::cout<<"calculateNextHop 86"<<std::endl;
        auto tmpStack = entry.outLabel;
        std::reverse(tmpStack.begin(),tmpStack.end());
        for(auto t : tmpStack)
        {
            std::cout<<"calculateNextHop 7"<<std::endl;
            labelStack.push_back(t);
        }
        return std::make_tuple(labelStack, entry.outPort, site->getConnectedPort(entry.outPort));
    }
    else if(entry.operation == LabelOperation::swap)
    {
        std::cout<<"calculateNextHop 8"<<std::endl;
        while(numberOfLabelsToDismiss--)
        {
            labelStack.pop_back();
        }

        auto tmpStack = entry.outLabel;
        std::cout<<"calculateNextHop 9"<<std::endl;
        std::reverse(tmpStack.begin(),tmpStack.end());
        for(auto t : tmpStack)
        {
            std::cout<<"calculateNextHop 10"<<std::endl;
            labelStack.push_back(t);
        }
        std::cout<<"calculateNextHop 11"<<std::endl;
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
                                                  std::shared_ptr<std::vector<IlmEntry> > ilmTable, std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable)
{
   // std::vector<IlmEntry> ::iterator it;

    for(auto i : *ilmTable)
    {
        if(i.inPort == inPort)
        {
            std::cout<<"size"<<labelStack.size()<<" "<<i.inLabels.size()<<std::endl;
       //     auto tmpStack = i.inLabels;
         //   std::reverse(tmpStack.begin(),tmpStack.end());
            int numberOfMatchingLabels = getNumberOfSameEntriesOnStack(labelStack, i.inLabels);
            if(numberOfMatchingLabels > 0)
            {
                auto nhlfeIt = std::find(nhlfeTable->begin(), nhlfeTable->end(), i.token);
                std::cout<<"calculateNextHop 4"<<std::endl;
                if(nhlfeIt == nhlfeTable->end())
                {
                    std::cout<<"calculateNextHop 5"<<std::endl;
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
                                                                std::shared_ptr<std::vector<NhlfeEntry> > nhlfeTable)
{
    std::cout<<"calculateNextHop 0"<<std::endl;
    std::vector<Label> outLabels;
    auto it = std::find(ftnTable->begin(), ftnTable->end(), fec);
    std::cout<<"calculateNextHop 1"<<std::endl;
    if(it == ftnTable->end())
    {
        std::cout<<"calculateNextHop 2"<<std::endl;
        throw CannotFindNextHop("Cannot find given FEC");
    }

    std::cout<<"calculateNextHop 3"<<std::endl;
    auto nhlfeIt = std::find(nhlfeTable->begin(), nhlfeTable->end(), it->token);
    if(nhlfeIt == nhlfeTable->end())
    {
        std::cout<<"calculateNextHop 8"<<std::endl;
        throw CannotFindNextHop("Given token in NHLFE table doesn't exist");
    }
    else
    {
        NhlfeEntry entry = *nhlfeIt;
        std::cout<<"calculateNextHop 4"<<std::endl;
        if(entry.operation == LabelOperation::push)
        {
            std::cout<<"calculateNextHop 5"<<std::endl;
            outLabels = entry.outLabel;
            std::cout<<"calculateNextHop 6"<<std::endl;
            return std::make_tuple(outLabels, entry.outPort, site->getConnectedPort(entry.outPort));
        }
        else
        {
            std::cout<<"calculateNextHop 7"<<std::endl;
            throw CannotFindNextHop("Push operation needed for Label Edge Router");
        }
    }

}

