#include <guisan.hpp>
#include <guisan/sdl.hpp>
#include <SDL.h>

#include <iostream>

#ifndef GUISAN_WINDOW_H_INCLUDED
#define GUISAN_WINDOW_H_INCLUDED

/* Window dimensions */
const int S_WIDTH  = 640;
const int S_HEIGHT = 480;

class GuisanWindow
{
public:
    GuisanWindow(void);
    ~GuisanWindow(void);

    bool init(void);

    bool update(void);
    
    bool sendEvents(SDL_Event *event);

private:

    /* SDL Stuff */
    SDL_Window  *m_Window;
    SDL_Surface *m_Surface;
    SDL_Event    m_Event;

    /* guisan stuff */
    gcn::SDLInput       *m_Input;
    gcn::SDLGraphics    *m_Graphics;
    gcn::SDLImageLoader *m_ImageLoader;

    /* guisan widgets */
    gcn::Gui       *m_Gui;
    gcn::Container *m_Top;
    gcn::ImageFont *m_Font;
    gcn::Label     *m_Label;
    
    /* Test input */
    //textField = new gcn::TextField("Text field");
    gcn::TextField *m_TextInput;
};

#endif

