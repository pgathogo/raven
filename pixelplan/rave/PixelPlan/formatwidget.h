#ifndef FORMATWIDGET_H
#define FORMATWIDGET_H

#include <QWidget>

class QLabel;
class QColor;
class QPushButton;
class QSpinBox;
class QComboBox;

namespace PIXELPLAN {

class FormatWidget: public QWidget {

    Q_OBJECT

public:
    explicit FormatWidget();

private:
    QLabel* m_lbl_font;        // QFontDialog - font, style, size
    QLabel* m_font;

    QLabel* m_lbl_font_color;  // QColorDialog -
    QPushButton* m_btn_font_color;

    QLabel* m_lbl_bg_color;    // QColorDialog -
    QPushButton* m_btn_bg_color;

    QLabel* m_header_space;
    QSpinBox* m_header_space_spin;

    QLabel* m_footer_space;
    QSpinBox* m_footer_space_spin;

    QLabel* m_lbl_alignment;
    QComboBox* m_alignment_combo; // left, center, right

};



}




#endif
