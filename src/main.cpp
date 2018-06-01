#include "Display.hpp"
#include "Sprite.hpp"
#include "Setting.hpp"
#include "Element.hpp"
#include "SettingHandlers.hpp"
#include "FontHandler.hpp"

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
FontHandler fontHandler;

std::vector<Setting> settings;
std::vector<Element> elements;

/* Colors we're going to use to draw with 
 * (besides BG Color, which is kept in
 * Display) */
std::map<std::string, Uint32> settingColors;

std::string themeDir;

int main(int argc, char *argv[])
{   
    /* Open/parse config file */
    if(argc != 2) {
        printf("Usage: %s <theme_dir>\n", argv[0]);
        exit(0);
    }
    
    themeDir = argv[1];
    parseFile((themeDir+"/conf_theme.cfg").c_str());
    
    /* This must happen before applying settings
     * as setting the background color requires the 
     * SDL Surface to already by initialized*/
    if(!d.init(640, 480, "OPL Theme Creator")) {
        exit(-1);
    }
    
    applySettings();
    
    mainLoop();
    
    return 0;
}

void mainLoop(void)
{
    SDL_Event event;
    const float FPS = 30.0f;
    
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) quit = true;
        }
        
        /* Clear the screen */
        d.clear();
        
        /* Draw elements (if they have an image) */
        for(int i=0; i<elements.size(); i++)
        {
            elements.at(i).draw(d.getSurface());
        }
        
        /* Draw some text */
        fontHandler.draw(d.getSurface());
        
        /* Update the screen */
        d.update();
    }
}

void parseFile(const char *fileName)
{
    std::ifstream file(fileName);
    if(!file.is_open()) {
        printf("Failed to open file: %s\n", fileName);
        exit(-1);
    }
    
    std::string line = "";
    /* Iterate through the global settings */
    for(; line.find(":") == std::string::npos && !file.eof(); std::getline(file, line))
    {
        /* Skip if an empty or is a comment line */
        if(line.empty() || line.at(0) == '#' || line.find_first_not_of(' ') == std::string::npos) continue;
        
        //printf("Global settings line: %s\n", line.c_str());
        Setting s;
        if(s.parse(line))
            settings.push_back(s);
    }
    
    printf("\nCurrent line: %s\n\n", line.c_str());
    
    /* Iterate through elements and their settings */
    for(; !file.eof(); std::getline(file, line))
    {
        /* Skip if an empty or is a comment line */
        if(line.empty() || line.at(0) == '#' || line.find_first_not_of(' ') == std::string::npos) continue;
        
        /* If we're defining a new element */
        if(line.find(":") != std::string::npos) 
        {
            Element element;
            if(element.parse(line))
                elements.push_back(element);
        }
        else
        {
            Setting s;
            if(s.parse(line))
                elements.back().addSetting(s);
        }
    }
    
    // debug
    printf("Settings: \n--------\n");
    for(Setting s : settings) {
        s.print();
    }
    
    printf("\n\n");
    
    printf("Elements: \n--------\n");
    for(Element e : elements)
    {
        e.print();
    }
    
    file.close();
}

void applySettings(void)
{
    /* Set some defaults first */
    fontHandler.setX(0); fontHandler.setY(0);
    fontHandler.setColor(SDL_MapRGB(d.getSurface()->format, 255, 255, 255));
    
    /* Parse the global settings */
    for(Setting s : settings)
    {
        if(s.getName() == "bg_color")                     hBGColor(d, s);
        else if(s.getName() == "hint_text_color" ||
                s.getName() == "item_text_color" ||
                s.getName() == "menu_text_color" ||
                s.getName() == "sel_text_color" ||
                s.getName() == "settings_text_color" ||
                s.getName() == "text_color" ||
                s.getName() == "ui_text_color"
        )                                                 hColorSetting(d, s);
        else if(s.getName() == "default_font")            hDefaultFont(d, s);
    }
    
    /* Parse the elements */
    for(int i=0; i<elements.size(); i++)
    {
        std::vector<Setting> elementSettings = elements.at(i).getSettings();
        
        /* If we're dealing with a main element */
        if(elements.at(i).getName().find("main") != std::string::npos)
        {
            /* Assumes the first property is type */
            if(elementSettings.at(0).getValueStr() == "Background")
            {
                /* Open the image */
                for(Setting s : elementSettings)
                {
					/* The main background image */
                    if(s.getName() == "default") {
                        std::string image = themeDir+"/"+s.getValueStr()+".jpg";
                        elements.at(i).addImage(image);
                    }
                }
            }

			else if (elementSettings.at(0).getValueStr() == "MenuIcon")
			{
				/* The position of the menu icons */
				int x = 0, y = 0;

				/* The menu icon list - which device we're reading games from */
				std::string image = themeDir + "/usb.png";
				elements.at(i).addImage(image);

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

				/* Update the image position */
				elements.at(i).setPosCentered(x, y);
			}
        }
        
        /* If we're dealing with a info element */
    }
    
    // DEBUG
    printf("\n\nSetting Colors:\n");
    std::map<std::string,Uint32>::iterator it;
    for(it = settingColors.begin(); it != settingColors.end(); it++)
    {
        printf("%s: 0x%X\n", it->first.c_str(), it->second);
    }
    
}

