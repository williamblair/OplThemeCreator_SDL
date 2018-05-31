#include "Sprite.hpp"

#include <SDL2/SDL_ttf.h>

#include <string>
#include <cstdio>

#ifndef FONT_HANDLER_H_INCLUDED
#define FONT_HANDLER_H_INCLUDED

/* Used in open() */
const int FONT_SIZE = 12;

class FontHandler
{
public:
    FontHandler(void);
    ~FontHandler(void);
    
    bool open(const std::string &fontStr);
    
    bool setMessage(const std::string &message);
    
    bool setColor(Uint32 color);
    
    bool setX(int x);
    bool setY(int y);
    
    bool draw(SDL_Surface *s);

private:
    TTF_Font *m_font;
    Sprite    m_messageSprite;
    Uint32    m_messageColor;
};

#endif
