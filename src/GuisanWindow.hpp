#include <guisan.hpp>
#include <guisan/sdl.hpp>
#include <SDL.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdio>

#include "Element.hpp"
#include "Setting.hpp"
#include "FontHandler.hpp"
#include "ActionEventHandlers.hpp"

#ifndef GUISAN_WINDOW_H_INCLUDED
#define GUISAN_WINDOW_H_INCLUDED

/* Function pointers map to our command handlers */
typedef void (*handlerFunc)(const gcn::ActionEvent &actionEvent);
static std::map<std::string, handlerFunc> handlerFuncs = {
    { InputIDs[MenuIcon] , hMenuIconAction },
    { InputIDs[ItemsList], hItemsListAction },
    { InputIDs[ItemCover], hItemCoverAction }
};

/* Window dimensions */
const int S_WIDTH  = 640;
const int S_HEIGHT = 480;

/* Input event */
class TextFieldActionListener : public gcn::ActionListener
{
public:

    void action(const gcn::ActionEvent &actionEvent)
    {
        /* Call the corresponding mapping func */
        handlerFuncs[actionEvent.getId()](actionEvent);
    }
};

class GuisanWindow
{
public:
    GuisanWindow(void);
    ~GuisanWindow(void);

    bool init(void);

    bool update(void);
    
    bool sendEvents(SDL_Event *event);

    int getWindowId(void);

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
    
    /* Input Widgets */
    gcn::TextField *m_MenuIconInput;
    gcn::TextField *m_ItemsListInput;
    gcn::TextField *m_ItemCoverInput;

    /* Action listener for each input */
    TextFieldActionListener *m_TFActionListener;

    /* Set basic properties of the input */
    int curInputX;
    int curInputY;
    void initTextField(gcn::TextField *tf, std::string id);
};

#endif

