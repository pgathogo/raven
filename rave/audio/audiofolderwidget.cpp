#include <QVBoxLayout>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QScrollBar>

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
        QString tree_style (R"(
            QTreeView{
             background-color: #374148;
             font-size: 16px;
            }
            QTreeView::item {
             color: #FFFFFF;
            }
            QTreeView::branch:has-children:!has-siblings:closed,
            QTreeView::branch:closed:has-children:has-siblings {
              border-image: none;
              image: url(:/images/icons/treeview_closed_purple.png);
            }
            QTreeView::branch:open:has-children:!has-siblings,
            QTreeView::branch:open:has-children:has-siblings  {
                border-image: none;
                image: url(:/images/icons/treeview_open_purple.png);
            }
            QTreeView::item:selected:!active {
             background-color: #ffd166; /* Faded highlight when window is out of focus */
             color: #121A14;
             font-weigh: bold;
             }

            QHeaderView::section {
              background-color:#708090;
              color:#FFFFFF;
              font-weight: bold;
            }

            )"
            );

        const QString scrollBarStyle = (R"(
            QScrollBar:vertical {
                background: #34424F;
                width: 10px;
                margin: 0px;
                border-radius: 0px;  /*7px;*/
            }

            QScrollBar::handle:vertical {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 #7cc4e8, stop:0.5 #2f9bd6, stop:1 #1e739f);
                min-height: 30px;
                border-radius: 0px; /*7px;*/
            }

            QScrollBar::handle:vertical:hover {
                background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                    stop:0 #a8d9f2, stop:0.5 #5cb5e4, stop:1 #2f9bd6);
            }

            QScrollBar::handle:vertical:pressed {
                background: #247ba8;
            }

            QScrollBar::add-line:vertical,
                QScrollBar::sub-line:vertical {
                height: 0px;
            }

            QScrollBar::add-page:vertical,
                QScrollBar::sub-page:vertical {
                background: none;
            }
        )");



        m_folder_view->setStyleSheet(tree_style);

        m_folder_view->verticalScrollBar()->setStyleSheet(scrollBarStyle);

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
