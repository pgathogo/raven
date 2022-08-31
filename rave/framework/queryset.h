#ifndef QUERYSET_H
#define QUERYSET_H

#include <vector>
#include <string>
#include <map>

using StringMapped = std::map<std::string, std::string>;

template<typename T1>
class QuerySet
{
public:
    QuerySet();
    ~QuerySet();
    void first();
    void next();
    void previous();
    void last();
    bool isFirst() const;
    bool isLast() const;
    bool empty() const;
    int elemSize() const;
    T1* currentElement();
    int currentIndex() const;
    void clear();

    void addElement(T1* element);
    typename std::vector<T1*>::iterator elemIter();

    virtual void all() = 0;
private:
    std::vector<T1*> elems;
    int index;
};

template<typename T1, typename T2>
class EntityModelQuerySet : public QuerySet<T1>{
    void all() override;
    template<typename... Types>
        void filter(Types... args);
};

template<typename T1>
class DataQuerySet : public QuerySet<T1>{
    public:
        DataQuerySet();
        void addRecord(T1* record);
        void all() override;
};

template<typename T1>
QuerySet<T1>::QuerySet()
{
}

template<typename T1>
QuerySet<T1>::~QuerySet()
{
    for(auto elem : elems){
        delete elem;
    }
    elems.clear();
}

template<typename T1>
void QuerySet<T1>::first()
{
    index = 0;
}

template<typename T1>
void QuerySet<T1>::next()
{
    ++index;
}

template<typename T1>
void QuerySet<T1>::previous()
{
    --index;
}

template<typename T1>
void QuerySet<T1>::last()
{
    index = elems.size();
}

template<typename T1>
bool QuerySet<T1>::isFirst() const
{
    return (index < 0) ? true : false;
}

template<typename T1>
bool QuerySet<T1>::isLast() const
{
    return (index >= elems.size()) ? true : false;
}

template<typename T1>
T1* QuerySet<T1>::currentElement()
{
    if (index > elems.size())
        index = elems.size();
    return elems[index];
}

template<typename T1>
int QuerySet<T1>::currentIndex() const
{
    return index;
}

template<typename T1>
void QuerySet<T1>::clear()
{
    for (auto elem : elems)
        delete elem;
    elems.clear();
}

template<typename T1>
int QuerySet<T1>::elemSize() const
{
    return elems.size();
}

template<typename T1>
void QuerySet<T1>::addElement(T1* element)
{
    elems.push_back(element);
}

template<typename T1>
typename std::vector<T1*>::iterator QuerySet<T1>::elemIter()
{
    return elems.begin();
}

/* ----------- EntityModelQuerySet<> ---------- */

template<typename T1, typename T2>
void EntityModelQuerySet<T1, T2>::all()
{
    T1* entity;
    T2* model = new T2(entity);
    model->all();
    do{
        entity = new T1;
        if (model->nextEntity(entity)){
            addElement(entity);
        } else {
            entity = nullptr;
        }
    }while(entity != nullptr);
}


/* ----------- DataQuerySet<> ---------- */
template<typename T1>
DataQuerySet<T1>::DataQuerySet()
    :QuerySet<T1>{}
{}

template<typename T1>
void DataQuerySet<T1>::addRecord(T1* record)
{
    this->addElement(record);
}

template<typename T1>
void DataQuerySet<T1>::all()
{
}

#endif // QUERYSET_H
