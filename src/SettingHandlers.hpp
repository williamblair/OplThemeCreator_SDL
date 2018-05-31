#include "Display.hpp"
#include "Setting.hpp"
#include "HexColorParser.hpp"

#include <map>
#include <string>

/* Defined in main.cpp */
extern std::map<std::string, Uint32> settingColors;

#ifndef SETTING_HANDLERS_H_INCLUDED
#define SETTING_HANDLERS_H_INCLUDED

bool hBGColor(Display &d, Setting &s);
bool hColorSetting(Display &d, Setting &s);

#endif
