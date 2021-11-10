#pragma once

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
}

class PickListBrowser;

template<typename T>
class ChoiceField;


class AudioForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    enum ArtistColumn{FirstName=0, LastName, FullName, ArtistType, Notes};

    explicit AudioForm(AUDIO::Audio*,
                       QDialog* parent = nullptr);
    ~AudioForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populate_choice_combo(QComboBox*, const ChoiceField<std::string>*);
    void set_choice_field_default(QComboBox*, const std::string);

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
    std::unique_ptr<PickListBrowser> m_artist_picker;

};

