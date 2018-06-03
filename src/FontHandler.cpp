#include "FontHandler.hpp"

FontHandler::FontHandler(void)
{
    m_font = NULL;
    m_messageColor = 0;
    m_curX = 0;
    m_curY = 0;
}

FontHandler::~FontHandler(void)
{
    if(m_font) TTF_CloseFont(m_font);

    /* Free each sprite in the list */
    for (int i = 0; i < m_messageSprites.size(); i++)
    {
        delete m_messageSprites.at(i);
    }

    /* No dangling pointers, plus reset the size to 0 */
    m_messageSprites.clear();
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

bool FontHandler::addMessage(const std::string &message)
{   
    /* Extract the values from the color to 
     * create a SDL color*/
    Uint8 r,g,b;
    r = (m_messageColor & 0xFF0000) >> 16;
    g = (m_messageColor & 0x00FF00) >> 8;
    b = (m_messageColor & 0x0000FF);
    
	SDL_Color color;
	color.r = r; color.g = g; color.b = b; color.a = 255;

    m_messageSprites.push_back(new Sprite);
    if (!m_messageSprites.back()->loadMessage(
        m_font, message.c_str(), color
    )) {
        return false;
    }

    /* Set current default x and y */
    m_messageSprites.back()->setX(m_curX);
    m_messageSprites.back()->setY(m_curY);

    /* increment the y position (move the next sprite below) */
    m_curY += FONT_HEIGHT;

    return true;
}

bool FontHandler::draw(SDL_Surface *s)
{
    for (int i = 0; i < m_messageSprites.size(); i++)
    {
        m_messageSprites.at(i)->draw(s);
    }

    return true;
}

bool FontHandler::setColor(Uint32 color)
{
    m_messageColor = color;
    return true;
}

bool FontHandler::setX(int x)
{
    m_curX = x;

    return true;
}

bool FontHandler::setY(int y)
{
    m_curY = y;
    
    return true;
}

