#pragma once

#include <QDialog>

namespace Ui {
class FolderNameForm;
}

class FolderNameForm : public QDialog
{
    Q_OBJECT

public:
    explicit FolderNameForm(QString folder_name="", QWidget *parent = nullptr);
    ~FolderNameForm();

    QString folder_name();

private slots:
    void close_form();
    void save();


private:
    Ui::FolderNameForm *ui;
    QString m_folder_name;
};

