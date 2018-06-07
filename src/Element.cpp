#include "Element.hpp"

Element::Element(void) { m_hasImage = false; }
Element::~Element(void) 
{
    for(int i = 0; i < m_sprites.size(); i++)
    {
        delete m_sprites.at(i);
    }
}

bool Element::parse(const std::string line)
{
    std::stringstream ss(line);

    /* Make sure we've got a valid string */
    if (line.back() != ':') {
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
    m_sprites.push_back(new Sprite);
    if(!m_sprites.back()->loadImage(imageStr.c_str())) {
        delete m_sprites.back();
        m_sprites.pop_back();
        return false;
    }
    
    m_hasImage = true;
    return true;
}

void Element::print(void)
{
    printf("Element Name: %s:\n", m_name.c_str());
    for (Setting s : m_settings)
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

bool Element::draw(SDL_Surface *s, int index)
{
    if (m_hasImage) 
    {
        if (index != -1) {
            m_sprites.at(index)->draw(s);
        }
        else {
            for (int i = 0; i < m_sprites.size(); i++)
                m_sprites.at(i)->draw(s);
        }
    }

    return true;
}

bool Element::setSize(int width, int height)
{
    if (m_sprites.size() > 0)
    {
        m_sprites.back()->setW(width);
        m_sprites.back()->setH(height);
    }

    return true;
}

bool Element::setPos(int x, int y)
{
    if(m_sprites.size() > 0)
    {
        /* Wrap the positions if they are negative */
        if(x < 0) x = 640 + x;
        if(y < 0) y = 480 + y;
        
        m_sprites.back()->setX(x);
        m_sprites.back()->setY(y);
    }
    
    return true;
}

bool Element::setPosCentered(int x, int y)
{
    if(m_sprites.size() > 0)
    {    
        m_sprites.back()->setX(x - m_sprites.back()->getW() / 2);
        m_sprites.back()->setY(y - m_sprites.back()->getH() / 2);
    }
    
    return true;
}
