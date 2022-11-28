/********************************************************************************
** Form generated from reading UI file 'audiowaveform.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOWAVEFORM_H
#define UI_AUDIOWAVEFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

namespace AUDIO {

class Ui_AudioWaveForm
{
public:
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QLabel *lblTitle;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_3;
    QLabel *lblArtist;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_2;
    QLabel *lblDuration;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_6;
    QLabel *lblCurrTime;
    QLineEdit *edtTest;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnPlay;
    QPushButton *btnPause;
    QPushButton *btnStop;
    QSpacerItem *horizontalSpacer_3;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_7;
    QLabel *lblStartMarkTime;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnMarkStartMarker;
    QPushButton *btnPlayStartMarker;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_10;
    QLabel *lblFadeInMarkTime;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnMarkFadeIn;
    QPushButton *btnPlayFadeIn;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_12;
    QLabel *lblIntroMarkTime;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *btnMarkIntro;
    QPushButton *btnPlayIntro;
    QSpacerItem *horizontalSpacer_6;
    QVBoxLayout *verticalLayout_11;
    QVBoxLayout *verticalLayout_12;
    QLabel *label_14;
    QLabel *lblFadeOutMarkTime;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnMarkFadeOut;
    QPushButton *btnPlayFadeOut;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout_13;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_16;
    QLabel *lblExtroMarkTime;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *btnMarkExtro;
    QPushButton *btnPlayExtro;
    QSpacerItem *horizontalSpacer_8;
    QVBoxLayout *verticalLayout_15;
    QVBoxLayout *verticalLayout_16;
    QLabel *label_18;
    QLabel *lblEndMarkTime;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *btnMarkEndMarker;
    QPushButton *btnPlayEndMarker;
    QSpacerItem *horizontalSpacer_2;
    QFrame *line;
    QHBoxLayout *hlWave;
    QGraphicsView *gvWave;
    QHBoxLayout *horizontalLayout_11;
    QLabel *lblFilepath;
    QLabel *lblFileDuration;
    QLabel *lblBitRate;
    QLabel *lblSampleRate;
    QLabel *lblFileSize;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *hlVumeter;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupUi(QDialog *AUDIO__AudioWaveForm)
    {
        if (AUDIO__AudioWaveForm->objectName().isEmpty())
            AUDIO__AudioWaveForm->setObjectName("AUDIO__AudioWaveForm");
        AUDIO__AudioWaveForm->resize(680, 480);
        AUDIO__AudioWaveForm->setMinimumSize(QSize(680, 480));
        AUDIO__AudioWaveForm->setMaximumSize(QSize(680, 480));
        verticalLayout_17 = new QVBoxLayout(AUDIO__AudioWaveForm);
        verticalLayout_17->setObjectName("verticalLayout_17");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        label = new QLabel(AUDIO__AudioWaveForm);
        label->setObjectName("label");

        verticalLayout_5->addWidget(label);

        lblTitle = new QLabel(AUDIO__AudioWaveForm);
        lblTitle->setObjectName("lblTitle");
        QFont font;
        font.setBold(true);
        lblTitle->setFont(font);
        lblTitle->setFrameShape(QFrame::Panel);
        lblTitle->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(lblTitle);


        horizontalLayout_4->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        label_3 = new QLabel(AUDIO__AudioWaveForm);
        label_3->setObjectName("label_3");

        verticalLayout_6->addWidget(label_3);

        lblArtist = new QLabel(AUDIO__AudioWaveForm);
        lblArtist->setObjectName("lblArtist");
        lblArtist->setFont(font);
        lblArtist->setFrameShape(QFrame::Panel);
        lblArtist->setFrameShadow(QFrame::Sunken);

        verticalLayout_6->addWidget(lblArtist);


        horizontalLayout_4->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName("verticalLayout_7");
        label_2 = new QLabel(AUDIO__AudioWaveForm);
        label_2->setObjectName("label_2");

        verticalLayout_7->addWidget(label_2);

        lblDuration = new QLabel(AUDIO__AudioWaveForm);
        lblDuration->setObjectName("lblDuration");
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        lblDuration->setFont(font1);
        lblDuration->setFrameShape(QFrame::Panel);
        lblDuration->setFrameShadow(QFrame::Raised);
        lblDuration->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lblDuration);


        horizontalLayout_4->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName("verticalLayout_8");
        label_6 = new QLabel(AUDIO__AudioWaveForm);
        label_6->setObjectName("label_6");

        verticalLayout_8->addWidget(label_6);

        lblCurrTime = new QLabel(AUDIO__AudioWaveForm);
        lblCurrTime->setObjectName("lblCurrTime");
        lblCurrTime->setFont(font1);
        lblCurrTime->setStyleSheet(QString::fromUtf8("color: rgb(255, 85, 0);"));
        lblCurrTime->setFrameShape(QFrame::Panel);
        lblCurrTime->setFrameShadow(QFrame::Raised);
        lblCurrTime->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lblCurrTime);


        horizontalLayout_4->addLayout(verticalLayout_8);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 1);

        verticalLayout_17->addLayout(horizontalLayout_4);

        edtTest = new QLineEdit(AUDIO__AudioWaveForm);
        edtTest->setObjectName("edtTest");

        verticalLayout_17->addWidget(edtTest);

        line_3 = new QFrame(AUDIO__AudioWaveForm);
        line_3->setObjectName("line_3");
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_17->addWidget(line_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        btnPlay = new QPushButton(AUDIO__AudioWaveForm);
        btnPlay->setObjectName("btnPlay");
        btnPlay->setMinimumSize(QSize(0, 0));
        btnPlay->setMaximumSize(QSize(40, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/audio/images/icons/play_green.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlay->setIcon(icon);

        horizontalLayout_5->addWidget(btnPlay);

        btnPause = new QPushButton(AUDIO__AudioWaveForm);
        btnPause->setObjectName("btnPause");
        btnPause->setMinimumSize(QSize(0, 0));
        btnPause->setMaximumSize(QSize(40, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/audio/images/icons/pause_play.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPause->setIcon(icon1);

        horizontalLayout_5->addWidget(btnPause);

        btnStop = new QPushButton(AUDIO__AudioWaveForm);
        btnStop->setObjectName("btnStop");
        btnStop->setMinimumSize(QSize(0, 0));
        btnStop->setMaximumSize(QSize(40, 16777215));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/audio/images/icons/stop_play.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnStop->setIcon(icon2);

        horizontalLayout_5->addWidget(btnStop);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_17->addLayout(horizontalLayout_5);

        line_2 = new QFrame(AUDIO__AudioWaveForm);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_17->addWidget(line_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(8);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        label_7 = new QLabel(AUDIO__AudioWaveForm);
        label_7->setObjectName("label_7");
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_7);

        lblStartMarkTime = new QLabel(AUDIO__AudioWaveForm);
        lblStartMarkTime->setObjectName("lblStartMarkTime");
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        lblStartMarkTime->setFont(font2);
        lblStartMarkTime->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);\n"
"color: rgb(0, 0, 0);"));
        lblStartMarkTime->setFrameShape(QFrame::Panel);
        lblStartMarkTime->setFrameShadow(QFrame::Sunken);
        lblStartMarkTime->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblStartMarkTime);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName("horizontalLayout");
        btnMarkStartMarker = new QPushButton(AUDIO__AudioWaveForm);
        btnMarkStartMarker->setObjectName("btnMarkStartMarker");
        btnMarkStartMarker->setMaximumSize(QSize(40, 16777215));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/audio/images/icons/mark_start.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMarkStartMarker->setIcon(icon3);

        horizontalLayout->addWidget(btnMarkStartMarker);

        btnPlayStartMarker = new QPushButton(AUDIO__AudioWaveForm);
        btnPlayStartMarker->setObjectName("btnPlayStartMarker");
        btnPlayStartMarker->setMaximumSize(QSize(40, 16777215));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/audio/images/icons/play_start.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlayStartMarker->setIcon(icon4);

        horizontalLayout->addWidget(btnPlayStartMarker);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout_2);

        horizontalSpacer_4 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        label_10 = new QLabel(AUDIO__AudioWaveForm);
        label_10->setObjectName("label_10");
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_10);

        lblFadeInMarkTime = new QLabel(AUDIO__AudioWaveForm);
        lblFadeInMarkTime->setObjectName("lblFadeInMarkTime");
        lblFadeInMarkTime->setFont(font2);
        lblFadeInMarkTime->setStyleSheet(QString::fromUtf8("background-color: rgb(213, 213, 0);\n"
"color: rgb(0, 0, 0);"));
        lblFadeInMarkTime->setFrameShape(QFrame::Panel);
        lblFadeInMarkTime->setFrameShadow(QFrame::Sunken);
        lblFadeInMarkTime->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(lblFadeInMarkTime);


        verticalLayout_3->addLayout(verticalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        btnMarkFadeIn = new QPushButton(AUDIO__AudioWaveForm);
        btnMarkFadeIn->setObjectName("btnMarkFadeIn");
        btnMarkFadeIn->setMaximumSize(QSize(40, 16777215));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/audio/images/icons/mark_fade_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMarkFadeIn->setIcon(icon5);

        horizontalLayout_2->addWidget(btnMarkFadeIn);

        btnPlayFadeIn = new QPushButton(AUDIO__AudioWaveForm);
        btnPlayFadeIn->setObjectName("btnPlayFadeIn");
        btnPlayFadeIn->setMaximumSize(QSize(40, 16777215));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/audio/images/icons/play_fade_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlayFadeIn->setIcon(icon6);

        horizontalLayout_2->addWidget(btnPlayFadeIn);


        verticalLayout_3->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_3);

        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName("verticalLayout_9");
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName("verticalLayout_10");
        label_12 = new QLabel(AUDIO__AudioWaveForm);
        label_12->setObjectName("label_12");
        label_12->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_12);

        lblIntroMarkTime = new QLabel(AUDIO__AudioWaveForm);
        lblIntroMarkTime->setObjectName("lblIntroMarkTime");
        lblIntroMarkTime->setFont(font2);
        lblIntroMarkTime->setStyleSheet(QString::fromUtf8("background-color: rgb(55, 55, 166);\n"
"color: rgb(255, 255, 255);"));
        lblIntroMarkTime->setFrameShape(QFrame::Panel);
        lblIntroMarkTime->setFrameShadow(QFrame::Sunken);
        lblIntroMarkTime->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(lblIntroMarkTime);


        verticalLayout_9->addLayout(verticalLayout_10);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(1);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        btnMarkIntro = new QPushButton(AUDIO__AudioWaveForm);
        btnMarkIntro->setObjectName("btnMarkIntro");
        btnMarkIntro->setMaximumSize(QSize(40, 16777215));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/audio/images/icons/mark_intro.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMarkIntro->setIcon(icon7);

        horizontalLayout_6->addWidget(btnMarkIntro);

        btnPlayIntro = new QPushButton(AUDIO__AudioWaveForm);
        btnPlayIntro->setObjectName("btnPlayIntro");
        btnPlayIntro->setMaximumSize(QSize(40, 16777215));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/audio/images/icons/play_intro.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlayIntro->setIcon(icon8);

        horizontalLayout_6->addWidget(btnPlayIntro);


        verticalLayout_9->addLayout(horizontalLayout_6);


        horizontalLayout_3->addLayout(verticalLayout_9);

        horizontalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName("verticalLayout_11");
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName("verticalLayout_12");
        label_14 = new QLabel(AUDIO__AudioWaveForm);
        label_14->setObjectName("label_14");
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(label_14);

        lblFadeOutMarkTime = new QLabel(AUDIO__AudioWaveForm);
        lblFadeOutMarkTime->setObjectName("lblFadeOutMarkTime");
        lblFadeOutMarkTime->setFont(font2);
        lblFadeOutMarkTime->setStyleSheet(QString::fromUtf8("background-color: rgb(127, 127, 0);\n"
"color: rgb(255, 255, 255);"));
        lblFadeOutMarkTime->setFrameShape(QFrame::Panel);
        lblFadeOutMarkTime->setFrameShadow(QFrame::Sunken);
        lblFadeOutMarkTime->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lblFadeOutMarkTime);


        verticalLayout_11->addLayout(verticalLayout_12);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(1);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        btnMarkFadeOut = new QPushButton(AUDIO__AudioWaveForm);
        btnMarkFadeOut->setObjectName("btnMarkFadeOut");
        btnMarkFadeOut->setMaximumSize(QSize(40, 16777215));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/audio/images/icons/mark_fade_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMarkFadeOut->setIcon(icon9);

        horizontalLayout_8->addWidget(btnMarkFadeOut);

        btnPlayFadeOut = new QPushButton(AUDIO__AudioWaveForm);
        btnPlayFadeOut->setObjectName("btnPlayFadeOut");
        btnPlayFadeOut->setMaximumSize(QSize(40, 16777215));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/audio/images/icons/play_fade_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlayFadeOut->setIcon(icon10);

        horizontalLayout_8->addWidget(btnPlayFadeOut);


        verticalLayout_11->addLayout(horizontalLayout_8);


        horizontalLayout_3->addLayout(verticalLayout_11);

        horizontalSpacer_7 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName("verticalLayout_13");
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setObjectName("verticalLayout_14");
        label_16 = new QLabel(AUDIO__AudioWaveForm);
        label_16->setObjectName("label_16");
        label_16->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_16);

        lblExtroMarkTime = new QLabel(AUDIO__AudioWaveForm);
        lblExtroMarkTime->setObjectName("lblExtroMarkTime");
        lblExtroMarkTime->setFont(font2);
        lblExtroMarkTime->setStyleSheet(QString::fromUtf8("background-color: rgb(88, 176, 130);\n"
"color: rgb(0,0,0);"));
        lblExtroMarkTime->setFrameShape(QFrame::Panel);
        lblExtroMarkTime->setFrameShadow(QFrame::Sunken);
        lblExtroMarkTime->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(lblExtroMarkTime);


        verticalLayout_13->addLayout(verticalLayout_14);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(1);
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        btnMarkExtro = new QPushButton(AUDIO__AudioWaveForm);
        btnMarkExtro->setObjectName("btnMarkExtro");
        btnMarkExtro->setMaximumSize(QSize(40, 16777215));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/audio/images/icons/mark_extro.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMarkExtro->setIcon(icon11);

        horizontalLayout_9->addWidget(btnMarkExtro);

        btnPlayExtro = new QPushButton(AUDIO__AudioWaveForm);
        btnPlayExtro->setObjectName("btnPlayExtro");
        btnPlayExtro->setMaximumSize(QSize(40, 16777215));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/audio/images/icons/play_extro.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlayExtro->setIcon(icon12);

        horizontalLayout_9->addWidget(btnPlayExtro);


        verticalLayout_13->addLayout(horizontalLayout_9);


        horizontalLayout_3->addLayout(verticalLayout_13);

        horizontalSpacer_8 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName("verticalLayout_15");
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setObjectName("verticalLayout_16");
        label_18 = new QLabel(AUDIO__AudioWaveForm);
        label_18->setObjectName("label_18");
        label_18->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(label_18);

        lblEndMarkTime = new QLabel(AUDIO__AudioWaveForm);
        lblEndMarkTime->setObjectName("lblEndMarkTime");
        lblEndMarkTime->setFont(font2);
        lblEndMarkTime->setStyleSheet(QString::fromUtf8("background-color: rgb(212, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        lblEndMarkTime->setFrameShape(QFrame::Panel);
        lblEndMarkTime->setFrameShadow(QFrame::Sunken);
        lblEndMarkTime->setAlignment(Qt::AlignCenter);

        verticalLayout_16->addWidget(lblEndMarkTime);


        verticalLayout_15->addLayout(verticalLayout_16);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(1);
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        btnMarkEndMarker = new QPushButton(AUDIO__AudioWaveForm);
        btnMarkEndMarker->setObjectName("btnMarkEndMarker");
        btnMarkEndMarker->setMaximumSize(QSize(40, 16777215));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/audio/images/icons/mark_end.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnMarkEndMarker->setIcon(icon13);

        horizontalLayout_10->addWidget(btnMarkEndMarker);

        btnPlayEndMarker = new QPushButton(AUDIO__AudioWaveForm);
        btnPlayEndMarker->setObjectName("btnPlayEndMarker");
        btnPlayEndMarker->setMaximumSize(QSize(40, 16777215));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/audio/images/icons/play_end.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlayEndMarker->setIcon(icon14);

        horizontalLayout_10->addWidget(btnPlayEndMarker);


        verticalLayout_15->addLayout(horizontalLayout_10);


        horizontalLayout_3->addLayout(verticalLayout_15);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_17->addLayout(horizontalLayout_3);

        line = new QFrame(AUDIO__AudioWaveForm);
        line->setObjectName("line");
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_17->addWidget(line);

        hlWave = new QHBoxLayout();
        hlWave->setObjectName("hlWave");
        gvWave = new QGraphicsView(AUDIO__AudioWaveForm);
        gvWave->setObjectName("gvWave");

        hlWave->addWidget(gvWave);


        verticalLayout_17->addLayout(hlWave);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        lblFilepath = new QLabel(AUDIO__AudioWaveForm);
        lblFilepath->setObjectName("lblFilepath");
        lblFilepath->setFrameShape(QFrame::Panel);
        lblFilepath->setFrameShadow(QFrame::Sunken);

        horizontalLayout_11->addWidget(lblFilepath);

        lblFileDuration = new QLabel(AUDIO__AudioWaveForm);
        lblFileDuration->setObjectName("lblFileDuration");
        lblFileDuration->setFrameShape(QFrame::Panel);
        lblFileDuration->setFrameShadow(QFrame::Sunken);

        horizontalLayout_11->addWidget(lblFileDuration);

        lblBitRate = new QLabel(AUDIO__AudioWaveForm);
        lblBitRate->setObjectName("lblBitRate");
        lblBitRate->setFrameShape(QFrame::Panel);
        lblBitRate->setFrameShadow(QFrame::Sunken);

        horizontalLayout_11->addWidget(lblBitRate);

        lblSampleRate = new QLabel(AUDIO__AudioWaveForm);
        lblSampleRate->setObjectName("lblSampleRate");
        lblSampleRate->setFrameShape(QFrame::Panel);
        lblSampleRate->setFrameShadow(QFrame::Sunken);

        horizontalLayout_11->addWidget(lblSampleRate);

        lblFileSize = new QLabel(AUDIO__AudioWaveForm);
        lblFileSize->setObjectName("lblFileSize");
        lblFileSize->setFrameShape(QFrame::Panel);
        lblFileSize->setFrameShadow(QFrame::Sunken);

        horizontalLayout_11->addWidget(lblFileSize);

        horizontalLayout_11->setStretch(0, 1);

        verticalLayout_17->addLayout(horizontalLayout_11);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        hlVumeter = new QHBoxLayout();
        hlVumeter->setObjectName("hlVumeter");

        horizontalLayout_7->addLayout(hlVumeter);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        btnSave = new QPushButton(AUDIO__AudioWaveForm);
        btnSave->setObjectName("btnSave");

        horizontalLayout_7->addWidget(btnSave);

        btnCancel = new QPushButton(AUDIO__AudioWaveForm);
        btnCancel->setObjectName("btnCancel");

        horizontalLayout_7->addWidget(btnCancel);


        verticalLayout_17->addLayout(horizontalLayout_7);


        retranslateUi(AUDIO__AudioWaveForm);

        QMetaObject::connectSlotsByName(AUDIO__AudioWaveForm);
    } // setupUi

    void retranslateUi(QDialog *AUDIO__AudioWaveForm)
    {
        AUDIO__AudioWaveForm->setWindowTitle(QCoreApplication::translate("AUDIO::AudioWaveForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Title", nullptr));
        lblTitle->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "TextLabel", nullptr));
        label_3->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Artist", nullptr));
        lblArtist->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "lblArtist", nullptr));
        label_2->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Duration", nullptr));
        lblDuration->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        label_6->setText(QString());
        lblCurrTime->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        btnPlay->setText(QString());
        btnPause->setText(QString());
        btnStop->setText(QString());
        label_7->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Start Marker", nullptr));
        lblStartMarkTime->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        btnMarkStartMarker->setText(QString());
        btnPlayStartMarker->setText(QString());
        label_10->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Fade In", nullptr));
        lblFadeInMarkTime->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        btnMarkFadeIn->setText(QString());
        btnPlayFadeIn->setText(QString());
        label_12->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Intro", nullptr));
        lblIntroMarkTime->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        btnMarkIntro->setText(QString());
        btnPlayIntro->setText(QString());
        label_14->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Fade Out", nullptr));
        lblFadeOutMarkTime->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        btnMarkFadeOut->setText(QString());
        btnPlayFadeOut->setText(QString());
        label_16->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Extro", nullptr));
        lblExtroMarkTime->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        btnMarkExtro->setText(QString());
        btnPlayExtro->setText(QString());
        label_18->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "End Marker", nullptr));
        lblEndMarkTime->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "00:00:00:00", nullptr));
        btnMarkEndMarker->setText(QString());
        btnPlayEndMarker->setText(QString());
        lblFilepath->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "path", nullptr));
        lblFileDuration->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "seconds", nullptr));
        lblBitRate->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "bitrate", nullptr));
        lblSampleRate->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "samplerate", nullptr));
        lblFileSize->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "filesize", nullptr));
        btnSave->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Save", nullptr));
        btnCancel->setText(QCoreApplication::translate("AUDIO::AudioWaveForm", "Cancel", nullptr));
    } // retranslateUi

};

} // namespace AUDIO

namespace AUDIO {
namespace Ui {
    class AudioWaveForm: public Ui_AudioWaveForm {};
} // namespace Ui
} // namespace AUDIO

#endif // UI_AUDIOWAVEFORM_H
