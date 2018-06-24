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
    if (font == NULL){
        fprintf(stderr, "Sprite: given NULL TTF_Font!\n");
        status = false;
    }
    else
    {
        /* Free the rendered surface if it's been rendered before */
        if (m_Surface) {
            SDL_FreeSurface(m_Surface);
            m_Surface = NULL;
        }

        m_Surface = TTF_RenderText_Solid(font, message, fg);
    }

    if (!m_Surface) {
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
    if (m_Surface)
        SDL_BlitSurface(m_Surface, NULL, screenSurface, &m_Rect);
}

// Getters / Setters
void Sprite::setX(int x){m_Rect.x = x;}
void Sprite::setY(int y){m_Rect.y = y;}


void Sprite::setW(int w)
{
    /* Use our current/original height */
    resizeSurface(w, m_Rect.h);
}

void Sprite::setH(int h)
{
    /* Use our current/original width */
    resizeSurface(m_Rect.w, h);
}

/* Code from https://stackoverflow.com/questions/40850196/sdl2-resize-a-surface?noredirect=1&lq=1 */
bool Sprite::resizeSurface(int newWidth, int newHeight)
{
    /* 32bpp surface to blit the current surface to */
    SDL_Surface *tmp32 = SDL_CreateRGBSurface(
        m_Surface->flags,
        m_Rect.w,
        m_Rect.h,
        32,
        m_Surface->format->Rmask,
        m_Surface->format->Gmask,
        m_Surface->format->Bmask,
        m_Surface->format->Amask
    );
    if (SDL_BlitSurface(m_Surface, NULL, tmp32, NULL) < 0) {
        fprintf(stderr, "Sprite: failed to blit surface to tmp32!\n");
        return false;
    }

    /* 32bpp resized surface to blit the temp surface to */
    SDL_Surface *tmp32_resized = SDL_CreateRGBSurface(
        tmp32->flags,
        newWidth,             // the new updated size
        newHeight,
        tmp32->format->BitsPerPixel,
        tmp32->format->Rmask,
        tmp32->format->Gmask,
        tmp32->format->Bmask,
        tmp32->format->Amask
    );
    if (!tmp32_resized) {
        fprintf(stderr, "Sprite: failed to create tmp32_resized!\n");
        if (tmp32) SDL_FreeSurface(tmp32);
        return false;
    }

    /* 32bpp surface requires a filled color */
    SDL_Rect resizedRect;
    resizedRect.x = m_Rect.x;
    resizedRect.y = m_Rect.y;
    resizedRect.w = newWidth;
    resizedRect.h = newHeight;
    SDL_FillRect(tmp32_resized, &resizedRect, SDL_MapRGBA(tmp32_resized->format, 255, 0, 0, 255));

    /* Blit the original surface to the resized surface */
    if (SDL_BlitScaled(tmp32, NULL, tmp32_resized, NULL) < 0) {
        fprintf(stderr, "Sprite: failed to blit tmp32 to tmp32_resized!\n");
        if (tmp32) SDL_FreeSurface(tmp32);
        return false;
    }

    /* Set our original pointer to the resized pointer */
    m_Surface = tmp32_resized;
    if (tmp32) {
        SDL_FreeSurface(tmp32);
        tmp32 = NULL;
    }

    /* Update our size rect */
    m_Rect.w = newWidth;
    m_Rect.h = newHeight;

    return true;
}

int Sprite::getX(void){return m_Rect.x;}
int Sprite::getY(void){return m_Rect.y;}
int Sprite::getW(void){return m_Rect.w;}
int Sprite::getH(void){return m_Rect.h;}

std::string Sprite::getMessage(void) { return m_message; }
