// Generic settings object
#include <iostream>
#include <sstream>

#ifndef SETTING_H_INCLUDED
#define SETTING_H_INCLUDED

class Setting
{
public:
    Setting(void);
    ~Setting(void);
    
    /* Parse a given string to get its value(s) */
    bool parse(const std::string &str);
    
    /* Print the settings name and value to stdout */
    void print(void);
    
    /* Getters */
    std::string getName(void);
    std::string getValueStr(void);
    int         getValueInt(void);
    
private:
    std::string m_name;
    
    // each setting will have a string version and 
    // (if possible) an int version
    std::string m_valueStr;
    int         m_valueInt;
};

#endif
