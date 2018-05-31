#include "SettingHandlers.hpp"

bool hBGColor(Display &d, Setting &s)
{
    Uint32 bgColor = parseHexColor(s.getValueStr());
    
    d.setBGColor(bgColor);
    
    return true;
}

/* Maps the name of the setting to the parsed color */
bool hColorSetting(Display &d, Setting &s)
{
    Uint32 settingColor = parseHexColor(s.getValueStr());
    
    settingColors[s.getName()] = settingColor;
    
    return true;
}

