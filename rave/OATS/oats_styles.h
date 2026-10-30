#ifndef OATS_STYLES_H
#define OATS_STYLES_H

#include <QString>

namespace OATSTYLE {

    #define BTN_DEFAULT  QString("height:30;""width:100;""font-family: Roboto;""font-size:12px;""font:bold;" "color:#FFFFFF;")
    #define BTN_BG QString("background-color: #4682B4;")

    #define PLAY_SLOW_FLASH_BG QString("background-color: #00C853;")
    #define STOP_SLOW_FLASH QString("background-color: #FF3B5C;")

    #define FAST_FLASH_GREEN QString("background-color:#AAFF00;")
    #define FAST_FLASH_ORANGE QString("background-color:#FFA500;")
    #define FAST_FLASH_RED QString("background-color:#FF0000;")


    //QPushButton {background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #374148 );
    QString btn_style(R"(
        QPushButton{
          background-color: #4682B4;
          border-radius: 20px;
          border-style: inset;
          border-bottom-width: 1px;
          border-radius: 3px;
          border-bottom-color:#374148;
          color:#FFFFFF;
          font-weight:bold;
          height: 40px;
          width: 100px;
        }

        QPushButton:hover{
            background-color:#34424F;
            border-width:1px;
            border-color:#0479B0;
       }

       QPushButton:pressed {
        background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #555D64 );
        border-radius: 20px;
        border-style: outset;
        border-bottom-width: 1px;
        border-radius: 3px;
        border-color:#34424F;
        color:#FFFFFF;
        font-weight:normal;
      }
      )");

    QString btn_play_slow_flash_style(R"(
        QPushButton{
          background-color: #00C853;
          border-radius: 20px;
          border-style: inset;
          border-bottom-width: 1px;
          border-radius: 3px;
          border-bottom-color:#374148;
          color:#FFFFFF;
          font-weight:bold;
          height: 40px;
          width: 100px;
        }
      )");

    QString btn_stop_slow_flash_style(R"(
      QPushButton{
        background-color: #FF0000;
        border-radius: 20px;
        border-style: inset;
        border-bottom-width: 1px;
        border-radius: 3px;
        border-bottom-color:#374148;
        color:#FFFFFF;
        font-weight:bold;
        height: 40px;
        width: 100px;
        }
      )");

    QString btn_fast_flash_green_style(R"(
     QPushButton{
        background-color: #AAFF00;
        border-radius: 20px;
        border-style: inset;
        border-bottom-width: 1px;
        border-radius: 3px;
        border-bottom-color:#374148;
        color:#FFFFFF;
        font-weight:bold;
        height: 40px;
        width: 100px;
        }
      )");

    QString btn_fast_flash_orange_style(R"(
      QPushButton{
        background-color: #AAFF00;
        border-radius: 20px;
        border-style: inset;
        border-bottom-width: 1px;
        border-radius: 3px;
        border-bottom-color:#374148;
        color:#FFFFFF;
        font-weight:bold;
        height: 40px;
        width: 100px;
      }
      )");

    QString btn_fast_flash_red_style(R"(
      QPushButton{
        background-color: #FF3B5C;
        border-radius: 20px;
        border-style: inset;
        border-bottom-width: 1px;
        border-radius: 3px;
        border-bottom-color:#374148;
        color:#FFFFFF;
        font-weight:bold;
        height: 40px;
        width: 100px;
      }
      )");

    QString play_button_style{btn_style};
    QString stop_button_style{btn_style};
    QString fade_button_style{btn_style};
    QString pause_button_style{btn_style};

    //QString play_button_style{BTN_BG+BTN_DEFAULT};

    //QString play_button_slow_flash_style{PLAY_SLOW_FLASH_BG+BTN_DEFAULT};
    //QString stop_button_slow_flash{STOP_SLOW_FLASH+BTN_DEFAULT};
    //QString fast_flash_green{FAST_FLASH_GREEN+BTN_DEFAULT};
    //QString fast_flash_orange{FAST_FLASH_ORANGE+BTN_DEFAULT};
    //QString fast_flash_red{FAST_FLASH_RED+BTN_DEFAULT};

    QString play_button_slow_flash_style{btn_play_slow_flash_style};
    QString stop_button_slow_flash{btn_stop_slow_flash_style};
    QString fast_flash_green{btn_fast_flash_green_style};
    QString fast_flash_orange{btn_fast_flash_orange_style};
    QString fast_flash_red{btn_fast_flash_red_style};

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
