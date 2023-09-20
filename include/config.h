//
// Created by Admin on 2023/9/11.
//

#ifndef UNTITLED_CONFIG_H
#define UNTITLED_CONFIG_H
#include <memory>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "log.h"
#include "util.h"
using namespace std;
namespace hhh {

    class ConfiVarBase
    {
    public:
        typedef shared_ptr<ConfiVarBase> ptr;
        ConfiVarBase(const std::string &name,const::string &description =""):m_name(name),m_description(description){}
        virtual ~ConfiVarBase(){}

        const std::string &getName() const {return m_name;}
        const std::string &getDescription()const{return m_description;}

        virtual std::string toString()=0;
        virtual bool fromString(const std::string&val) = 0;
    protected:
        std::string m_name;
        std::string  m_description;
    };
    template<class T>
    class Configvar:public ConfiVarBase
    {
    public:
        typedef std::shared_ptr<Configvar>ptr;
        Configvar(const string&name,const T&default_value,const std::string&description="")
        : ConfiVarBase(name,description),m_val(default_value){}
        std::string toString()override
        {
            try
            {
                return boost::lexical_cast<std::string>(m_val);
            }catch (std::exception &e)
            {
                HHH_LOG_ERROR(HHH_LOG_ROOT())<<"ConfigVar::toString exception"<<e.what()<<"convert"<< typeid(m_val).name()<<"tostring";
            }
            return "";
        }
        bool fromString(const std::string&val)override
        {
             try
             {
                 m_val=boost::lexical_cast<T>(val);
             }catch(exception&e)
             {
                 
             }
        }
    private:
        T m_val;
    };

} // hhh

#endif //UNTITLED_CONFIG_H