#include <QTableView>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QRegularExpression>
#include <QDebug>

#include "audiotrackviewer.h"
#include "../framework/letterfilterwidget.h"

namespace AUDIO
{

    AudioTrackViewer::AudioTrackViewer(QWidget *parent)
        : QWidget(parent)
        , m_track_view{nullptr}
        , m_track_model{nullptr}
    {
        m_track_view = std::make_unique<QTableView>(this);
        m_track_view->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_track_model = std::make_unique<QStandardItemModel>(this);
        m_track_view->setAlternatingRowColors(true);

        m_filter_model = std::make_unique<QSortFilterProxyModel>(this);
        m_filter_model->setSourceModel(m_track_model.get());

        m_track_view->setModel(m_filter_model.get());

        m_track_view->horizontalHeader()->setStretchLastSection(true);
        m_track_view->verticalHeader()->setVisible(false);
        m_track_view->setSortingEnabled(true);

        connect(m_track_view.get(), &QTableView::clicked, this, &AudioTrackViewer::tv_clicked);

        m_letter_filter_widget = std::make_unique<LetterFilterWidget>(this);
        connect(m_letter_filter_widget.get(), &LetterFilterWidget::tab_letter, this, &AudioTrackViewer::filter_by_letter);

        m_main_layout = new QVBoxLayout();
        m_main_layout->setContentsMargins(0,0,0,0);
        m_main_layout->setSpacing(0);
        m_main_layout->addWidget(m_track_view.get());
        m_main_layout->addWidget(m_letter_filter_widget.get());

        create_track_view_headers();
        set_viewer_style();

        setLayout(m_main_layout);
    }

    AudioTrackViewer::~AudioTrackViewer()
    {
    }

    void AudioTrackViewer::clear()
    {
        m_track_model->clear();
    }

    void AudioTrackViewer::create_track_view_headers()
    {
        m_track_model->setHorizontalHeaderItem(0, new QStandardItem("Track Title"));
        m_track_model->setHorizontalHeaderItem(1, new QStandardItem("Artist"));
        m_track_model->setHorizontalHeaderItem(2, new QStandardItem("Duration"));
        m_track_model->setHorizontalHeaderItem(3, new QStandardItem("Audio Type"));
        m_track_model->setHorizontalHeaderItem(4, new QStandardItem("Audio File"));
        m_track_model->setHorizontalHeaderItem(5, new QStandardItem("Folder"));
        m_track_model->setHorizontalHeaderItem(6, new QStandardItem("Extension"));

        set_track_view_column_width();
    }

    void AudioTrackViewer::set_viewer_style()
    {
        m_track_view->setStyleSheet("QTableView{background-color: #34424F;} "
                                    "QTableView::item{background-color: #323A3F; color: #FFFFFF;}"
                                    "QTableView::item:alternate{background-color:#3C4449;}"
                                    "QTableView::item:selected{background-color: #800000;}"
                                    "QHeaderView::section{background-color:#708090; color:#FFFFFF;}");

    }

    QStandardItemModel* AudioTrackViewer::track_model()
    {
        return m_track_model.get();
    }

    void AudioTrackViewer::set_track_view_column_width()
    {
        enum Column{Title, Artist, Duration, AudioType, AudioFile, Folder, FileExtension};

        m_track_view->setColumnWidth(Column::Title, 300);
        m_track_view->setColumnWidth(Column::Artist,250);
        m_track_view->setColumnWidth(Column::Duration, 100);
        m_track_view->setColumnWidth(Column::AudioType, 150);
        m_track_view->setColumnWidth(Column::AudioFile, 250);
        m_track_view->setColumnWidth(Column::Folder, 250);
        m_track_view->setColumnWidth(Column::FileExtension, 250);
    }

    void AudioTrackViewer::tv_clicked(const QModelIndex& index)
    {
        auto indexes = m_track_view->selectionModel()->selectedRows();

        if (indexes.size() == 0)
            return;

        auto current_index = indexes[0];
        int id = current_index.data(Qt::UserRole).toInt();

        emit track_selected(id);

        /*
        auto first_column = m_track_view->model()->index(index.row(), 0);
        auto audio_id = first_column.data(Qt::UserRole).toInt();
        emit track_selected(audio_id);
       */

    }

    QModelIndex AudioTrackViewer::current_index()
    {
        return m_track_view->currentIndex();
    }

    int AudioTrackViewer::get_selected_audio_id()
    {
        if (m_track_view->selectionModel()->selectedIndexes().count() == 0)
            return -1;

        auto model_index = current_index();
        auto first_column = m_track_view->model()->index(model_index.row(), 0);
        auto audio_id = first_column.data(Qt::UserRole).toInt();

        return audio_id;

    }

    QStandardItemModel* AudioTrackViewer::model()
    {
        return m_track_model.get();
    }

    QSortFilterProxyModel* AudioTrackViewer::filter_model()
    {
        return m_filter_model.get();
    }

    void AudioTrackViewer::filter_by_letter(QString letter)
    {
        QString pattern = "";
        if (letter != "*")
             pattern = "^["+letter+"].*";

        m_filter_model->setFilterRegularExpression(QRegularExpression(pattern));
        m_filter_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
        m_filter_model->setFilterKeyColumn(0);
    }

}
