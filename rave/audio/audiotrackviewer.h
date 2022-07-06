#pragma once

#include <memory>

#include <QWidget>
#include <QModelIndex>
#include <QSortFilterProxyModel>

#include "../framework/letterfilterwidget.h"

class QTableView;
class QStandardItemModel;
class QVBoxLayout;

namespace AUDIO
{

    class AudioTrackViewer : public QWidget
    {
        Q_OBJECT
    public:
        explicit AudioTrackViewer(QWidget *parent = nullptr);
        ~AudioTrackViewer();
        void clear();
        void create_track_view_headers();
        void set_track_view_column_width();
        QModelIndex current_index();
        int get_selected_audio_id();
        void adjust_header_size();
        QStandardItemModel* model();

    private:
        std::unique_ptr<QTableView> m_track_view;
        std::unique_ptr<QStandardItemModel> m_track_model;
        QVBoxLayout* m_main_layout;
        QStandardItemModel* track_model();
        QSortFilterProxyModel* filter_model();
        std::unique_ptr<LetterFilterWidget> m_letter_filter_widget;

        std::unique_ptr<QSortFilterProxyModel> m_filter_model;

    private slots:
        void tv_clicked(const QModelIndex& index);
        void filter_by_letter(QString letter);

    signals:
        void track_view_clicked(const QModelIndex&);
        void track_selected(int);

    };

}
