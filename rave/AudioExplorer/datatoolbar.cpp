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
