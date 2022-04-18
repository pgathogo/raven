#include <QDebug>

#include "applicationcontext.h"

namespace FRAMEWORK
{

    ApplicationContext* ApplicationContext::m_application_context = nullptr;

    ApplicationContext::ApplicationContext(QApplication* app)
    {
        qDebug() << "Context created ...";
        m_app = app;
    }

    QApplication* ApplicationContext::application()
    {
        return m_app;
    }

}
