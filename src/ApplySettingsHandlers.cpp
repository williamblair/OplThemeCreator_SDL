#include "ApplySettingsHandlers.hpp"

void hApplyBackground(std::vector<Setting> elementSettings, int i)
{
    /* Open the image */
    for (Setting s : elementSettings)
    {
        /* The main background image */
        if (s.getName() == "default") {
            std::string image = themeDir + "/" + s.getValueStr() + ".jpg";
            elements.at(i).addImage(image);
        }
    }
}

void hApplyMenuIcon(std::vector<Setting> elementSettings, int i)
{
    /* The position of the menu icons */
    int x = 0, y = 0;

    /* The menu icon - which device games are begin read from */
    for (Setting s : elementSettings)
    {
        if (s.getName() == "x") {
            x = s.getValueInt();
        }
        else if (s.getName() == "y") {
            y = s.getValueInt();
        }
    }

    /* Load each menu icon image */
    for (int j = 0; j < menuIconList.size(); j++)
    {
        /* The menu icon list - which device we're reading games from */
        std::string image = themeDir + "/" + menuIconList.at(j);
        elements.at(i).addImage(image);

        /* Update the image position */
        elements.at(i).setPosCentered(x, y);
    }
}

void hApplyItemsList(std::vector<Setting> elementSettings, int i)
{
    /* The x and y pos of the Games list */
    int x = 0, y = 0;
    for (Setting s : elementSettings)
    {
        if (s.getName() == "x") {
            x = s.getValueInt();
        }
        else if (s.getName() == "y") {
            y = s.getValueInt();
        }
    }

    gamesListFontHandler.setX(x);
    gamesListFontHandler.setY(y);
}

void hApplyItemCover(std::vector<Setting> elementSettings, int i)
{
    /* The x and y pos of the game cover */
    int x = 0, y = 0;

    /* The default size of the game cover */
    int coverWidth = 140, coverHeight = 200;

    /* The position of the game cover overlay, which will decide
     * the new size of the cover image */
    int ulx = 0, uly = 0, urx = 0, ury = 0, llx = 0, lly = 0, lrx = 0, lry = 0;

    /* The new size of the cover image based on the above coordinates */
    int newCoverWidth = 1, newCoverHeight = 1;

    /* The name of the item cover image (assumes.png for now...) */
    std::string coverImage = "";

    for (Setting s : elementSettings)
    {
        if (s.getName() == "x") {
            x = s.getValueInt();
        }
        else if (s.getName() == "y") {
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

        else if (s.getName() == "overlay") {
            coverImage = s.getValueStr();
        }
    }

    /* A default cover image */
    std::string image = "images/p4cover.png";
    elements.at(i).addImage(image);
    elements.at(i).setPosCentered(x, y);
    
    /* Calculate and resize the cover image */
    newCoverWidth = urx - ulx;
    newCoverHeight = lly - uly;
    elements.at(i).setSize(newCoverWidth, newCoverHeight);

    /* Load, calculate the upper left pos of the cover,
     * and set it */
    image = themeDir + "/" + coverImage + ".png";
    elements.at(i).addImage(image);
    elements.at(i).setPos((x-(coverWidth/2))-ulx, (y-(coverHeight/2))-uly);
}

void hApplyLoadingIcon(std::vector<Setting> elementSettings, int i)
{
    int x = 0, y = 0;
    for (Setting s : elementSettings) 
    {
        /* Wrap values if needed */
        if (s.getName() == "x") {
            x = s.getValueInt();
            if(x < 0) x = 640 + x;
        }
        else if (s.getName() == "y") {
            y = s.getValueInt();
            if(y < 0) y = 480 + y;
        }
    }

    /* Instead of putting these images in the element, 
     * they're stored in their own vector so we can animate them */
    for (int i = 0; i < 8; i++)
    {
        std::stringstream image;
        image << themeDir << "/load" << i  << ".png";
        loadingIcons.push_back(new Sprite);
        loadingIcons.back()->loadImage(image.str().c_str());
        loadingIcons.back()->setX(x);
        loadingIcons.back()->setY(y);
    }
}

void hApplyHintText(std::vector<Setting> elementSettings, int i)
{
    int y = 0;
    for(Setting s : elementSettings)
    {
        if(s.getName() == "y") {
            y = s.getValueInt();
        }
    }
    
    /* Load each hint text image */
    elements.at(i).addImage(themeDir+"/start.png");
    elements.at(i).setPos(0, y);
    elements.at(i).addImage(themeDir+"/cross.png");
    elements.at(i).setPos(100, y);
    elements.at(i).addImage(themeDir+"/triangle.png");
    elements.at(i).setPos(170, y);
    elements.at(i).addImage(themeDir+"/select.png");
    elements.at(i).setPos(320, y);
    elements.at(i).addImage(themeDir+"/circle.png");
    elements.at(i).setPos(440, y);
    elements.at(i).addImage(themeDir+"/square.png");
    elements.at(i).setPos(545, y);
    
    /* Load each hint text */
    hintTextFontHandler.setColor(settingColors["hint_text_color"]);
    hintTextFontHandler.addMessage(hintTextList.at(0), 40, y);
    hintTextFontHandler.addMessage(hintTextList.at(1), 125, y);
    hintTextFontHandler.addMessage(hintTextList.at(2), 190, y);
    hintTextFontHandler.addMessage(hintTextList.at(3), 360, y);
    hintTextFontHandler.addMessage(hintTextList.at(4), 460, y);
    hintTextFontHandler.addMessage(hintTextList.at(5), 570, y);
}

