/********************************************************************************
** Form generated from reading UI file 'baseentitybrowserdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASEENTITYBROWSERDLG_H
#define UI_BASEENTITYBROWSERDLG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BaseEntityBrowserDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAdd;
    QPushButton *btnEdit;
    QPushButton *btnDelete;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *hlExtra;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *vlGrid;
    QTableView *tvEntity;
    QVBoxLayout *vlFilterTabs;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *edtFilter;
    QPushButton *btnSearch;
    QComboBox *cbFilter;
    QHBoxLayout *hlExtend;

    void setupUi(QDialog *BaseEntityBrowserDlg)
    {
        if (BaseEntityBrowserDlg->objectName().isEmpty())
            BaseEntityBrowserDlg->setObjectName(QString::fromUtf8("BaseEntityBrowserDlg"));
        BaseEntityBrowserDlg->resize(530, 350);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BaseEntityBrowserDlg->sizePolicy().hasHeightForWidth());
        BaseEntityBrowserDlg->setSizePolicy(sizePolicy);
        BaseEntityBrowserDlg->setMinimumSize(QSize(530, 350));
        verticalLayout_2 = new QVBoxLayout(BaseEntityBrowserDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnAdd = new QPushButton(BaseEntityBrowserDlg);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAdd->setIcon(icon);

        horizontalLayout->addWidget(btnAdd);

        btnEdit = new QPushButton(BaseEntityBrowserDlg);
        btnEdit->setObjectName(QString::fromUtf8("btnEdit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/icons/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnEdit->setIcon(icon1);

        horizontalLayout->addWidget(btnEdit);

        btnDelete = new QPushButton(BaseEntityBrowserDlg);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/icons/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDelete->setIcon(icon2);

        horizontalLayout->addWidget(btnDelete);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        hlExtra = new QHBoxLayout();
        hlExtra->setObjectName(QString::fromUtf8("hlExtra"));

        horizontalLayout->addLayout(hlExtra);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        vlGrid = new QVBoxLayout();
        vlGrid->setSpacing(0);
        vlGrid->setObjectName(QString::fromUtf8("vlGrid"));
        tvEntity = new QTableView(BaseEntityBrowserDlg);
        tvEntity->setObjectName(QString::fromUtf8("tvEntity"));
        tvEntity->setSelectionMode(QAbstractItemView::SingleSelection);
        tvEntity->setSelectionBehavior(QAbstractItemView::SelectRows);

        vlGrid->addWidget(tvEntity);

        vlFilterTabs = new QVBoxLayout();
        vlFilterTabs->setSpacing(0);
        vlFilterTabs->setObjectName(QString::fromUtf8("vlFilterTabs"));
        vlFilterTabs->setContentsMargins(-1, -1, -1, 0);

        vlGrid->addLayout(vlFilterTabs);


        verticalLayout_2->addLayout(vlGrid);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(BaseEntityBrowserDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        edtFilter = new QLineEdit(BaseEntityBrowserDlg);
        edtFilter->setObjectName(QString::fromUtf8("edtFilter"));

        gridLayout->addWidget(edtFilter, 0, 2, 1, 1);

        btnSearch = new QPushButton(BaseEntityBrowserDlg);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/icons/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSearch->setIcon(icon3);

        gridLayout->addWidget(btnSearch, 0, 3, 1, 1);

        cbFilter = new QComboBox(BaseEntityBrowserDlg);
        cbFilter->setObjectName(QString::fromUtf8("cbFilter"));

        gridLayout->addWidget(cbFilter, 0, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        hlExtend = new QHBoxLayout();
        hlExtend->setObjectName(QString::fromUtf8("hlExtend"));

        verticalLayout_2->addLayout(hlExtend);


        retranslateUi(BaseEntityBrowserDlg);

        QMetaObject::connectSlotsByName(BaseEntityBrowserDlg);
    } // setupUi

    void retranslateUi(QDialog *BaseEntityBrowserDlg)
    {
        BaseEntityBrowserDlg->setWindowTitle(QCoreApplication::translate("BaseEntityBrowserDlg", "Dialog", nullptr));
        btnAdd->setText(QCoreApplication::translate("BaseEntityBrowserDlg", "Add", nullptr));
        btnEdit->setText(QCoreApplication::translate("BaseEntityBrowserDlg", "Edit", nullptr));
        btnDelete->setText(QCoreApplication::translate("BaseEntityBrowserDlg", "Delete", nullptr));
        label->setText(QCoreApplication::translate("BaseEntityBrowserDlg", "Search By:", nullptr));
        btnSearch->setText(QCoreApplication::translate("BaseEntityBrowserDlg", "Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BaseEntityBrowserDlg: public Ui_BaseEntityBrowserDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASEENTITYBROWSERDLG_H
