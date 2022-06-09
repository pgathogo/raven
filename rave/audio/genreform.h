#pragma once

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

namespace Ui {
class GenreForm;
}

namespace AUDIO{
  class Genre;
}

class GenreForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit GenreForm(AUDIO::Genre* genre,
                       QDialog *parent = nullptr);
    ~GenreForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::GenreForm *ui;
    AUDIO::Genre* m_genre;
};

