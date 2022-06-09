#include <QPushButton>
#include <QHBoxLayout>
#include "vumeter.h"

#include <QDebug>

namespace AUDIO {

    VuMeter::VuMeter(meter_dir dir, QWidget *parent)
     : QWidget(parent),
        ledoff{},
        ledon{},
        lblLedOff{nullptr},
        lblLedOn{nullptr},
        m_maxPeak{0},
        baseRect{},
        m_meter_dir{dir}
    {
        lblLedOff = new QLabel(this);
        lblLedOn = new QLabel(lblLedOff);

        if (dir == meter_dir::DIR_HORIZONTAL){
            //ledoff.load("D:/home/Lab/qtcreator/FFmpeg/FFmpegDemo/images/ledoff_thin_lr.bmp");
            //ledon.load("D:/home/Lab/qtcreator/FFmpeg/FFmpegDemo/images/ledon_thin_lr.bmp");


            //ledoff.load("D:/home/PMS/Raven/rave/images/icons/ledoff.bmp");
            ledoff.load(":/audio/images/icons/ledoff.bmp");
            ledon.load(":/audio/images/icons/ledon.bmp");
            m_maxPeak = lblLedOff->width();
        }

        if (dir == meter_dir::DIR_VERTICLE){
            ledoff.load(":/images/icon/ledoff_vert.bmp");
            ledon.load(":/images/icon/ledon_vert.bmp");
            m_maxPeak = lblLedOff->height();
        }
            baseRect = lblLedOff->frameRect();
            lblLedOff->setPixmap(ledoff);
            lblLedOn->setPixmap(ledon);


        m_box_layout = new QHBoxLayout(this);
        m_box_layout->addWidget(lblLedOff);

    }

    VuMeter::~VuMeter()
    {
        //delete lblLedOff;
        delete lblLedOn;
        delete m_box_layout;
    }

    void VuMeter::setPeak(int peak)
    {
        if (m_meter_dir == meter_dir::DIR_HORIZONTAL){
            baseRect.setWidth(peak);
        } else {
            baseRect.setHeight(peak);
        }

        lblLedOn->setFrameRect(baseRect);

    }

} // AUDIO
