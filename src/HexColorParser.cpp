#include "HexColorParser.hpp"

Uint32 parseHexColor(const std::string &colorStr)
{
    Uint32 result=0;
    
    /* Make sure we have a valid string */
    if(colorStr.length() == 0 || colorStr[0] != '#') {
        printf("Hex Color Parser: Invalid color string: %s\n", colorStr.c_str());
        return 0;
    }
    
    /* Read the remaining part of the string as a hex number */
    sscanf(colorStr.c_str(), "#%X", &result);
    
    return result;
}

