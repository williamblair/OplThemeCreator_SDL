#include "Display.hpp"

Display::Display(void)
{
    m_window = NULL;
    m_surface = NULL;
    
    imageWasInit = false;
}

Display::~Display(void)
{
    if(m_surface) {
        SDL_FreeSurface(m_surface);
        m_surface = NULL;
    }
    
    if(m_window) {
        SDL_DestroyWindow(m_window);
        m_window = NULL;
    }
}

bool Display::init(int width, int height, const char *title)
{
    /* Init SDL video */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Failed to init SDL: %s\n", SDL_GetError());
        return false;
    }
    
    /* init IMG */
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == -1)
    {
        fprintf(stderr, "Failed to init IMG: %s\n", IMG_GetError());
        return false;
    }
    else{
        imageWasInit = true;
    }
    
    /* init TTF */
    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Failed to init TTF: %s\n", TTF_GetError());
        return false;
    }
    
    /* create window */
    m_window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if(!m_window)
    {
        fprintf(stderr, "Failed to create SDL Window: %s\n", SDL_GetError());
        return false;
    }

    /* get game surface */
    m_surface = SDL_GetWindowSurface(m_window);
    if(!m_surface) 
    {
        fprintf(stderr, "Failed to get window surface: %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}

SDL_Surface *Display::getSurface(void)
{
    return m_surface;
}

void Display::clear(Uint32 color)
{
    SDL_FillRect(m_surface, NULL, color);
}

void Display::update(void)
{
    SDL_UpdateWindowSurface(m_window);
    
    /* hardcoded 30fps at the moment... */
    SDL_Delay(1000.0f/30.0f);
}
