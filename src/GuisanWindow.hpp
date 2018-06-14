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

#ifndef GUISAN_WINDOW_H_INCLUDED
#define GUISAN_WINDOW_H_INCLUDED

/* The text list of games */
extern FontHandler gamesListFontHandler;

/* The elements list we'll want to modify */
extern std::vector<Element> elements;

/* each menu icon image name */
extern std::vector<std::string> menuIconList;

/* What we want to name our text input */
enum inputEnum { MenuIcon, ItemsList, ItemCover };
static std::vector<std::string> InputIDs = {
    "MenuIcon",
    "ItemsList",
    "ItemCover"
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
        std::stringstream ss;
        std::string str;

        /* Test if the event is from our button */
        if(actionEvent.getId() == InputIDs[MenuIcon])
        {
            /* Get the widget pointer */
            gcn::TextField *t = (gcn::TextField*)actionEvent.getSource();

            /* See what our text value currently is */
            printf("Text Input: %s\n", t->getText().c_str());

            int x=0;
            sscanf(t->getText().c_str(), "%d", &x);
            printf("scanf'd x: %d\n", x);
            
            Element *e = findElement(InputIDs[MenuIcon]);
            if (e) {
                for (int j = 0; j < menuIconList.size(); j++)
                {
                    int curY = e->getYCentered(j);
                    e->setPosCentered(x, curY, j);
                }
            }
        }

        else if (actionEvent.getId() == InputIDs[ItemsList])
        {
            gcn::TextField *t = (gcn::TextField*)actionEvent.getSource();

            int x = 0;
            sscanf(t->getText().c_str(), "%d", &x);

            for (int i = 0; i < gamesListFontHandler.getNumMessages(); i++)
            {
                gamesListFontHandler.setX(x, i);
            }
        }
        
        else if (actionEvent.getId() == InputIDs[ItemCover])
        {
            printf("Item Cover Control!\n");
        }
        
    }
private:
    Element * findElement(std::string valueStr)
    {
        for (int i = 0; i < elements.size(); i++)
        {
            if (elements.at(i).getSettings().at(0).getValueStr() == valueStr)
                return &elements.at(i);
        }

        return NULL;
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

