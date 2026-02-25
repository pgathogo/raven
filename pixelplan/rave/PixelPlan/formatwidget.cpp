#include <QGridLayout>
#include <QLabel>

#include  "formatwidget.h"

namespace PIXELPLAN {

    FormatWidget::FormatWidget()
    : m_lbl_font{nullptr}
    ,m_font{nullptr}
    ,m_lbl_font_color{nullptr}
    ,m_btn_font_color{nullptr}

    ,m_lbl_bg_color{nullptr}
    ,m_btn_bg_color{nullptr}

    ,m_header_space{nullptr}
    ,m_header_space_spin{nullptr}

    ,m_footer_space{nullptr}
    ,m_footer_space_spin{nullptr}

    ,m_lbl_alignment{nullptr}
    ,m_alignment_combo{nullptr}
    {
        // Make grid layout with 2 columns and 5 rows
        QGridLayout* glayout = new QGridLayout(this);

        m_lbl_font = new QLabel("Font");
        m_font = new QLabel("Sample Font");

        m_lbl_font_color = new QLabel("Font Color");
        m_btn_font_color = new QPushButton("Choose Font Color");

        m_lbl_bg_color = new QLabel("Background Color");
        m_btn_bg_color = new QPushButton("Choose Background Color");

        m_header_space = new QLabel("Header Space");
        m_header_space_spin = new QSpinBox();
        m_header_space_spin->setMaximum(5);

        m_footer_space = new QLabel("Footer Space");
        m_footer_space_spin = new QSpinBox();
        m_footer_space_spin->setMaximum(5);

        m_lbl_alignment = new QLabel("Alignment");
        m_alignment_combo = new QComboBox();
        m_alignment_combo->addItem("Left");
        m_alignment_combo->addItem("Center");
        m_alignment_combo->addItem("Right");

        glayout->addWidget(m_lbl_font, 0, 0);
        glayout->addWidget(m_font, 0, 1);

        glayout->addWidget(m_lbl_font_color, 1, 0);
        glayout->addWidget(m_btn_font_color, 1, 1);

        glayout->addWidget(m_lbl_bg_color, 2, 0);
        glayout->addWidget(m_btn_bg_color, 2, 1);

        glayout->addWidget(m_header_space, 3, 0);
        glayout->addWidget(m_header_space_spin, 3, 1);

        glayout->addWidget(m_footer_space, 4, 0);
        glayout->addWidget(m_footer_space_spin, 4, 1);

        glayout->addWidget(m_lbl_alignment, 5, 0);
        glayout->addWidget(m_alignment_combo, 5, 1);

        setLayout(glayout);

    }

}
