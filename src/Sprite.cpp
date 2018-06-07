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
    m_message = std::string(message);
    if(font == NULL){
        fprintf(stderr, "Sprite: given NULL TTF_Font!\n");
        status = false;
    }

    /* Free the rendered surface if it's been rendered before */
    if (m_Surface) {
        SDL_FreeSurface(m_Surface);
        m_Surface = NULL;
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

// TODO - actually make this worki :P
void Sprite::setW(int w){m_Rect.w = w;}
void Sprite::setH(int h){m_Rect.h = h;}

int Sprite::getX(void){return m_Rect.x;}
int Sprite::getY(void){return m_Rect.y;}
int Sprite::getW(void){return m_Rect.w;}
int Sprite::getH(void){return m_Rect.h;}

std::string Sprite::getMessage(void) { return m_message; }
