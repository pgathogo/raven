#ifndef SUBJECTOBSERVER_H
#define SUBJECTOBSERVER_H

#include <string>
#include <vector>

using Message = double;

namespace OATS{

    class Subject;

    class Observer{
        public:
            virtual ~Observer();
            virtual void update(OATS::Subject* changed_subject) = 0;
            virtual std::string format_message(Message message) = 0;
            virtual std::string name() = 0;
        protected:
            Observer();
    };

    class Subject{
        public:
            virtual ~Subject();
            virtual void attach(OATS::Observer*);
            virtual void detach(OATS::Observer*);
            virtual void notify();
            virtual void notify(OATS::Subject*);
        protected:
            Subject();
        private:
            std::vector<Observer*> m_observers;
    };
}

#endif // SUBJECTOBSERVER_H
