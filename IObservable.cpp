#include "IObservable.hpp"
#include <iostream>

void IObservable::update()
{
    for(auto o :observers)
    {
        std::cout<<"update";
        if(o) o->update();
        std::cout<<"update2";
    }
}

void IObservable::attach(IObserver* obs)
{
    observers.push_back(obs);
}

void IObservable::dettach(IObserver* obs)
{
    auto it = find (observers.begin(), observers.end(), obs);
    if (it != observers.end())
    {
        observers.erase(it);
    }
}
