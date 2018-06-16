#include "GuisanWindow.hpp"

GuisanWindow::GuisanWindow(void)
{
    m_Window  = NULL;
    m_Surface = NULL;

    m_Input       = NULL;
    m_Graphics    = NULL;
    m_ImageLoader = NULL;

    m_Gui   = NULL;
    m_Font  = NULL;
    m_Top   = NULL;
    m_Label = NULL;
    
    m_TFActionListener = NULL;

    curInputX = curInputY = 10;
}

void GuisanWindow::close(void)
{ 
    for(std::vector<EntryGroup>::iterator i = m_EntryGroups.begin(); i != m_EntryGroups.end(); i++)
    {
        /* Cast the iterator as our current entry */
        EntryGroup e = *i;

        if ( e.label != NULL) {
            delete e.label;
            e.label = NULL;
        }

        if (e.textFieldX != NULL) {
            delete e.textFieldX;
            e.textFieldX = NULL;
        }

        if (e.textFieldY != NULL) {
            delete e.textFieldY;
            e.textFieldY = NULL;
        }
    }

    if (m_Label) { delete m_Label; m_Label = NULL; }
    if (m_Font) { delete m_Font; m_Font = NULL; }
    if (m_Top) { delete m_Top; m_Top = NULL; }
    if (m_Gui) { delete m_Gui; m_Gui = NULL; }

    if (m_Input) { delete m_Input; m_Input = NULL; }
    if (m_Graphics) { delete m_Graphics; m_Graphics = NULL; }
    if (m_ImageLoader) { delete m_ImageLoader; m_ImageLoader = NULL; }
    
    if (m_Window) { SDL_DestroyWindow(m_Window); m_Window = NULL; }
}

bool GuisanWindow::init(void)
{
    /* Init SDL stuff */
    m_Window = SDL_CreateWindow(
        "Guisan Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        S_WIDTH, S_HEIGHT,
        0
    );
    if (!m_Window) {
        fprintf(stderr, "Guisan::init: failed to create window: %s\n", SDL_GetError());
        return false;
    }

    m_Surface = SDL_GetWindowSurface(m_Window);
    if (!m_Surface) {
        fprintf(stderr, "Guisan::init: failed to get window surface: %s\n", SDL_GetError());
        return false;
    }

    /* Create the guisan image loader */
    m_ImageLoader = new gcn::SDLImageLoader();
    gcn::Image::setImageLoader(m_ImageLoader);

    /* Create guisan graphics */
    m_Graphics = new gcn::SDLGraphics();
    m_Graphics->setTarget(m_Surface);

    /* Create the guisan input */
    m_Input = new gcn::SDLInput();


    /* Create Basic Widgets */
    m_Top = new gcn::Container();
    m_Top->setDimension(gcn::Rectangle(0, 0, S_WIDTH, S_HEIGHT));

    m_Gui = new gcn::Gui();
    m_Gui->setGraphics(m_Graphics);
    m_Gui->setInput(m_Input);
    m_Gui->setTop(m_Top);

    m_Font = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    gcn::Widget::setGlobalFont(m_Font);

    /* Global action listener for each input to use */
    m_TFActionListener = new TextFieldActionListener();

    // TODO - remove me 
    m_Label = new gcn::Label("Hello World!");
    m_Label->setPosition(280, 220);
    m_Top->add(m_Label);

    /* Create our input widgets */
/*    initTextField(m_MenuIconInput, InputIDs[MenuIcon]);
    initTextField(m_ItemsListInput, InputIDs[ItemsList]);
    initTextField(m_ItemCoverInput, InputIDs[ItemCover]);*/
    initEntryGroup(InputIDs[MenuIcon]);
    initEntryGroup(InputIDs[ItemsList]);
    initEntryGroup(InputIDs[ItemCover]);

    return true;
}

bool GuisanWindow::update(void)
{
    m_Gui->logic();
    m_Gui->draw();

    SDL_UpdateWindowSurface(m_Window);

    return true;
}

bool GuisanWindow::sendEvents(SDL_Event *event)
{
    m_Input->pushInput(*event);
    
    return true;
}


int GuisanWindow::getWindowId(void)
{
    return (m_Window ? SDL_GetWindowID(m_Window) : -1);
}

void GuisanWindow::initEntryGroup(std::string id)
{
    /* Define our new entry group */
    EntryGroup eg;
    m_EntryGroups.push_back(eg);
    m_EntryGroups.back().label      = new gcn::Label(id);
    
    m_EntryGroups.back().textFieldX = new gcn::TextField(std::string(" ")); 
    m_EntryGroups.back().textFieldY = new gcn::TextField(std::string(" ")); 
    
    m_EntryGroups.back().textFieldX->setWidth(50);
    m_EntryGroups.back().textFieldY->setWidth(50);
    
    m_EntryGroups.back().textFieldX->addActionListener(m_TFActionListener);
    m_EntryGroups.back().textFieldY->addActionListener(m_TFActionListener);

    m_EntryGroups.back().textFieldX->setActionEventId(id + "X");
    m_EntryGroups.back().textFieldY->setActionEventId(id + "Y");

    m_Top->add(m_EntryGroups.back().label,      curInputX,     curInputY);
    m_Top->add(m_EntryGroups.back().textFieldX, curInputX+60, curInputY);
    m_Top->add(m_EntryGroups.back().textFieldY, curInputX+120, curInputY);

    curInputY += 30;
    if(curInputY >= (S_HEIGHT - 50))
    {
        curInputY = 10;
        curInputX += 120;
    }
    
}





