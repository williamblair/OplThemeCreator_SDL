#include "Element.hpp"

Element::Element(void){}
Element::~Element(void){}

bool Element::parse(const std::string line)
{
    std::stringstream ss(line);
    
    /* Make sure we've got a valid string */
    if(line.back() != ':') {
        printf("Element: invalid line on parse: %s\n", line.c_str());
        return false;
    }
    
    /* Get the name and eleminate the ':' at the end */
    std::getline(ss, m_name, ':');
    
    return true;
}

bool Element::addSetting(Setting setting)
{
    m_settings.push_back(setting);
    return true;
}

void Element::print(void)
{
    printf("Element Name: %s:\n", m_name.c_str());
    for(Setting s : m_settings)
    {
        printf("    Name: %s    Val: %s\n", s.getName().c_str(), s.getValueStr().c_str());
    }
}