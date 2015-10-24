#pragma once

#include <vector>
#include "IObserver.hpp"

class IObservable
{
public:
    void attach(IObserver* obs);
    void dettach(IObserver* obs);
protected:
    void update();

private:
    std::vector<IObserver*> observers;

};
