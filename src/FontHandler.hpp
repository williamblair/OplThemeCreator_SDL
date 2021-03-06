#include "Sprite.hpp"

#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>
#include <cstdio>

#ifndef FONT_HANDLER_H_INCLUDED
#define FONT_HANDLER_H_INCLUDED

/* Used in open() */
/* According to 
 * http://www.ps2-home.com/forum/viewtopic.php?t=1834
 * the font size in OPL is 17 */
const int FONT_SIZE = 17;

/* Used to move down to the next line to draw */
const int FONT_HEIGHT = 19;

class FontHandler
{
public:
    FontHandler(void);
    ~FontHandler(void);
    
    bool open(const std::string &themeDir, const std::string &fontName);
    
    /* Optional position to override the default y increment, which is used by the games list */
    bool addMessage(const std::string &message, int x = -999, int y = -999, bool centered = false);
    
    bool setColor(Uint32 color, int index = -1);
    
    bool setX(int x, int index = -1);
    bool setY(int y, int index = -1);
    
    bool draw(SDL_Surface *s);

    int getNumMessages(void);
    std::string getFontName(void);

private:
    
    TTF_Font *m_font;
    
    /* So we can set the X and Y before actually loading the text,
     * and so we can draw a list of messages from top to bottom */
    int                   m_curX;
    int                   m_curY;
    std::string           m_fontName;
    std::vector<Sprite *> m_messageSprites;
    Uint32              m_messageColor;
};

#endif
