#ifndef TRACKBROWSER_H
#define TRACKBROWSER_H

#include <memory>

#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>

namespace AUDIO
{
    class AudioFolderWidget;
    class AudioSearchWidget;
    class AudioTrackWidget;
    class AudioDataModel;
    class Audio;

    using Filter = std::tuple<std::string, std::string, std::string>;

    class TrackBrowser : public QWidget
    {
        Q_OBJECT
    public:
        explicit TrackBrowser(QWidget *parent = nullptr);

        void init_top_widget(QSplitter*);
        void init_bottom_widget(QSplitter*);

        std::shared_ptr<Audio> current_selected_audio();
        std::shared_ptr<Audio> find_audio_by_id(int);

    signals:

    private slots:
        void folder_clicked(int);
        void selected_audio(std::shared_ptr<Audio>);
        void search_filter(Filter);

    private:
       std::unique_ptr<QWidget> m_top_widget;
       std::unique_ptr<QWidget> m_bottom_widget;
       std::unique_ptr<QVBoxLayout> m_main_layout;

       std::unique_ptr<QTabWidget> m_tab_widget;
       QHBoxLayout* btn_h_layout;

       // std::unique_ptr<QPushButton> m_btn_listen;
       // std::unique_ptr<QPushButton> m_btn_info;
       // std::unique_ptr<QPushButton> m_btn_history;

       std::unique_ptr<AudioFolderWidget> m_audio_folder_widget;
       std::unique_ptr<AudioSearchWidget> m_search_widget;

       std::unique_ptr<AudioTrackWidget> m_audio_track_widget;
       std::unique_ptr<AudioDataModel> m_audio_data_model;

       QSplitter* m_main_splitter;

       std::shared_ptr<Audio> m_current_selected_audio;

       void set_tab_widget_style();
    };




}

#endif // TRACKBROWSER_H
