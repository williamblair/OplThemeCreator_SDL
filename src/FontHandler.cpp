#include "FontHandler.hpp"

FontHandler::FontHandler(void)
{
    m_font = NULL;
    m_messageColor = 0;
}

FontHandler::~FontHandler(void)
{
    if(m_font) TTF_CloseFont(m_font);
}

bool FontHandler::open(const std::string &fontStr)
{
    /* Close the existing font if it's open */
    this->~FontHandler();
    
    if(!(m_font = TTF_OpenFont(fontStr.c_str(), FONT_SIZE))) {
        printf("Failed to open font: %s\n", fontStr.c_str());
        return false;
    }
    
    return true;
}

bool FontHandler::setMessage(const std::string &message)
{
    /* Destroy the message sprite if it already exists */
    m_messageSprite.~Sprite();
    
    /* Extract the values from the color to 
     * create a SDL color*/
    Uint8 r,g,b;
    r = (m_messageColor & 0xFF0000) >> 16;
    g = (m_messageColor & 0x00FF00) >> 8;
    b = (m_messageColor & 0x0000FF);
    
    if(!m_messageSprite.loadMessage(
        m_font, message.c_str(), (SDL_Color){r,g,b,255})) {
        return false;
    }
    
}

bool FontHandler::draw(SDL_Surface *s)
{
    m_messageSprite.draw(s);
    return true;
}

bool FontHandler::setColor(Uint32 color)
{
    m_messageColor = color;
    return true;
}

bool FontHandler::setX(int x)
{
    m_messageSprite.setX(x);
    return true;
}

bool FontHandler::setY(int y)
{
    m_messageSprite.setY(y);
    return true;
}

