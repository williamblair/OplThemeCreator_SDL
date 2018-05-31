#include "Display.hpp"
#include "Setting.hpp"
#include "HexColorParser.hpp"
#include "FontHandler.hpp"

#include <map>
#include <string>

/* Defined in main.cpp */
extern std::map<std::string, Uint32> settingColors;
extern FontHandler fontHandler;
extern std::string themeDir;

#ifndef SETTING_HANDLERS_H_INCLUDED
#define SETTING_HANDLERS_H_INCLUDED

bool hBGColor(Display &d, Setting &s);
bool hColorSetting(Display &d, Setting &s);
bool hDefaultFont(Display &d, Setting &s);

#endif
