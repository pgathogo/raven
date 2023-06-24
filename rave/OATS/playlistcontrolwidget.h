#pragma once

#include <memory>

#include <QWidget>

class QLabel;
class QPushButton;
class QHBoxLayout;

namespace OATS{
    class PlayListControlWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit PlayListControlWidget(QWidget *parent = nullptr);
        ~PlayListControlWidget();

        void style_button(const QString);

    public slots:
        void go_current_clicked();
        void keep_current_clicked();

    signals:
        void go_current();
        void keep_current();

    private:
        std::unique_ptr<QLabel> m_title;
        std::unique_ptr<QPushButton> m_go_current;
        std::unique_ptr<QPushButton> m_keep_current;

        QHBoxLayout* m_main_layout;
    };
}
