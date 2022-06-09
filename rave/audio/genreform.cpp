#include "genreform.h"
#include "ui_genreform.h"

#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"

#include "../audio/audio.h"

GenreForm::GenreForm(AUDIO::Genre* genre,
                  QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::GenreForm)
    ,m_genre{genre}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    setMaximumWidth(400);
    setMaximumHeight(115);

    populateFormWidgets();

    ui->edtGenreName->setFocus(Qt::ActiveWindowFocusReason);
}

GenreForm::~GenreForm()
{
    delete ui;
}

ActionResult GenreForm::saveRecord()
{
    populateEntityFields();
    return m_genre->validate();
}

std::string GenreForm::windowTitle()
{
    return "Genre Form";
}

void GenreForm::populateEntityFields()
{
    m_genre->set_genre_name(ui->edtGenreName->text().toStdString());
}

void GenreForm::populateFormWidgets()
{
    ui->edtGenreName->setText(stoq(m_genre->genre_name()->value()));
}
