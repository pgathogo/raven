#pragma once

#include <QDialog>
#include "../framework/entitydatamodel.h"

namespace Ui {
class BookedAudioCheckerForm;
}

struct DetailData{
    int audio_id;
    std::string audio_title;
    std::string audio_type;
    std::string book_date;
    std::string book_time;
};

class BookedAudioCheckerForm : public QDialog
{
    Q_OBJECT

public:
    explicit BookedAudioCheckerForm(std::map<int, std::tuple<std::string, bool>>& selected_audios, QWidget* parent = nullptr);
    ~BookedAudioCheckerForm();

    void find_booked_audio(std::map<int, std::tuple<std::string, bool>>&);
    void gather_data();
    void show_booking_summary();
    void show_zero_booking(std::map<int, std::tuple<std::string, bool>>&);

private slots:
    void save();
    void cancel();

private:
    Ui::BookedAudioCheckerForm *ui;
    EntityDataModel m_edm;
    std::map<int,std::tuple<std::string, int>> m_summary_data;
    std::vector<DetailData> m_detail_data;
};

