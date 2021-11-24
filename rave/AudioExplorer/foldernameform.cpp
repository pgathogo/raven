#include "foldernameform.h"
#include "ui_foldernameform.h"

FolderNameForm::FolderNameForm(QString folder_name, QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::FolderNameForm)
    ,m_folder_name{folder_name}
{
    ui->setupUi(this);

    connect(ui->btnCancel, &QPushButton::clicked, this, &FolderNameForm::close_form);
    connect(ui->btnSave, &QPushButton::clicked, this, &FolderNameForm::save);

    if (folder_name.isEmpty()){
        setWindowTitle("Create New Folder");
    }else{
        ui->edtFolderName->setText(folder_name);
        setWindowTitle("Rename Folder");
    }

}

FolderNameForm::~FolderNameForm()
{
    delete ui;
}

void FolderNameForm::close_form()
{
    done(0);
}

void FolderNameForm::save()
{
    m_folder_name = ui->edtFolderName->text();
    done(1);
}

QString FolderNameForm::folder_name()
{
    return m_folder_name;
}
