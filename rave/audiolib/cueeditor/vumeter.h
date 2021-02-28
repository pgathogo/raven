#ifndef VUMETER_H
#define VUMETER_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QRect>

namespace AUDIO{

    class VuMeter : public QWidget
    {
        Q_OBJECT
    public:
        enum meter_dir{DIR_HORIZONTAL=0, DIR_VERTICLE};

        explicit VuMeter(meter_dir dir, QWidget *parent = nullptr);
        ~VuMeter();
        void setPeak(int level);
        int minPeak() const { return m_minPeak; }
        int maxPeak() const { return m_maxPeak; }

        meter_dir current_dir() { return m_meter_dir; };

    signals:

    public slots:

    private:
        QPixmap ledoff;
        QPixmap ledon;
        QLabel* lblLedOff;
        QLabel* lblLedOn;
        QPushButton* btn;

        int m_maxPeak;
        int m_minPeak;

        QRect baseRect;
        meter_dir m_meter_dir;
    };

} // AUDIO

#endif // VUMETER_H
