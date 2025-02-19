#ifndef AUDIOSEARCHWIDGET_H
#define AUDIOSEARCHWIDGET_H

#include <QLineEdit>
#include <QShowEvent>

#include <memory>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

using Filter = std::tuple<std::string, std::string, std::string>;

namespace  AUDIO {

    class AudioSearchWidget : public QWidget
    {
        Q_OBJECT
    public:
        const int COL0 = 0;
        const int COL1 = 1;
        const int COL2 = 2;
        const int ROW0 = 0;
        const int ROW1 = 1;
        const int ROW2 = 2;

        AudioSearchWidget();
        void layout_controls();
    signals:
        void search_filter(Filter);

    protected:
        void showEvent(QShowEvent*);

    private slots:
        void search_clicked();
    private:
        std::unique_ptr<QVBoxLayout> m_base_layout;
        std::unique_ptr<QHBoxLayout> m_button_layout;
        std::unique_ptr<QGridLayout> m_grid_layout;

        std::unique_ptr<QLabel> m_lbl_title;
        std::unique_ptr<QLineEdit> m_edt_title;

        std::unique_ptr<QLabel> m_lbl_artist;
        std::unique_ptr<QLineEdit>m_edt_artist;

        std::unique_ptr<QPushButton>m_btn_search;

        void style_search_button();
    };


}


#endif
