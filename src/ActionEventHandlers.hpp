#include <guisan.hpp>
#include <guisan/sdl.hpp>
#include <SDL.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdio>

#include "Element.hpp"
#include "Setting.hpp"
#include "FontHandler.hpp"

#ifndef ACTION_EVENT_HANDLERS_INCLUDED
#define ACTION_EVENT_HANDLERS_INCLUDED

/* The text list of games */
extern FontHandler gamesListFontHandler;

/* The text list of hint icons */
extern FontHandler hintTextFontHandler;

/* The elements list we'll want to modify */
extern std::vector<Element> elements;

/* each menu icon image name */
extern std::vector<std::string> menuIconList;

/* Animated loading icons list */
extern std::vector<Sprite *> loadingIcons;

/* List of hint text items labels */
extern std::vector<std::string> hintTextList;

/* What we want to name our text input */
enum inputEnum { MenuIcon, ItemsList, ItemCover, LoadingIcon, HintText };
static std::vector<std::string> InputIDs = {
    "MenuIcon",
    "ItemsList",
    "ItemCover",
    "LoadingIcon",
    "HintText"
};
 
void hMenuIconAction(const gcn::ActionEvent &actionEvent);
void hItemsListAction(const gcn::ActionEvent &actionEvent);
void hItemCoverAction(const gcn::ActionEvent &actionEvent);
void hLoadingIconAction(const gcn::ActionEvent &actionEvent);
void hHintTextAction(const gcn::ActionEvent &actionEvent);

/* Used by each of the above to find their corresponding element */
Element * findElement(std::string valueStr);

#endif
