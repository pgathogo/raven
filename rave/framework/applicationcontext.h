#pragma once

#include <QApplication>

namespace FRAMEWORK
{

    class ApplicationContext
    {
    public:
        ApplicationContext(const ApplicationContext& context) = delete;
        ApplicationContext& operator=(const ApplicationContext& context) = delete;

        static ApplicationContext* instance(QApplication* app = nullptr){
            return (!m_application_context) ? m_application_context = new ApplicationContext(app) : m_application_context;
        }

        QApplication* application();

    private:
        ApplicationContext(QApplication* app);
        static ApplicationContext* m_application_context;
        QApplication* m_app;
    };

}
