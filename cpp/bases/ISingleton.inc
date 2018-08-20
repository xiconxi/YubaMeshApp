#ifndef SINGLETON_H
#define SINGLETON_H
#include "../bases/glviewerGlobal.inc"
#include <easylogging++.h>

template <typename T>
class ICtrl{
public:
    static T& getInstanceRef() {
        if (s_instance == nullptr){
            s_instance = new T();
        }
//        LOG(INFO) << typeid(T).name() << ' ' << s_instance;
        return *s_instance;
    }
    static T* getInstance() {
        if (s_instance == nullptr){
            s_instance = new T();
        }
//        LOG(INFO) << typeid(T).name() << ' ' << s_instance;
        return s_instance;
    }
private:
    static T* s_instance;
};

template<typename T>
T* ICtrl<T>::s_instance = nullptr;

template <typename T>
LIBSHARED_EXPORT T& con();

#endif // SINGLETON_H
