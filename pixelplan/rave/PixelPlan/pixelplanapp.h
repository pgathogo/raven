#ifndef PIXELPLANAPP_H
#define PIXELPLANAPP_H

#include <QApplication>
#include "../../../rave/framework/ravensetup.h"


class PixelPlanApp : public QApplication
{
    Q_OBJECT

public:
    PixelPlanApp(int& argc, char** argv);
    ~PixelPlanApp();

    RavenSetup* setup() const { return m_setup; }

    static PixelPlanApp* instance();

private:
    RavenSetup* m_setup;
};


#endif
