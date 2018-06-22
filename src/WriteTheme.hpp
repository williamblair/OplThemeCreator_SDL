#include "FontHandler.hpp"
#include "Setting.hpp"
#include "Element.hpp"
#include "Sprite.hpp"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

#include <filesystem>

#ifndef WRITE_THEME_H_INCLUDED
#define WRITE_THEME_H_INCLUDED

/* data from main.cpp */
extern FontHandler gamesListFontHandler;
extern FontHandler hintTextFontHandler;
extern std::vector<Setting> settings;
extern std::vector<Element> elements;
extern std::vector<Sprite *> loadingIcons;
extern std::map<std::string, Uint32> settingColors;
extern std::string themeDir;

/* All of our required stuff will come from main */
void writeTheme(void);

#endif
