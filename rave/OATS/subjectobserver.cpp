#include <QDebug>
#include "subjectobserver.h"

namespace OATS{
    Observer::~Observer()
    {

    }

    Observer::Observer()
    {

    }

    Subject::~Subject()
    {

    }

    void Subject::attach(OATS::Observer* ob)
    {
        m_observers.push_back(ob);
    }

    void Subject::detach(OATS::Observer *)
    {

    }

    void Subject::notify()
    {
        for (auto o : m_observers){
            o->update(this);
        }
    }

    void Subject::notify(OATS::Subject* changed_subject)
    {
        for(auto o : m_observers){
            o->update(changed_subject);
        }
    }


    Subject::Subject()
    {

    }

}
