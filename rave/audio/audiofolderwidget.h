#ifndef AUDIOFOLDERWIDGET_H
#define AUDIOFOLDERWIDGET_H

#include <memory>
#include <vector>

#include <QTreeView>


//#include "../framework/treeviewmodel.h"

 namespace FRAMEWORK {
      class TreeViewModel;
 }

class QVBoxLayout;
//class QTreeView;
//class NodeData;

namespace AUDIO
{
    class AudioFolderWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioFolderWidget();
        ~AudioFolderWidget();
        void layout_controls();
        void set_model();

    private slots:
        void folder_clicked(const QModelIndex& index);

    signals:
        void folder_clicked_signal(int);

    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QTreeView> m_folder_view;

        //std::unique_ptr<NodeData*> m_folders;
        //std::vector<NodeData*> m_folders;

        std::unique_ptr<FRAMEWORK::TreeViewModel> m_folder_model;

        void style_folder_view();
    };
}

#endif
