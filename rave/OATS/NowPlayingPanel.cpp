#include "NowPlayingPanel.h"
#include "PanelTheme.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include "togglebutton.h"

namespace {
const char *kButtonStyle =
    "QPushButton { color:#e6edf5; font-size:12px; font-weight:bold;"
    "  padding:12px 20px; border:1px solid #3a4a5e; border-radius:8px;"
    "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #3b485b, stop:1 #252e3c); }"
    "QPushButton:hover { border-color:#22d3ee; color:#ffffff;"
    "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #46566d, stop:1 #2b3646); }"
    "QPushButton:pressed { background:#1b2330; border-color:#39ff6a; }";
}

NowPlayingPanel::NowPlayingPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 5);

    auto *card = new QFrame;
    card->setObjectName("nowCard");
    card->setStyleSheet(PanelTheme::cardStyle("QFrame#nowCard", "#2f9bd6"));
    root->addWidget(card);

    m_modeLabel = new QLabel;
    m_modeLabel->setStyleSheet(
        "background:transparent; color:#22d3ee; font-size:12px; font-weight:bold;");

    m_titleLabel = new QLabel;
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "background:transparent; color:#f4f8ff; font-size:26px; font-weight:500;");

    auto *goBtn   = new QPushButton(tr("Go Current"));

    //auto *keepBtn = new QPushButton(tr("Keep Current"));

    QString box_color = "#ff3b5c";
    auto* box = new QGroupBox("Keep Current: ");
    box->setObjectName("keepBox");
    box->setStyleSheet(
        PanelTheme::cardStyle("QGroupBox#keepBox", box_color) +
        QString("QGroupBox#keepBox { margin-top: 2px; padding-top: 2px;"
                "  font-size: 10px; font-weight: bold; color: %1; }"
                "QGroupBox#keepBox::title { subcontrol-origin: margin;"
                "  subcontrol-position: top left; left: 14px; padding: 0 6px; }")
            .arg(box_color));
    box->setMinimumWidth(100);

    auto* hl_keep = new QHBoxLayout(box);
    // QLabel* lblKeep = new QLabel("Keep Current");
    // lblKeep->setStyleSheet("color: #39ff6a; font-size: 8px; font-weight: bold;");
    // hl_keep->addWidget(lblKeep);

    ToggleButton* tog_keep_current = new ToggleButton(12, 10, this);
    tog_keep_current->setContentsMargins(0, 5, 0, 0);
    //QColor("#00e5ff");
    tog_keep_current->setTrackColors(QColor("#4caf50"), QColor("#9e9e9e"));
    tog_keep_current->setThumbColors(QColor("#ffffff"), QColor("#f5f5f5"));

    tog_keep_current->setGlowColor(QColor("#39ff6a"));
    tog_keep_current->setGlowRadius(12);
    tog_keep_current->setGlowOnlyWhenChecked(true);
    tog_keep_current->setStateTexts("On", "Off");
    tog_keep_current->setStateTextVisible(true);

    hl_keep->addWidget(tog_keep_current);


    goBtn->setStyleSheet(kButtonStyle);
    // keepBtn->setStyleSheet(kButtonStyle);

    goBtn->setCursor(Qt::PointingHandCursor);
    // keepBtn->setCursor(Qt::PointingHandCursor);

    connect(goBtn,   &QPushButton::clicked, this, &NowPlayingPanel::goCurrentClicked);
    // connect(keepBtn, &QPushButton::clicked, this, &NowPlayingPanel::keepCurrentClicked);

    auto *lay = new QHBoxLayout(card);
    //lay->setContentsMargins(18, 14, 18, 14);
    lay->setContentsMargins(5, 5, 5 , 5);
    lay->setSpacing(10);
    lay->addWidget(m_modeLabel, 0, Qt::AlignTop);
    lay->addWidget(m_titleLabel, 1);
    lay->addWidget(goBtn);

    lay->addWidget(box);

    // lay->addWidget(keepBtn);

    setPlayMode(tr("Auto"));
    setTitle(tr("No audio playing"));
}

void NowPlayingPanel::setTitle(const QString &title)    { m_titleLabel->setText(title); }
void NowPlayingPanel::setPlayMode(const QString &mode)  { m_modeLabel->setText(tr("Play Mode: ") + mode); }
