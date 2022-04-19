#pragma once

#include <QWidget>

class QHBoxLayout;
class QPushButton;

class DataToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit DataToolBar(QWidget *parent = nullptr);
    ~DataToolBar();
    QPushButton* add_button();
    QPushButton* edit_button();
    QPushButton* delete_button();
    void set_button_icons();
private:
    QHBoxLayout* m_btn_layout;
    QPushButton* m_btnAdd;
    QPushButton* m_btnEdit;
    QPushButton* m_btnDelete;
};

