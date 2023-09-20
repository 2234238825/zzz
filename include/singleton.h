//
// Created by Admin on 2023/9/11.
//

#ifndef UNTITLED_SINGLETON_H
#define UNTITLED_SINGLETON_H
#include <memory>
using namespace std;
namespace hhh
{
    template<class T,class X = void,int N = 0>
    class Singleton
    {
    public:
        static T* GetInstance()
        {
            static T v;
            return &v;
        }
    private:

    };
    template<class T,class X=void,int N = 0>
    class SingletonPtr
    {
    public:
        static shared_ptr<T> GetInstance()
        {
            static shared_ptr<T>v (new T);
            return v;
        }
    private:

    };

} // hhh

#endif //UNTITLED_SINGLETON_H
