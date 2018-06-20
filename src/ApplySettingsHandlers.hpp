#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

#include "Setting.hpp"
#include "FontHandler.hpp"
#include "Element.hpp"

/* Defined in main.cpp */
extern std::vector<Element> elements;
extern std::vector<std::string> menuIconList;
extern std::string themeDir;
extern FontHandler gamesListFontHandler;
extern FontHandler hintTextFontHandler;
extern std::vector<Sprite *> loadingIcons;
extern std::map<std::string, Uint32> settingColors;
extern std::vector<std::string> hintTextList;

#ifndef APPLY_SETTINGS_H_INCLUDED
#define APPLY_SETTINGS_H_INCLUDED

void hApplyBackground(std::vector<Setting> *elementSettings, int i);
void hApplyMenuIcon(std::vector<Setting> *elementSettings, int i);
void hApplyItemsList(std::vector<Setting> *elementSettings, int i);
void hApplyItemCover(std::vector<Setting> *elementSettings, int i);
void hApplyLoadingIcon(std::vector<Setting> *elementSettings, int i);
void hApplyHintText(std::vector<Setting> *elementSettings, int i);

#endif
