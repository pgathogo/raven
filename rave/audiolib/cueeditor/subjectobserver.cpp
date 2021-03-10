#include "subjectobserver.h"

Observer::~Observer()
{

}

Observer::Observer()
{

}

Subject::~Subject()
{

}

void Subject::attach(Observer* ob)
{
    m_observers.push_back(ob);
}

void Subject::detach(Observer *)
{

}

void Subject::notify(double msg)
{
    for(auto o : m_observers){
        o->update(this, msg);
    }
}

Subject::Subject()
{

}


