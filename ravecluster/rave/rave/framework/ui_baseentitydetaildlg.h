/********************************************************************************
** Form generated from reading UI file 'baseentitydetaildlg.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASEENTITYDETAILDLG_H
#define UI_BASEENTITYDETAILDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BaseEntityDetailDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *noticeLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lblTitle;
    QFrame *line;
    QWidget *baseContainer;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSave;
    QPushButton *btnSaveNew;
    QPushButton *btnClose;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *BaseEntityDetailDlg)
    {
        if (BaseEntityDetailDlg->objectName().isEmpty())
            BaseEntityDetailDlg->setObjectName(QString::fromUtf8("BaseEntityDetailDlg"));
        BaseEntityDetailDlg->resize(462, 313);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BaseEntityDetailDlg->sizePolicy().hasHeightForWidth());
        BaseEntityDetailDlg->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(BaseEntityDetailDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        noticeLayout = new QVBoxLayout();
        noticeLayout->setObjectName(QString::fromUtf8("noticeLayout"));
        noticeLayout->setSizeConstraint(QLayout::SetMaximumSize);

        verticalLayout_2->addLayout(noticeLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 10);
        lblTitle = new QLabel(BaseEntityDetailDlg);
        lblTitle->setObjectName(QString::fromUtf8("lblTitle"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        lblTitle->setFont(font);

        horizontalLayout->addWidget(lblTitle);


        verticalLayout_2->addLayout(horizontalLayout);

        line = new QFrame(BaseEntityDetailDlg);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        baseContainer = new QWidget(BaseEntityDetailDlg);
        baseContainer->setObjectName(QString::fromUtf8("baseContainer"));

        verticalLayout_2->addWidget(baseContainer);

        line_2 = new QFrame(BaseEntityDetailDlg);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 10, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        btnSave = new QPushButton(BaseEntityDetailDlg);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));

        horizontalLayout_3->addWidget(btnSave);

        btnSaveNew = new QPushButton(BaseEntityDetailDlg);
        btnSaveNew->setObjectName(QString::fromUtf8("btnSaveNew"));

        horizontalLayout_3->addWidget(btnSaveNew);

        btnClose = new QPushButton(BaseEntityDetailDlg);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        horizontalLayout_3->addWidget(btnClose);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(BaseEntityDetailDlg);

        QMetaObject::connectSlotsByName(BaseEntityDetailDlg);
    } // setupUi

    void retranslateUi(QDialog *BaseEntityDetailDlg)
    {
        BaseEntityDetailDlg->setWindowTitle(QCoreApplication::translate("BaseEntityDetailDlg", "Form", nullptr));
        lblTitle->setText(QCoreApplication::translate("BaseEntityDetailDlg", "TextLabel", nullptr));
        btnSave->setText(QCoreApplication::translate("BaseEntityDetailDlg", "Save", nullptr));
        btnSaveNew->setText(QCoreApplication::translate("BaseEntityDetailDlg", "Save && New", nullptr));
        btnClose->setText(QCoreApplication::translate("BaseEntityDetailDlg", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BaseEntityDetailDlg: public Ui_BaseEntityDetailDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASEENTITYDETAILDLG_H
