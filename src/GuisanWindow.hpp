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

#ifndef GUISAN_WINDOW_H_INCLUDED
#define GUISAN_WINDOW_H_INCLUDED

/* The elements list we'll want to modify */
extern std::vector<Element> elements;

/* each menu icon image name */
extern std::vector<std::string> menuIconList;

/* What we want to name our text input */
const std::string INPUT_ID = "textInput1";

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
        if(actionEvent.getId() == INPUT_ID)
        {
            /* Get the widget pointer */
            gcn::TextField *t = (gcn::TextField*)actionEvent.getSource();

            /* See what our text value currently is */
            printf("Text Input: %s\n", t->getText().c_str());

            int x=0;
            sscanf(t->getText().c_str(), "%d", &x);
            printf("scanf'd x: %d\n", x);

            /* Find the element we want to change */
            for(int i=0; i<elements.size(); i++)
            {
                if(elements.at(i).getSettings().at(0).getValueStr() == "MenuIcon")
                {
                    //std::vector<Setting> elementSettings = elements.at(i).getSettings();
                    for(int j=0; j<menuIconList.size(); j++)
                    {
                        int curY = elements.at(i).getYCentered(j);
                        elements.at(i).setPosCentered(x, curY, j);
                    }
                }
            }
        }
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
    
    /* Test input */
    //textField = new gcn::TextField("Text field");
    gcn::TextField *m_TextInput;
};

#endif

