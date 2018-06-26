#include "Display.hpp"
#include "Sprite.hpp"
#include "Setting.hpp"
#include "Element.hpp"
#include "SettingHandlers.hpp"
#include "FontHandler.hpp"
#include "GuisanWindow.hpp"
#include "ApplySettingsHandlers.hpp"
#include "ScreenType.hpp"

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

void mainLoop(void);
void parseFile(const char *filename);

void applySettings(void);

Display d;
Setting s;
FontHandler gamesListFontHandler;
FontHandler hintTextFontHandler;

GuisanWindow guisanWindow;

std::vector<Setting> settings;
std::vector<Element> elements;

/* Put these separate so we can animate them */
std::vector<Sprite *> loadingIcons;

/* Image names for each game location (Hard Drive, USB, Ethernet, etc) */
std::vector<std::string> menuIconList = {
    "usb.png",
    "eth.png",
    "hdd.png",
    "app.png",
};

/* The index of the current selected menu item */
int selectedMenuIndex = 0;

/* Placeholder text for the items list */
std::vector<std::string> dummyItemsList = {
    "Persona 4",
    "Kingdom Hearts",
    "Shadow of the Colussus",
    "Okami"
};

/* The index of the current selected game */
int selectedGameIndex = 0;

/* Which screen are we looking at? */
ScreenType currentScreen = MainScreen;

/* Hint messages for the HintText element */
std::vector<std::string> hintTextList = {
    "Menu",
    "Run",
    "Game Settings",
    "Refresh",
    "Rename",
    "Delete"
};

/* Colors we're going to use to draw with
 * (besides BG Color, which is kept in
 * Display) */
std::map<std::string, Uint32> settingColors;

std::string themeDir;

/* Function pointers map to our apply setting handlers */
typedef void (*applySettingFunc)(std::vector<Setting> *elementSettings, int i);
static std::map<std::string, applySettingFunc> applySettingFuncs = {
    { "Background",  hApplyBackground  },
    { "MenuIcon",    hApplyMenuIcon    },
    { "ItemsList",   hApplyItemsList   },
    { "ItemCover",   hApplyItemCover   },
    { "LoadingIcon", hApplyLoadingIcon },
    { "HintText",    hApplyHintText    }
};

int main(int argc, char *argv[])
{
    /* Open/parse config file */
    if (argc != 2) {
        printf("Usage: %s <theme_dir>\n", argv[0]);
        exit(0);
    }

    themeDir = argv[1];
    parseFile((themeDir + "/conf_theme.cfg").c_str());

    /* This must happen before applying settings
     * as setting the background color requires the
     * SDL Surface to already by initialized*/
    if (!d.init(640, 480, "OPL Theme Creator")) {
        exit(-1);
    }

    /* GuisanWindow must be init AFTER SDL is init */
    if (!guisanWindow.init()) {
        exit(-1);
    }

    applySettings();

    mainLoop();

    /* Free our loading icon sprites */
    for (int i = 0; i < loadingIcons.size(); i++)
    {
        delete loadingIcons.at(i);
    }

    return 0;
}

void mainLoop(void)
{
    SDL_Event event;
    const float FPS = 30.0f;

    int curLoadingIcon = 0;

    /* So we can tell which window the event is coming from */
    int displayWindowID = d.getWindowId();
    int guisanWindowID = guisanWindow.getWindowId();
    int currentWindowID = 0;

    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) quit = true;

            /* X out either window */
            else if (event.type == SDL_WINDOWEVENT)
            {
                /* Update the current window ID */
                currentWindowID = event.window.windowID;

                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    quit = true;
            }
            
            /* We only want to check the key events if we're working
             * in the theme window */
            else if (event.type == SDL_KEYUP && currentWindowID == displayWindowID)
            {
                if (event.key.keysym.sym == SDLK_UP)
                {
                    int prevSelectedItem = selectedGameIndex;
                    selectedGameIndex--;
                    if (selectedGameIndex < 0) selectedGameIndex = dummyItemsList.size() - 1;

                    /* Reset the previously selected items color and update the new one */
                    gamesListFontHandler.setColor(settingColors["text_color"], prevSelectedItem);
                    gamesListFontHandler.setColor(settingColors["sel_text_color"], selectedGameIndex);
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    int prevSelectedItem = selectedGameIndex;
                    selectedGameIndex++;
                    if (selectedGameIndex > dummyItemsList.size() - 1) selectedGameIndex = 0;

                    /* Reset the previously selected items color and update the new one */
                    gamesListFontHandler.setColor(settingColors["text_color"], prevSelectedItem);
                    gamesListFontHandler.setColor(settingColors["sel_text_color"], selectedGameIndex);
                }

                else if (event.key.keysym.sym == SDLK_LEFT) 
                {
                    selectedMenuIndex--;
                    if (selectedMenuIndex < 0) selectedMenuIndex = menuIconList.size() - 1;
                }

                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    selectedMenuIndex++;
                    if (selectedMenuIndex > menuIconList.size() - 1) selectedMenuIndex = 0;
                }
                
                else if (event.key.keysym.sym == SDLK_1)
                {
                    currentScreen = MainScreen;
                }
                
                else if (event.key.keysym.sym == SDLK_2)
                {
                    currentScreen = InfoScreen;
                }
            }

            /* Send remaining SDL events to guisan if the window is active */
            else if(currentWindowID == guisanWindowID)
                guisanWindow.sendEvents(&event);
        }
        

        /* Clear the screen */
        d.clear();
        
        /* Draw main screen stuff if we're on it */
        if (currentScreen == MainScreen)
        {
            /* Draw elements (if they have an image) */
            for (int i = 0; i < elements.size(); i++)
            {
                /* If the element is a main element */
                if (elements.at(i).getName().find("main") != std::string::npos)
                {
                    if (elements.at(i).getSettings()->at(0).getValueStr() == "MenuIcon") {
                        elements.at(i).draw(d.getSurface(), selectedMenuIndex);
                    }
                    else {
                        elements.at(i).draw(d.getSurface());
                    }
                }
            }
    
            /* Animate the loading icon */
            loadingIcons.at(curLoadingIcon++)->draw(d.getSurface());
            if (curLoadingIcon >= loadingIcons.size()) curLoadingIcon = 0;
    
            /* Draw some text */
            gamesListFontHandler.draw(d.getSurface());
            hintTextFontHandler.draw(d.getSurface());
        }
        
        /* If we're on the info screen */
        else
        {
            /* Draw elements (if they have an image) */
            for (int i = 0; i < elements.size(); i++)
            {
                /* If the element is a main element */
                if (elements.at(i).getName().find("info") != std::string::npos)
                {
                    elements.at(i).draw(d.getSurface());
                }
            }
        }

        /* Update the screen */
        d.update();

        /* Update the gui window */
        guisanWindow.update();
    }

    /* Letting the deconstructor run when it normally would causes
     * a list iterator not dereferencable error */
    guisanWindow.close();
}

void parseFile(const char *fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        printf("Failed to open file: %s\n", fileName);
        exit(-1);
    }

    std::string line = "";
    /* Iterate through the global settings */
    for (; line.find(":") == std::string::npos && !file.eof(); std::getline(file, line))
    {
        /* Skip if an empty or is a comment line */
        if (line.empty() || line.at(0) == '#' || line.find_first_not_of(' ') == std::string::npos) continue;

        //printf("Global settings line: %s\n", line.c_str());
        Setting s;
        if (s.parse(line))
            settings.push_back(s);
    }

    printf("\nCurrent line: %s\n\n", line.c_str());

    /* Iterate through elements and their settings */
    for (; !file.eof(); std::getline(file, line))
    {
        /* Skip if an empty or is a comment line */
        if (line.empty() || line.at(0) == '#' || line.find_first_not_of(' ') == std::string::npos) continue;

        /* If we're defining a new element */
        if (line.find(":") != std::string::npos)
        {
            Element element;
            if (element.parse(line))
                elements.push_back(element);
        }
        else
        {
            Setting s;
            if (s.parse(line))
                elements.back().addSetting(s);
        }
    }

    // debug
    printf("Settings: \n--------\n");
    for (Setting s : settings) {
        s.print();
    }

    printf("\n\n");

    printf("Elements: \n--------\n");
    for (Element e : elements)
    {
        e.print();
    }

    file.close();
}

void applySettings(void)
{
    /* All of the different color option names */
    std::vector<std::string> settingColorNames = {
        "bg_color",
        "hint_text_color",
        "item_text_color",
        "menu_text_color",
        "sel_text_color",
        "settings_text_color",
        "text_color",
        "ui_text_color"
    };

    /* Set some defaults first */
    gamesListFontHandler.setColor(SDL_MapRGB(d.getSurface()->format, 255, 255, 255));
    hintTextFontHandler.setColor(SDL_MapRGB(d.getSurface()->format, 255, 255, 255));
    hintTextFontHandler.setX(0); hintTextFontHandler.setY(0);

    /* Parse the global settings */
    for (Setting s : settings)
    {
        if (s.getName() == "bg_color")                     hBGColor(d, s);
        else if (std::find(settingColorNames.begin(),      
                 settingColorNames.end(), s.getName())     
                != settingColorNames.end())                hColorSetting(d, s);
        else if (s.getName() == "default_font")            hDefaultFont(d, s);
    }

    /* Parse the elements */
    for (int i = 0; i < elements.size(); i++)
    {
        std::vector<Setting> *elementSettings = elements.at(i).getSettings();

        /* If we're dealing with a main element */
        if (elements.at(i).getName().find("main") != std::string::npos)
        {
            /* Call the corresponding setting handler */
            applySettingFuncs[elementSettings->at(0).getValueStr()](elementSettings, i);
        }

        /* If we're dealing with a info element */
        // TODO - merge with above 
        else if (elements.at(i).getName().find("info") != std::string::npos)
        {
            /* Call the corresponding setting handler*/
            if (elementSettings->at(0).getValueStr() == "Background")
            {
                /*for (int j = 0; j < elementSettings->size(); j++)
                {
                    if (elementSettings->at(j).getName() == "default") {
                        std::string image = themeDir + "/" + elementSettings->at(j).getValueStr() + ".jpg";
                        elements.at(i).addImage(image);
                    }
                }*/
                hApplyBackground(elementSettings, i);
            }
        }
    }

    // DEBUG
    printf("\n\nSetting Colors:\n");
    std::map<std::string, Uint32>::iterator it;
    for (it = settingColors.begin(); it != settingColors.end(); it++)
    {
        printf("%s: 0x%X\n", it->first.c_str(), it->second);
    }

    for (std::string s : dummyItemsList)
    {
        gamesListFontHandler.setColor(settingColors["text_color"]);
        gamesListFontHandler.addMessage(s);
    }

    /* Set the current selected text color */
    gamesListFontHandler.setColor(settingColors["sel_text_color"], selectedGameIndex);
}

