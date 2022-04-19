#include <QPushButton>
#include <QHBoxLayout>

#include "datatoolbar.h"

DataToolBar::DataToolBar(QWidget *parent) : QWidget(parent)
{
    m_btnAdd = new QPushButton("Add");
    m_btnEdit = new QPushButton("Edit");
    m_btnDelete = new QPushButton("Delete");

    m_btn_layout = new QHBoxLayout(this);
    m_btn_layout->addWidget(m_btnAdd);
    m_btn_layout->addWidget(m_btnEdit);
    m_btn_layout->addWidget(m_btnDelete);

    m_btn_layout->addStretch();

    set_button_icons();
}

void DataToolBar::set_button_icons()
{
    QPixmap add_pixmap(":/images/media/icons/add.png");
    QIcon add_icon(add_pixmap);
    m_btnAdd->setIcon(add_icon);

    QPixmap edit_pixmap(":/images/media/icons/edit.png");
    QIcon edit_icon(edit_pixmap);
    m_btnEdit->setIcon(edit_icon);

    QPixmap delete_pixmap(":/images/media/icons/delete.png");
    QIcon delete_icon(delete_pixmap);
    m_btnDelete->setIcon(delete_icon);
}

DataToolBar::~DataToolBar()
{
    delete m_btnAdd;
    delete m_btnEdit;
    delete m_btnDelete;
    delete m_btn_layout;
}

QPushButton* DataToolBar::add_button()
{
    return m_btnAdd;
}

QPushButton* DataToolBar::edit_button()
{
    return m_btnEdit;
}

QPushButton* DataToolBar::delete_button()
{
    return m_btnDelete;
}
