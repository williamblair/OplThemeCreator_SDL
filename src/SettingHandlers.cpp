#include "SettingHandlers.hpp"

bool hBGColor(Display &d, Setting &s)
{
    Uint32 bgColor = parseHexColor(s.getValueStr());
    
    d.setBGColor(bgColor);
    
    return true;
}