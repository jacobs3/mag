#include "IObservable.hpp"

void IObservable::update()
{
    for(auto o :observers)
    {
        if(o) o->update();
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
