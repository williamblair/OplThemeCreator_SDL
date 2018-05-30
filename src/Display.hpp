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
        
        /* Fills the screen with the BG color */
        void clear(void);
        
        /* Set the color to clear with by default */
        void setBGColor(Uint32 color);
        
        /* Flips the screen surface */
        void update(void);
        
        /* Returns the window surface */
        SDL_Surface *getSurface(void);
    
    private:
        SDL_Window *m_window;
        SDL_Surface *m_surface;
        
        Uint32 m_bgColor;
        
        // For SDL_Image, which doesn't have a tracker for this
        bool imageWasInit;
    
};

#endif
