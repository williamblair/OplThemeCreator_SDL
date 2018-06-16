#include <guisan.hpp>
#include <guisan/sdl.hpp>

#include <vector>
#include <string>

#ifndef ENTRY_GROUP_H_INCLUDED
#define ENTRY_GROUP_H_INCLUDED

typedef struct 
{
    /* Show what the entries are for */
    gcn::Label *label;
    
    /* The x and y inputs */
    gcn::TextField *textFieldX;
    gcn::TextField *textFieldY;

} EntryGroup;

#endif

