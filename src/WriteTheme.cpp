#include "WriteTheme.hpp"

void writeTheme(void)
{
    /* Back up the existing theme file */
    std::filesystem::path themePath(themeDir);
    std::filesystem::rename(themePath/"conf_theme.cfg", themePath/"conf_theme.cfg.bak");

    /* Open the file to write to */
    std::ofstream outFile(themePath/"conf_theme.cfg");

    /* First write the setting colors to the file */
    outFile << std::hex;
    outFile << "# Global Colors\n";
    for(std::map<std::string, Uint32>::iterator it = settingColors.begin(); it != settingColors.end(); it++)
    {
        outFile << it->first << "=#" << it->second << "\n";
    }

    /* Add the background color */
    outFile << "bg_color=#" << d.getBGColor() << "\n";

    /* Add default font */
    outFile << "default_font=" << gamesListFontHandler.getFontName() << "\n";

    /* Back to using decimal numbers please; */
    outFile << std::dec;

    /* Now write the main settings */
    outFile << "\n# Main elements\n";
    for (int i = 0; i < elements.size(); i++)
    {
        Element *e = &elements.at(i);
        outFile << e->getName() << ":\n";
        std::vector<Setting> *settings = e->getSettings();
        for (int j = 0; j < settings->size(); j++) 
        {
            Setting s = settings->at(j);

            int val = s.getValueInt();
            std::string str = s.getValueStr();

            outFile << "\t" << s.getName() << "=";
            if (val != -1) outFile << val;
            else           outFile << str;

            outFile << "\n";
        }
    }

    outFile.close();
}

