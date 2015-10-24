#pragma once

template <class T>
class IdGenerator
{
public:
       IdGenerator():lastId(0){}
       T getNewId()
       {
           return ++lastId;
       }

private:
       T lastId;
};
