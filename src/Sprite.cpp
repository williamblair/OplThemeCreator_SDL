#include "Sprite.hpp"

// Constructor/Deconstructor
Sprite::Sprite(void)
{
    m_Rect = {0,0,0,0};
}

Sprite::~Sprite(void){
    if(m_Surface != NULL) SDL_FreeSurface(m_Surface);
}

bool Sprite::loadImage(const char *imgSource){
    bool status = true;
    m_Surface = IMG_Load(imgSource);
    if(!m_Surface){
        fprintf(stderr, "Sprite: failed to load: %s\n", imgSource);
        status = false;
    }
    else{
        m_Rect.w = m_Surface->w;
        m_Rect.h = m_Surface->h;
    }
    return status;
}

bool Sprite::loadMessage(TTF_Font *font, const char *message, SDL_Color fg){
    bool status = true;
    if(font == NULL){
        fprintf(stderr, "Sprite: given NULL TTF_Font!\n");
        status = false;
    }
    m_Surface = TTF_RenderText_Solid(font, message, fg);
    if(!m_Surface){
        fprintf(stderr, "Sprite: failed to render font: %s\n", TTF_GetError());
        status = false;
    }
    else{
        m_Rect.w = m_Surface->w;
        m_Rect.h = m_Surface->h;
    }
    return status;
}

void Sprite::draw(SDL_Surface *screenSurface)
{
    if(m_Surface)
        SDL_BlitSurface(m_Surface, NULL, screenSurface, &m_Rect);
}

// Getters / Setters
void Sprite::setX(int x){m_Rect.x = x;}
void Sprite::setY(int y){m_Rect.y = y;}

int Sprite::getX(void){return m_Rect.x;}
int Sprite::getY(void){return m_Rect.y;}
int Sprite::getW(void){return m_Rect.w;}
int Sprite::getH(void){return m_Rect.h;}
