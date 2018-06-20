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

#include "EntryGroup.hpp"

#ifndef GUISAN_WINDOW_H_INCLUDED
#define GUISAN_WINDOW_H_INCLUDED

/* Function pointers map to our command handlers */
typedef void (*handlerFunc)(const gcn::ActionEvent &actionEvent);
static std::map<std::string, handlerFunc> handlerFuncs = {
    { InputIDs[MenuIcon]+"X" , hMenuIconAction },
    { InputIDs[MenuIcon]+"Y" , hMenuIconAction },
    { InputIDs[ItemsList]+"X", hItemsListAction },
    { InputIDs[ItemsList]+"Y", hItemsListAction },
    { InputIDs[ItemCover]+"X", hItemCoverAction },
    { InputIDs[ItemCover]+"Y", hItemCoverAction },
    { InputIDs[LoadingIcon]+"X", hLoadingIconAction },
    { InputIDs[LoadingIcon]+"Y", hLoadingIconAction },
    { InputIDs[HintText] + "X", hHintTextAction },
    { InputIDs[HintText] + "Y", hHintTextAction }
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
    /* Instead of a deconstructor 
     * we want to free memory ourselves
     * due to Guisan's threads causing
     * concurrency issues on close */
    void close(void);

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
    std::vector<EntryGroup> m_EntryGroups;

    /* Action listener for each input */
    TextFieldActionListener *m_TFActionListener;

    /* Set basic properties of the input */
    int curInputX;
    int curInputY;
    void initEntryGroup(std::string id);
};

#endif

