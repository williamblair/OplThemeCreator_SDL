// helper class to more easily draw images

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cstdio>
#include <string>

#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

class Sprite
{
public:
    Sprite(void);
    ~Sprite(void);
    
    bool loadImage(const char *);
    bool loadMessage(TTF_Font *, const char *, SDL_Color);
    
    void draw(SDL_Surface *);
    
    void setX(int x);
    void setY(int y);

    void setW(int w);
    void setH(int h);
    
    int getX(void);
    int getY(void);
    int getW(void);
    int getH(void);

    std::string getMessage(void);
    
private:
    std::string  m_message;
    SDL_Surface *m_Surface = NULL;
    SDL_Rect     m_Rect;

    bool resizeSurface(int newWidth, int newHeight);
};

#endif
