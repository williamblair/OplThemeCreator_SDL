#include "Element.hpp"

Element::Element(void){m_hasImage = false;}
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

bool Element::addImage(const std::string &imageStr)
{
    if(!m_sprite.loadImage(imageStr.c_str())) {
        return false;
    }
    m_hasImage = true;
    return true;
}

void Element::print(void)
{
    printf("Element Name: %s:\n", m_name.c_str());
    for(Setting s : m_settings)
    {
        printf("    ");
        s.print();
    }
}

std::vector<Setting> Element::getSettings(void)
{
    return m_settings;
}

std::string Element::getName(void)
{
    return m_name;
}

bool Element::draw(SDL_Surface *s)
{
    if(m_hasImage) {
        m_sprite.draw(s);
    }
    
    return true;
}

bool Element::setPos(int x, int y)
{
	m_sprite.setX(x);
	m_sprite.setY(y);

	return true;
}

bool Element::setPosCentered(int x, int y)
{
	m_sprite.setX(x - m_sprite.getW() / 2);
	m_sprite.setY(y - m_sprite.getH() / 2);

	return true;
}
