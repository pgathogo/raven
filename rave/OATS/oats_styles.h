#ifndef OATS_STYLES_H
#define OATS_STYLES_H

#include <QString>

namespace OATSTYLE {

    #define BTN_DEFAULT  QString("height:30;""width:100;""font-family: Roboto;""font-size:12px;""font:bold;" "color:#FFFFFF;")

    #define BTN_BG QString("background-color: #4682B4;")
    #define PLAY_SLOW_FLASH_BG QString("background-color: #00C853;")
    #define STOP_SLOW_FLASH QString("background-color: #FF0000;")

    #define FAST_FLASH_GREEN QString("background-color:#AAFF00;")
    #define FAST_FLASH_ORANGE QString("background-color:#FFA500;")
    #define FAST_FLASH_RED QString("background-color:#FF0000;")

    QString play_button_style{BTN_BG+BTN_DEFAULT};
    QString stop_button_style{BTN_BG+BTN_DEFAULT};

    QString play_button_slow_flash_style{PLAY_SLOW_FLASH_BG+BTN_DEFAULT};

    QString stop_button_slow_flash{STOP_SLOW_FLASH+BTN_DEFAULT};

    QString fast_flash_green{FAST_FLASH_GREEN+BTN_DEFAULT};
    QString fast_flash_orange{FAST_FLASH_ORANGE+BTN_DEFAULT};
    QString fast_flash_red{FAST_FLASH_RED+BTN_DEFAULT};


    QString scrollbar_style{
         "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
         "border: 2px solid grey;"
         "width: 3px;"
         "height: 3px;"
         "background: white;"
    };

    QString panel_style ="background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #4A5E72 , stop:1 #34424F );";

}

#endif
