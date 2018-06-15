#include "ActionEventHandlers.hpp"

void hMenuIconAction(const gcn::ActionEvent &actionEvent)
{
    /* Get the widget pointer */
    gcn::TextField *t = (gcn::TextField*)actionEvent.getSource();

    /* See what our text value currently is */
    printf("Text Input: %s\n", t->getText().c_str());

    int x=0;
    sscanf(t->getText().c_str(), "%d", &x);
    printf("scanf'd x: %d\n", x);
    
    Element *e = findElement(InputIDs[MenuIcon]);
    if (e) {
        for (int j = 0; j < menuIconList.size(); j++)
        {
            int curY = e->getYCentered(j);
            e->setPosCentered(x, curY, j);
        }
    }
}

void hItemsListAction(const gcn::ActionEvent &actionEvent)
{
    gcn::TextField *t = (gcn::TextField*)actionEvent.getSource();

    int x = 0;
    sscanf(t->getText().c_str(), "%d", &x);

    for (int i = 0; i < gamesListFontHandler.getNumMessages(); i++)
    {
        gamesListFontHandler.setX(x, i);
    }
}

void hItemCoverAction(const gcn::ActionEvent &actionEvent)
{
    gcn::TextField *t = (gcn::TextField*)actionEvent.getSource();
    
    /* The x and y pos of the game cover */
    int x = 0, y = 0;
    
    sscanf(t->getText().c_str(), "%d", &x);
    
    Element *e = findElement(InputIDs[ItemCover]);
    
    if(e) {

        /* The default size of the game cover */
        int coverWidth = 140, coverHeight = 200;

        /* The position of the game cover overlay, which will decide
         * the new size of the cover image */
        int ulx = 0, uly = 0, urx = 0, ury = 0, llx = 0, lly = 0, lrx = 0, lry = 0;

        /* The new size of the cover image based on the above coordinates */
        int newCoverWidth = 1, newCoverHeight = 1;
        
        std::vector<Setting> elementSettings = e->getSettings();

        for (Setting s : elementSettings)
        {
            if (s.getName() == "y") {
                y = s.getValueInt();
            }
            
            else if (s.getName() == "overlay_ulx") {
                ulx = s.getValueInt();
            }

            else if (s.getName() == "overlay_uly") {
                uly = s.getValueInt();
            }

            else if (s.getName() == "overlay_urx") {
                urx = s.getValueInt();
            }

            else if (s.getName() == "overlay_ury") {
                ury = s.getValueInt();
            }

            else if (s.getName() == "overlay_llx") {
                llx = s.getValueInt();
            }

            else if (s.getName() == "overlay_lly") {
                lly = s.getValueInt();
            }

            else if (s.getName() == "overlay_lrx") {
                lrx = s.getValueInt();
            }

            else if (s.getName() == "overlay_lry") {
                lry = s.getValueInt();
            }
        }

        /* Reset the image size to original otherwise it screws up
         * moving calculations */
        e->setSize(140, 200, 0);

        /* Set the new postion of the cover image */
        e->setPosCentered(x, y, 0);
        
        /* Calculate and resize the cover image */
        newCoverWidth = urx - ulx;
        newCoverHeight = lly - uly;
        e->setSize(newCoverWidth, newCoverHeight, 0);

        /* calculate the upper left pos of the cover,
         * and set it */
        e->setPos((x-(coverWidth/2))-ulx, (y-(coverHeight/2))-uly, 1);
    }
}

Element * findElement(std::string valueStr)
{
    for (int i = 0; i < elements.size(); i++)
    {
        if (elements.at(i).getSettings().at(0).getValueStr() == valueStr)
            return &elements.at(i);
    }

    return NULL;
}
