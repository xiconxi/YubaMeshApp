#ifndef SINGLETON_H
#define SINGLETON_H
#include "../bases/glviewerGlobal.inc"
#include <map>

template <typename T>
class ICtrl{
public:
    static T& getGrobalInstanceRef() {
        if (s_instance == nullptr){
            s_instance = new T();
        }
//        LOG(INFO) << typeid(T).name() << ' ' << s_instance;
        return *s_instance;
    }
    static T& getPluginInstanceRef() {
        if (s_pairing_instance == nullptr){
            s_pairing_instance = new T();
        }
//        LOG(INFO) << typeid(T).name() << ' ' << s_instance;
        return *s_pairing_instance;
    }
    static T* getInstance() {
        if (s_instance == nullptr){
            s_instance = new T();
        }
        return s_instance;
    }

    static void ReleasePluginInstance(){
        T* tmp = nullptr;
        std::swap(tmp, s_pairing_instance);
        if(tmp) delete tmp;
    }
private:
    static T* s_instance;
    static T* s_pairing_instance;
};

template<typename T>
T* ICtrl<T>::s_instance = nullptr;

template<typename T>
T* ICtrl<T>::s_pairing_instance = nullptr;

namespace global {
    template <typename T>
    LIBSHARED_EXPORT T& con();
}

namespace plugin {
    template <typename T>
    LIBSHARED_EXPORT T& con();
}


#endif // SINGLETON_H
