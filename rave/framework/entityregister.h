#ifndef ENTITYREGISTER_H
#define ENTITYREGISTER_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "../utils/tools.h"

template<typename Base, typename... Args>
class Factory{
    public:
        template<typename ... T>
        static std::unique_ptr<Base> make(const std::string &s, T&&... args){
            return data().at(s)(std::forward<T>(args)...);
        }

        template<typename T>
        struct Registrar : Base{
            friend T;

            static bool registerT() {
                const auto name = demangle(typeid(T).name());
                Factory::data()[name] = [](Args... args)->std::unique_ptr<Base> {
                    return std::make_unique<T>(std::forward<Args>(args)...);
                };

                return true;
            }
            static bool registered;

            private:
                Registrar() : Base(Key{}) { (void)registered; }
        };

          static auto _data(){ return data(); }

          friend Base;

    private:
          class Key{
              Key(){}
              template<typename T>
              friend struct Registrar;
          };

          using FuncType = std::unique_ptr<Base> (*)(Args...);
          Factory() = default;

          static auto &data(){
              static std::unordered_map<std::string, FuncType> s;
              return s;
          }
};

template<typename Base, typename... Args>
template<typename T>
bool Factory<Base, Args...>::Registrar<T>::registered =
        Factory<Base, Args...>::Registrar<T>::registerT();


#endif // ENTITYREGISTER_H
