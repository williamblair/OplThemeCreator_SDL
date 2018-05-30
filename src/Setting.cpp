#include "Setting.hpp"

Setting::Setting(void){m_valueInt = -1;}
Setting::~Setting(void){}

bool Setting::parse(const std::string &str)
{
    std::istringstream ss(str);
    std::string line;
    
    /* Get the setting name */
    std::getline(ss, m_name, '=');
    
    /* If we read the entire line already
     * it must not be a proper settings line */
    if(ss.eof()) {
        printf("%s: reached end of line\n", str.c_str());
        return false;
    }
    
    /* Get the setting value */
    std::getline(ss, m_valueStr, '=');
    
    /* See if we can parse an int out of it.
     * It might not work but that's ok */
    sscanf(m_valueStr.c_str(), "%d", &m_valueInt);
    
    return true;    
}

void Setting::print(void)
{
    printf("Name: %8s    Value: %8s\n", m_name.c_str(), m_valueStr.c_str());
}

std::string Setting::getName(void){return m_name;}
std::string Setting::getValueStr(void){return m_valueStr;}
int         Setting::getValueInt(void){return m_valueInt;}