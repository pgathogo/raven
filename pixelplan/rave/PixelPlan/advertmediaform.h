#ifndef ADVERTMEDIAFORM_H
#define ADVERTMEDIAFORM_H

#include <QWidget>
#include <QMediaPlayer>
#include <QTime>


#include "../framework/baseentitydetaildlg.h"

class QComboBox;
//class QMediaPlayer;
class QVideoWidget;
class QAudioOutput;

template<typename T>
class ChoiceField;


namespace Ui {
    class AdvertMediaForm;
}

namespace PIXELPLAN {
    class AdvertMedia;
}

class AdvertMediaForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit AdvertMediaForm(std::shared_ptr<PIXELPLAN::AdvertMedia>,
                             FormMode form_mode = FormMode::ReadWrite,
        QDialog* parent = nullptr);
    ~AdvertMediaForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populate_rating_combo(QComboBox*, const ChoiceField<std::string>*);
    void set_rating_field_default(QComboBox*, const std::string);

    int item_index(QComboBox*, const QString);

private slots:
    void play_media();
    void stop_media();
    void on_media_status_changed(QMediaPlayer::MediaStatus);
    void on_duration_changed(qint64);

private:
    void load_media(const QString);
    QTime duration_to_time(qint64);
    void setChoiceFieldDefault(QComboBox*, const std::string);
    void set_choice_field(ChoiceField<std::string>*, QComboBox*);
    std::string get_extension(const std::string);

    Ui::AdvertMediaForm *ui;
    std::shared_ptr<PIXELPLAN::AdvertMedia> m_advert_media;
    std::string m_title_tag {""};

    FormMode m_form_mode;
    std::unique_ptr<QMediaPlayer> m_media_player;
    std::unique_ptr<QVideoWidget> m_video_widget;
    std::unique_ptr<QAudioOutput> m_audio_output;
};

#endif // ADVERTMEDIAFORM_H
