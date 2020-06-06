#ifndef CHOICEFIELDGEN_H
#define CHOICEFIELDGEN_H

#include "entityfields.h"

template<typename T>
class ChoiceField : public Field{
    public:
            ChoiceField();
            ChoiceField(std::string aName, std::string aLabel);
            ~ChoiceField() override;

            std::string valueToString() const override;
            std::string dbValueFormatter() override;
            void stringToValue(std::string val) override;
            T value();
            std::string displayName() const override;

            void addChoice(std::tuple<T, std::string> choice);
            void setValue(T value);
            void setIndex(int i);
            int index() const;

            void setCurrText(std::string text);
            std::string currText() const;

        private:
            std::vector<std::tuple<T, std::string>> mChoices;
            T mValue;
            int mIndex;
            std::string mCurrText;
};

template<typename T>
ChoiceField<T>::ChoiceField()
{
}
template<typename T>
ChoiceField<T>::ChoiceField(std::string aName,
                                  std::string aLabel)
    :Field(aName, aLabel)
{
}

template<typename T>
ChoiceField<T>::~ChoiceField()
{
}

template<typename T>
std::string ChoiceField<T>::valueToString() const
{
    if constexpr(std::is_integral_v<T>){
        return std::to_string(mValue);
    }else{
        return mValue;
    }
}

template<typename T>
std::string ChoiceField<T>::dbValueFormatter()
{
    if constexpr(std::is_integral_v<T>){
        return std::to_string(mValue);
    }else{
        return mValue;
    }
}

template<typename T>
void ChoiceField<T>::stringToValue(std::string /*val*/)
{
}

template<typename T>
T ChoiceField<T>::value()
{
    return mValue;
}

template<typename T>
std::string ChoiceField<T>::displayName() const
{
    std::string name = "NOT-FOUND";

    for (auto& c : mChoices){
        if (mValue == std::get<0>(c)){
            name = std::get<1>(c);
        }
    }

    return name;
}

template<typename T>
void ChoiceField<T>::addChoice(std::tuple<T, std::string> choice)
{
    mChoices.push_back(choice);
}

template<typename T>
void ChoiceField<T>::setValue(T value)
{
    mValue = value;
}

template<typename T>
void ChoiceField<T>::setIndex(int i)
{
    mIndex = i;
}

template<typename T>
int ChoiceField<T>::index() const
{
    return mIndex;
}

template<typename T>
void ChoiceField<T>::setCurrText(std::string text)
{
    mCurrText = text;
}

template<typename T>
std::string ChoiceField<T>::currText() const
{
    return mCurrText;
}
#endif // CHOICEFIELDGEN_H
