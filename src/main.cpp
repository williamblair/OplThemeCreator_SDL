#include "Display.hpp"
#include "Sprite.hpp"
#include "Setting.hpp"
#include "Element.hpp"

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>

void mainLoop(void);
void parseFile(const char *filename);

Display d;
Setting s;

std::vector<Setting> settings;
std::vector<Element> elements;

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

    if(!d.init(640, 480, "OPL Theme Creator")) {
        exit(-1);
    }
    
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
        printf("    ");
        e.print();
    }
    
    file.close();
}
