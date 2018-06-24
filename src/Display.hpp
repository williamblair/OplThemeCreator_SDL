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

        /* Returns the SDL id of the window */
        int getWindowId(void);

        /* Returns what the user entered as the BG color */
        Uint32 getBGColor(void);
    
    private:
        SDL_Window *m_window;
        SDL_Surface *m_surface;
        
        /* m_bgColor is what SDL actually uses to display the color,
         * while m_settingBGColor is what the user entered */
        Uint32 m_settingBGColor;
        Uint32 m_bgColor;
        
        // For SDL_Image, which doesnt have a tracker for this
        bool imageWasInit;
    
};

#endif
