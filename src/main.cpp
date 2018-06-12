#include "Display.hpp"
#include "Sprite.hpp"
#include "Setting.hpp"
#include "Element.hpp"
#include "SettingHandlers.hpp"
#include "FontHandler.hpp"
#include "GuisanWindow.hpp"

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <map>

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

/* Image names for each game location (Hard Drive, USB, Ethernet, etc */
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
            }

            /* Send remaining SDL events to guisan if the window is active */
            else if(currentWindowID == guisanWindowID)
                guisanWindow.sendEvents(&event);
        }
        

        /* Clear the screen */
        d.clear();

        /* Draw elements (if they have an image) */
        for (int i = 0; i < elements.size(); i++)
        {
            if (elements.at(i).getSettings().at(0).getValueStr() == "MenuIcon") {
                elements.at(i).draw(d.getSurface(), selectedMenuIndex);
            }
            else {
                elements.at(i).draw(d.getSurface());
            }
        }

        /* Animate the loading icon */
        loadingIcons.at(curLoadingIcon++)->draw(d.getSurface());
        if (curLoadingIcon >= loadingIcons.size()) curLoadingIcon = 0;

        /* Draw some text */
        gamesListFontHandler.draw(d.getSurface());
        hintTextFontHandler.draw(d.getSurface());

        /* Update the screen */
        d.update();

        /* Update the gui window */
        guisanWindow.update();
    }

    /* Letting the deconstructor run when it normally would causes
     * a list iterator not dereferencable error */
    guisanWindow.~GuisanWindow();
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
    /* Set some defaults first */
    gamesListFontHandler.setColor(SDL_MapRGB(d.getSurface()->format, 255, 255, 255));
    hintTextFontHandler.setColor(SDL_MapRGB(d.getSurface()->format, 255, 255, 255));
    hintTextFontHandler.setX(0); hintTextFontHandler.setY(0);

    /* Parse the global settings */
    for (Setting s : settings)
    {
        if (s.getName() == "bg_color")                     hBGColor(d, s);
        else if (s.getName() == "hint_text_color" ||
            s.getName() == "item_text_color" ||
            s.getName() == "menu_text_color" ||
            s.getName() == "sel_text_color" ||
            s.getName() == "settings_text_color" ||
            s.getName() == "text_color" ||
            s.getName() == "ui_text_color"
            )                                                 hColorSetting(d, s);
        else if (s.getName() == "default_font")            hDefaultFont(d, s);
    }

    /* Parse the elements */
    for (int i = 0; i < elements.size(); i++)
    {
        std::vector<Setting> elementSettings = elements.at(i).getSettings();

        /* If we're dealing with a main element */
        if (elements.at(i).getName().find("main") != std::string::npos)
        {
            /* Assumes the first property is type */
            if (elementSettings.at(0).getValueStr() == "Background")
            {
                /* Open the image */
                for (Setting s : elementSettings)
                {
                    /* The main background image */
                    if (s.getName() == "default") {
                        std::string image = themeDir + "/" + s.getValueStr() + ".jpg";
                        elements.at(i).addImage(image);
                    }
                }
            }

            else if (elementSettings.at(0).getValueStr() == "MenuIcon")
            {
                /* The position of the menu icons */
                int x = 0, y = 0;

                /* The menu icon - which device games are begin read from */
                for (Setting s : elementSettings)
                {
                    if (s.getName() == "x") {
                        x = s.getValueInt();
                    }
                    else if (s.getName() == "y") {
                        y = s.getValueInt();
                    }
                }

                /* Load each menu icon image */
                for (int j = 0; j < menuIconList.size(); j++)
                {
                    /* The menu icon list - which device we're reading games from */
                    std::string image = themeDir + "/" + menuIconList.at(j);
                    elements.at(i).addImage(image);

                    /* Update the image position */
                    elements.at(i).setPosCentered(x, y);
                }
                
            }

            else if (elementSettings.at(0).getValueStr() == "ItemsList")
            {
                /* The x and y pos of the Games list */
                int x = 0, y = 0;
                for (Setting s : elementSettings)
                {
                    if (s.getName() == "x") {
                        x = s.getValueInt();
                    }
                    else if (s.getName() == "y") {
                        y = s.getValueInt();
                    }
                }

                gamesListFontHandler.setX(x);
                gamesListFontHandler.setY(y);
            }

            /* Unsupported at the moment: overlay_ulx... and all of those */
            else if (elementSettings.at(0).getValueStr() == "ItemCover")
            {
                /* The x and y pos of the game cover */
                int x = 0, y = 0;

                /* The default size of the game cover */
                int coverWidth = 140, coverHeight = 200;

                /* The position of the game cover overlay, which will decide
                 * the new size of the cover image */
                int ulx = 0, uly = 0, urx = 0, ury = 0, llx = 0, lly = 0, lrx = 0, lry = 0;

                /* The new size of the cover image based on the above coordinates */
                int newCoverWidth = 1, newCoverHeight = 1;

                /* The name of the item cover image (assumes.png for now...) */
                std::string coverImage = "";

                for (Setting s : elementSettings)
                {
                    if (s.getName() == "x") {
                        x = s.getValueInt();
                    }
                    else if (s.getName() == "y") {
                        y = s.getValueInt();
                    }
                    
                    else if (s.getName() == "overlay_ulx") {
                        ulx = s.getValueInt();
                    }

                    else if (s.getName() == "overlay_uly") {
                        uly = s.getValueInt();
                    }

                    else if (s.getName() == "overlay_urx") {
                        urx = s.getValueInt();
                    }

                    else if (s.getName() == "overlay_ury") {
                        ury = s.getValueInt();
                    }

                    else if (s.getName() == "overlay_llx") {
                        llx = s.getValueInt();
                    }

                    else if (s.getName() == "overlay_lly") {
                        lly = s.getValueInt();
                    }

                    else if (s.getName() == "overlay_lrx") {
                        lrx = s.getValueInt();
                    }

                    else if (s.getName() == "overlay_lry") {
                        lry = s.getValueInt();
                    }

                    else if (s.getName() == "overlay") {
                        coverImage = s.getValueStr();
                    }
                }

                /* A default cover image */
                std::string image = "images/p4cover.png";
                elements.at(i).addImage(image);
                elements.at(i).setPosCentered(x, y);

                // TODO - fix Sprite class so resizing works!
                /* Calculate and resize the cover image */
                newCoverWidth = urx - ulx;
                newCoverHeight = lly - uly;
                elements.at(i).setSize(newCoverWidth, newCoverHeight);

                /* Load, calculate the upper left pos of the cover,
                 * and set it */
                image = themeDir + "/" + coverImage + ".png";
                elements.at(i).addImage(image);
                elements.at(i).setPos((x-(coverWidth/2))-ulx, (y-(coverHeight/2))-uly);
            }

            else if (elementSettings.at(0).getValueStr() == "LoadingIcon")
            {
                int x = 0, y = 0;
                for (Setting s : elementSettings) 
                {
                    /* Wrap values if needed */
                    if (s.getName() == "x") {
                        x = s.getValueInt();
                        if(x < 0) x = 640 + x;
                    }
                    else if (s.getName() == "y") {
                        y = s.getValueInt();
                        if(y < 0) y = 480 + y;
                    }
                }

                /* Instead of putting these images in the element, 
                 * they're stored in their own vector so we can animate them */
                for (int i = 0; i < 8; i++)
                {
                    std::stringstream image;
                    image << themeDir << "/load" << i  << ".png";
                    loadingIcons.push_back(new Sprite);
                    loadingIcons.back()->loadImage(image.str().c_str());
                    loadingIcons.back()->setX(x);
                    loadingIcons.back()->setY(y);
                }
            }
            
            else if(elementSettings.at(0).getValueStr() == "HintText")
            {
                int y = 0;
                for(Setting s : elementSettings)
                {
                    if(s.getName() == "y") {
                        y = s.getValueInt();
                    }
                }
                
                /* Load each hint text image */
                elements.at(i).addImage(themeDir+"/start.png");
                elements.at(i).setPos(0, y);
                elements.at(i).addImage(themeDir+"/cross.png");
                elements.at(i).setPos(100, y);
                elements.at(i).addImage(themeDir+"/triangle.png");
                elements.at(i).setPos(170, y);
                elements.at(i).addImage(themeDir+"/select.png");
                elements.at(i).setPos(320, y);
                elements.at(i).addImage(themeDir+"/circle.png");
                elements.at(i).setPos(440, y);
                elements.at(i).addImage(themeDir+"/square.png");
                elements.at(i).setPos(545, y);
                
                /* Load each hint text */
                hintTextFontHandler.setColor(settingColors["hint_text_color"]);
                hintTextFontHandler.addMessage(hintTextList.at(0), 40, y);
                hintTextFontHandler.addMessage(hintTextList.at(1), 125, y);
                hintTextFontHandler.addMessage(hintTextList.at(2), 190, y);
                hintTextFontHandler.addMessage(hintTextList.at(3), 360, y);
                hintTextFontHandler.addMessage(hintTextList.at(4), 460, y);
                hintTextFontHandler.addMessage(hintTextList.at(5), 570, y);
            }
        }

        /* If we're dealing with a info element */
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

