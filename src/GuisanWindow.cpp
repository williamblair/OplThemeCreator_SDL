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
}

GuisanWindow::~GuisanWindow(void)
{
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


    /* Create widgets */
    m_Top = new gcn::Container();
    m_Top->setDimension(gcn::Rectangle(0, 0, S_WIDTH, S_HEIGHT));

    m_Gui = new gcn::Gui();
    m_Gui->setGraphics(m_Graphics);
    m_Gui->setInput(m_Input);
    m_Gui->setTop(m_Top);

    m_Font = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    gcn::Widget::setGlobalFont(m_Font);

    m_Label = new gcn::Label("Hello World!");
    m_Label->setPosition(280, 220);
    m_Top->add(m_Label);

    return true;
}

bool GuisanWindow::update(void)
{
    m_Gui->logic();
    m_Gui->draw();

    SDL_UpdateWindowSurface(m_Window);

    return true;
}
