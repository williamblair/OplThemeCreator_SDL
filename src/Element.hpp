// One of the 'Main' (e.g. main0, main1, ...) elements of the 
// OPL theme, like image locations
#include "Setting.hpp"

#include <cstdio>
#include <vector>
#include <sstream>

#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED

class Element
{
public:
    Element(void);
    ~Element(void);
    
    /* Get the element name (like main0) */
    bool parse(const std::string line);
    
    /* Parse a setting and add to the settings vector */
    bool addSetting(Setting setting);
    
    /* Print out the element to stdout */
    void print(void);
    
private:
    std::string m_name;
    
    std::vector<Setting> m_settings;
};

#endif
