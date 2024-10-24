#pragma once

#include <memory>
#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class QComboBox;

namespace Ui {
    class AudioForm;
}

namespace AUDIO{
  class Audio;
  class Artist;
  class Genre;
  class Folder;
  class AudioTool;
}

class PickListBrowser;

template<typename T>
class ChoiceField;

class RavenSetup;

enum class FormMode{ReadWrite, ReadOnly, WriteOnly};

class AudioForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    enum ArtistColumn{FirstName=0, LastName, FullName, ArtistType, Notes};


    explicit AudioForm(AUDIO::Audio*,
                       RavenSetup* setup,
                       FormMode form_mode = FormMode::ReadWrite,
                       QDialog* parent = nullptr);
    ~AudioForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populate_choice_combo(QComboBox*, const ChoiceField<std::string>*);
    void set_choice_field_default(QComboBox*, const std::string);

    int item_index(QComboBox*, const QString);

    template<typename T>
    void set_choice_field(T* cf, QComboBox* cbox)
    {
        cf->setValue(cbox->itemData(cbox->currentIndex()).toString().toStdString());
    }

private slots:
    void pick_artist();
    void genre_combo_changed(int);


private:
    Ui::AudioForm *ui;
    AUDIO::Audio* m_audio;
    RavenSetup* m_setup;
    std::unique_ptr<PickListBrowser> m_artist_picker;
    std::unique_ptr<AUDIO::AudioTool> m_audio_tool;
    std::string m_title_tag{""};

    FormMode m_form_mode;

    void setup_ui();
};

