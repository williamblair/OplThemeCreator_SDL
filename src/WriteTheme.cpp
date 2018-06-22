#include "WriteTheme.hpp"

void writeTheme(void)
{
    /* Back up the existing theme file */
    std::filesystem::path themePath(themeDir);
    std::filesystem::rename(themePath/"conf_theme.cfg", themePath/"conf_theme.cfg.bak");

    /* Open the file to write to */
    std::ofstream outFile(themePath/"conf_theme.cfg");

    /* First write the setting colors to the file */
    outFile << "# This is bj's theme boyyy\n";

    outFile.close();
}

