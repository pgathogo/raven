#include "pixelplanapp.h"

PixelPlanApp::PixelPlanApp(int& argc, char** argv)
    : QApplication(argc, argv)
    , m_setup{nullptr}
{
    m_setup = new RavenSetup();

}
