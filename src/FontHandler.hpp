#include "Sprite.hpp"

#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>
#include <cstdio>

#ifndef FONT_HANDLER_H_INCLUDED
#define FONT_HANDLER_H_INCLUDED

/* Used in open() */
const int FONT_SIZE = 12;

/* Used to move down to the next line to draw */
const int FONT_HEIGHT = 15;

class FontHandler
{
public:
    FontHandler(void);
    ~FontHandler(void);
    
    bool open(const std::string &fontStr);
    
    bool addMessage(const std::string &message);
    
    bool setColor(Uint32 color);
    
    bool setX(int x);
    bool setY(int y);
    
    bool draw(SDL_Surface *s);

private:
    TTF_Font *m_font;
    /* So we can set the X and Y before actually loading the text,
     * and so we can draw a list of messages from top to bottom */
    int                   m_curX;
    int                   m_curY;
    std::vector<Sprite *> m_messageSprites;
    Uint32              m_messageColor;
};

#endif
