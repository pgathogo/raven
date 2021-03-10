#ifndef SUBJECTOBSERVER_H
#define SUBJECTOBSERVER_H

#include <string>
#include <vector>

using Message = double;

class Subject;

class Observer{
    public:
        virtual ~Observer();
        virtual void update(Subject* changed_subject, Message time) = 0;
        virtual std::string format_message(Message message) = 0;
        virtual std::string name() = 0;
    protected:
        Observer();
};

class Subject{
    public:
        virtual ~Subject();
        virtual void attach(Observer*);
        virtual void detach(Observer*);
        virtual void notify(double);
    protected:
        Subject();
    private:
        std::vector<Observer*> m_observers;
};





#endif // SUBJECTOBSERVER_H
