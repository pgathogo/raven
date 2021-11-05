#include "artistform.h"
#include "ui_artistform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/choicefield.h"

#include "../audio/artist.h"

ArtistForm::ArtistForm(AUDIO::Artist* artist,
                       QDialog *parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::ArtistForm)
    ,m_artist{artist}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    setMaximumWidth(390);
    setMaximumHeight(285);

    populateFormWidgets();

    connect(ui->edtFirstName, &QLineEdit::textChanged, this, &ArtistForm::update_fullname_first);
    connect(ui->edtSurname, &QLineEdit::textChanged, this, &ArtistForm::update_fullname_last);

    connect(ui->cbArtistType, QOverload<int>::of(QComboBox::currentIndexChanged),
            this, &ArtistForm::artist_type_changed);

}

ArtistForm::~ArtistForm()
{
    delete ui;
}

ActionResult ArtistForm::saveRecord()
{
    populateEntityFields();
    return m_artist->validate();
}

std::string ArtistForm::windowTitle()
{
    return "Artist Form";
}

void ArtistForm::populateEntityFields()
{
    m_artist->set_first_name(ui->edtFirstName->text().toStdString());
    m_artist->set_surname(ui->edtSurname->text().toStdString());
    m_artist->set_fullname(ui->edtFullname->text().toStdString());
    m_artist->set_notes(ui->edtNotes->toPlainText().toStdString());

    set_choice_field(m_artist->artist_type(), ui->cbArtistType);
}

void ArtistForm::populateFormWidgets()
{
    ui->edtFirstName->setText(stoq(m_artist->first_name()->value()));
    ui->edtSurname->setText(stoq(m_artist->surname()->value()));
    ui->edtFullname->setText(stoq(m_artist->fullName()->value()));

    populate_choice_combo(ui->cbArtistType, m_artist->artist_type());
}


void ArtistForm::populate_choice_combo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

    set_choice_field_default(cbox, cf->value());
}

void ArtistForm::set_choice_field_default(QComboBox* cbox, const std::string val)
{
    cbox->setCurrentIndex(cbox->findData(QVariant(stoq(val))));
}

void ArtistForm::artist_type_changed(int i)
{
    m_artist->artist_type()->setValue(
                ui->cbArtistType->itemData(i).toString().toStdString()
                );
}

void ArtistForm::update_fullname_first(const QString text)
{
    ui->edtFullname->setText(text+" "+ui->edtSurname->text());
}

void ArtistForm::update_fullname_last(const QString text)
{
    ui->edtFullname->setText(ui->edtFirstName->text()+" "+text);
}
