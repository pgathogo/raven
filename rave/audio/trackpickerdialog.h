#ifndef TRACKPICKERDIALOG_H
#define TRACKPICKERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>

class QPushButton;


namespace AUDIO
{
    class Audio;
    class TrackBrowser;

    class SelectionWidget: public QWidget
    {
        Q_OBJECT
    public:
        SelectionWidget();
    private slots:
        void select_clicked();
        void cancel_clicked();
    signals:
        void audio_selected();
        void cancel_selection();
    private:
        std::unique_ptr<QHBoxLayout> m_h_layout;
        std::unique_ptr<QPushButton> m_select_btn;
        std::unique_ptr<QPushButton> m_cancel_btn;
    };

    class TrackPickerDialog : public QDialog
    {
        Q_OBJECT
    public:
        TrackPickerDialog(QWidget* parent=nullptr);
    private slots:
        void audio_selected();
        void cancel_selection();
    signals:
        void selected_audio(std::shared_ptr<AUDIO::Audio>);
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<AUDIO::TrackBrowser> m_track_browser;
        std::unique_ptr<SelectionWidget> m_selection_widget;
    };
}

#endif // TRACKPICKERDIALOG_H
