//#include "../framework/treeviewmodel.h"

#include "trackbrowser.h"
#include "trackpickerdialog.h"

namespace AUDIO
{
    SelectionWidget::SelectionWidget()
        :m_h_layout{nullptr}
        ,m_select_btn{nullptr}
        ,m_cancel_btn{nullptr}
    {
        m_select_btn = std::make_unique<QPushButton>("Select");
        connect(m_select_btn.get(), &QPushButton::clicked, this, &SelectionWidget::select_clicked);
        m_cancel_btn = std::make_unique<QPushButton>("Close");
        connect(m_cancel_btn.get(), &QPushButton::clicked, this, &SelectionWidget::cancel_clicked);

        m_h_layout = std::make_unique<QHBoxLayout>();

        m_h_layout->addStretch();
        m_h_layout->addWidget(m_select_btn.get());
        m_h_layout->addWidget(m_cancel_btn.get());

        setLayout(m_h_layout.get());


    }

    void SelectionWidget::select_clicked()
    {
        emit audio_selected();

    }

    void SelectionWidget::cancel_clicked()
    {
        emit cancel_selection();
    }


    TrackPickerDialog::TrackPickerDialog(QWidget* parent)
        :QDialog(parent)
        ,m_v_layout{nullptr}
        ,m_track_browser{nullptr}
        ,m_selection_widget{nullptr}
    {
        m_track_browser = std::make_unique<AUDIO::TrackBrowser>();
        m_selection_widget = std::make_unique<SelectionWidget>();

        connect(m_selection_widget.get(), &SelectionWidget::audio_selected, this, &TrackPickerDialog::audio_selected);
        connect(m_selection_widget.get(), &SelectionWidget::cancel_selection, this, &TrackPickerDialog::cancel_selection);

        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->addWidget(m_track_browser.get());
        m_v_layout->addWidget(m_selection_widget.get());

        setMinimumWidth(600);
        setMinimumWidth(500);

        setLayout(m_v_layout.get());

        setStyleSheet("QDialog{background: #555D64;}");
    }

    void TrackPickerDialog::audio_selected()
    {
        emit selected_audio(m_track_browser->current_selected_audio());

    }

    void TrackPickerDialog::cancel_selection()
    {
        done(0);
    }
}
