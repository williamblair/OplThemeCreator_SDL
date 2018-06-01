// One of the 'Main' (e.g. main0, main1, ...) elements of the 
// OPL theme, like image locations
#include "Setting.hpp"
#include "Sprite.hpp"

#include <cstdio>
#include <vector>
#include <sstream>

#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED

class Element
{
public:
    Element(void);
    ~Element(void);

    /* Get the element name (like main0) */
    bool parse(const std::string line);

    /* Parse a setting and add to the settings vector */
    bool addSetting(Setting setting);

    /* Print out the element to stdout */
    void print(void);

    /* Return the settings list */
    std::vector<Setting> getSettings(void);

    /* Return the element name */
    std::string getName(void);

    /* Load an image from the given file into
     * the sprite member */
    bool addImage(const std::string &imageStr);

    /* Draw the sprite if we have one */
    bool draw(SDL_Surface *s);

    /* Set the position of the sprite */
    bool setPos(int x, int y);

    /* Set the position of the sprite with the given
     * x and y representing the center of the image */
    bool setPosCentered(int x, int y);

private:
    std::string m_name;

    std::vector<Setting> m_settings;

    bool m_hasImage;
    Sprite m_sprite;
};

#endif
