#pragma once

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
 class ArtistForm;
}

namespace AUDIO{
 class Artist;
}

template<typename T>
class ChoiceField;


class ArtistForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ArtistForm(AUDIO::Artist* artist,
                        QDialog *parent = nullptr);

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populate_choice_combo(QComboBox*, const ChoiceField<std::string>*);
    void set_choice_field_default(QComboBox*, const std::string);

    ~ArtistForm();

    template<typename T>
    void set_choice_field(T* cf, QComboBox* cbox)
    {
        cf->setValue(cbox->itemData(cbox->currentIndex()).toString().toStdString());
    }

private slots:
    void artist_type_changed(int);
    void update_fullname_first(const QString);
    void update_fullname_last(const QString);

private:
    Ui::ArtistForm* ui;

    AUDIO::Artist* m_artist;

};

