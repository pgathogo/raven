#include <QVBoxLayout>
#include <QStandardItem>
#include <QStandardItemModel>

#include "audiofolderwidget.h"

#include "../framework/tree.h"
#include "../audio/audiolibrary.h"

#include "../framework/ravenexception.h"
#include "../framework/treeviewmodel.h"

namespace AUDIO
{

    AudioFolderWidget::AudioFolderWidget()
        :m_v_layout{nullptr}
        ,m_folder_view{nullptr}
    {
        m_v_layout = std::make_unique<QVBoxLayout>();

        //layout_controls();

        m_folder_view = std::make_unique<QTreeView>();
        m_folder_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        connect(m_folder_view.get(), &QTreeView::clicked, this, &AudioFolderWidget::folder_clicked);

        m_v_layout->addWidget(m_folder_view.get());

        setLayout(m_v_layout.get());
        set_model();

        style_folder_view();
    }

    AudioFolderWidget::~AudioFolderWidget()
    {}

    void AudioFolderWidget::style_folder_view()
    {
        QString tree_style (
            "QTreeView{background-color: #374148;}"
            "QTreeView::item{color: #FFFFFF;}"
            "QTreeView::branch:has-children:!has-siblings:closed,"
            "QTreeView::branch:closed:has-children:has-siblings {"
            "  border-image: none;"
            "  image: url(:/images/icons/treeview_closed_purple.png);"
            "}"
            "QTreeView::branch:open:has-children:!has-siblings,"
            "QTreeView::branch:open:has-children:has-siblings  {"
            "border-image: none;"
            "image: url(:/images/icons/treeview_open_purple.png);}"
            "QHeaderView::section{background-color:#708090; color:#FFFFFF;}"
            );


        m_folder_view->setStyleSheet(tree_style);

    }

    void AudioFolderWidget::layout_controls()
    {
        m_folder_view = std::make_unique<QTreeView>();
        m_folder_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
        connect(m_folder_view.get(), &QTreeView::clicked, this, &AudioFolderWidget::folder_clicked);

        m_v_layout->addWidget(m_folder_view.get());
    }

    void AudioFolderWidget::set_model()
    {

        try{
            AudioLibrary audio_lib;

            auto folders = audio_lib.read_data_from_db();

            m_folder_model = std::make_unique<FRAMEWORK::TreeViewModel>(folders, this);
            m_folder_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));

            m_folder_view->setModel(m_folder_model.get());

        } catch (DatabaseException& de) {
            qDebug() << QString::fromStdString(de.errorMessage());
        }

    }

    void AudioFolderWidget::folder_clicked(const QModelIndex& index)
    {
        int folder_id = index.data(Qt::UserRole).toInt();
        emit folder_clicked_signal(folder_id);
    }


}
