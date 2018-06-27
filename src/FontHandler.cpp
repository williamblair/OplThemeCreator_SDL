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
    if(m_font) {
        TTF_CloseFont(m_font);
        m_font = NULL;
    }

    /* Free each sprite in the list */
    for (int i = 0; i < m_messageSprites.size(); i++)
    {
        delete m_messageSprites.at(i);
    }

    /* No dangling pointers, plus reset the size to 0 */
    m_messageSprites.clear();
}

bool FontHandler::open(const std::string &themeDir, const std::string &fontName)
{
    /* Close the existing font if it's open */
    this->~FontHandler();

    m_fontName = fontName;
    
    if(!(m_font = TTF_OpenFont((themeDir+"/"+fontName).c_str(), FONT_SIZE))) {
        printf("Failed to open font: %s\n", fontName.c_str());
        return false;
    }
    
    return true;
}

bool FontHandler::addMessage(const std::string &message, int x, int y, bool centered)
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
    
    if(x != -999 && y != -999)
    {
        /* Wrap position if negative */
        if(x < 0) x = 640 + x;
        if(y < 0) y = 480 + y;
        
        int textWidth = m_messageSprites.back()->getW();
        int textHeight = m_messageSprites.back()->getH();
        m_messageSprites.back()->setX(x - (centered ? (textWidth / 2) : 0));
        m_messageSprites.back()->setY(y - (centered ? (textHeight /2) : 0));
    }
    else
    {
        /* Set current default x and y */
        m_messageSprites.back()->setX(m_curX);
        m_messageSprites.back()->setY(m_curY);

        /* increment the y position (move the next sprite below) */
        m_curY += FONT_HEIGHT;
    }
    

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

bool FontHandler::setColor(Uint32 color, int index)
{
    m_messageColor = color;
    if(index != -1)
    {
        /* Extract the values from the color to
        * create a SDL color*/
        Uint8 r, g, b;
        r = (color & 0xFF0000) >> 16;
        g = (color & 0x00FF00) >> 8;
        b = (color & 0x0000FF);

        SDL_Color color;
        color.r = r; color.g = g; color.b = b;

        std::string curMessage = m_messageSprites.at(index)->getMessage();
        if (!m_messageSprites.at(index)->loadMessage(
            m_font, curMessage.c_str(), color
        )) {
            return false;
        }
    }

    return true;
}

bool FontHandler::setX(int x, int index)
{
    if(index == -1)
        m_curX = x;
    else
        m_messageSprites.at(index)->setX(x);

    return true;
}

bool FontHandler::setY(int y, int index)
{
    if(index == -1)
        m_curY = y;
    else
    {
        m_messageSprites.at(index)->setY(y);
    }
    
    return true;
}

int FontHandler::getNumMessages(void)
{
    return m_messageSprites.size();
}

std::string FontHandler::getFontName(void)
{
    return m_fontName;
}
