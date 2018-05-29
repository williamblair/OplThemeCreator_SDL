// Window management

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cstdio>

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

class Display
{
    public:
        Display(void);
        ~Display(void);
        
        /* Initializes SDL subsystems and creates window */
        bool init(int, int, const char*);
        
        /* Fills the screen with an (optional) specified color */
        void clear(Uint32 color=0);
        
        /* Flips the screen surface */
        void update(void);
        
        /* Returns the window surface */
        SDL_Surface *getSurface(void);
    
    private:
        SDL_Window *m_window;
        SDL_Surface *m_surface;
        
        // For SDL_Image, which doesn't have a tracker for this
        bool imageWasInit;
    
};

#endif
