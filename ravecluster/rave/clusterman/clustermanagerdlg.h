#ifndef CLUSTERMANAGERDLG_H
#define CLUSTERMANAGERDLG_H

#include <variant>

#include <QDialog>
#include <QTreeWidgetItem>
#include <QUuid>
#include <QMap>
#include <QVariant>

#include "confignode.h"

namespace Ui {
class ClusterManagerDlg;
}

namespace ClusterManager{
    class Cluster;
    class Station;
    class Server;
    class StorageDisk;
    class AudioFolder;
}

class QMdiArea;

using RootNode = ClusterManager::ConfigNode<ClusterManager::RootConfigNode>;
using ClusterNode = ClusterManager::ConfigNode<ClusterManager::Cluster>;
using StationNode = ClusterManager::ConfigNode<ClusterManager::Station>;
using ServerNode = ClusterManager::ConfigNode<ClusterManager::Server>;
using DiskNode = ClusterManager::ConfigNode<ClusterManager::StorageDisk>;
using AudioFolderNode = ClusterManager::ConfigNode<ClusterManager::AudioFolder>;


using VarNodes =  std::map<QString, std::variant<RootNode*, ClusterNode*, StationNode*, ServerNode*, DiskNode*, AudioFolderNode*>>;
struct ClusterConfiguration {
    static int temp_id;
    int next_temp_id();
    VarNodes cluster_nodes;
};


class ClusterManagerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ClusterManagerDlg(QWidget *parent = nullptr);
    ~ClusterManagerDlg();

    void setMdiArea(QMdiArea* mdi);
    void show_cluster_context_menu(QString, QPoint);
    void show_root_context_menu(QString, QPoint);
    void show_audio_server_context_menu(QString, QPoint);
    void show_disk_context_menu(QString, QPoint);
    void show_folder_context_menu(QString, QPoint);
    void tree_printer(std::map<QString, std::tuple<ConfigItemType, std::any>>);
    void save_tree(std::map<QString, std::tuple<ConfigItemType, std::any>>, int);

    template<typename T1, typename T2, typename T3>
    void make_node(T1 node_entity, T3* parent_node, ClusterManager::ConfigItem config_item){

        auto node = new T2(config_item);

        node->set_node_entity(std::move(node_entity));

        auto uuid = QUuid().createUuid();
        QString uuid_str =  uuid.toString(QUuid::WithoutBraces).left(8);

        QMap<QString, QVariant> item_data;
        item_data["type"] = static_cast<int>(config_item.config_item_type);
        item_data["uuid"] = uuid_str;

        node->set_temp_id(m_cluster_config->next_temp_id());
        node->setData(0, Qt::UserRole, item_data);
        node->setText(0, config_item.node_text);
        node->set_id(config_item.id);

        //node->set_config_item_type(config_item.config_item_type);

        node->set_parent_id(parent_node->temp_id());
        node->set_uuid(uuid_str);

        parent_node->add_child(uuid_str, config_item.config_item_type, node) ;

        parent_node->addChild(node);
        m_cluster_config->cluster_nodes[uuid_str]=node;

    }

    template<typename T>
    T* get_cluster_node(QString uid)
    {
        if (auto it{m_cluster_config->cluster_nodes.find(uid)};
                it != std::end(m_cluster_config->cluster_nodes)){

            const auto&[key, value] {*it};

            auto node = dynamic_cast<T*>(std::get<T*>(value));

            if (node->uuid() == uid)
                return node;
        }

        return nullptr;
   }

    template<typename T1, typename T2>
    void delete_node(T1* node, T2* parent_node)
    {
        if (node->child_nodes().size() > 0){
            //showMessage("Delete Failed! Node is NOT empty.");
           return;
        }
        for(auto&[key, tuple]: parent_node->child_nodes()){
            auto&[type, node_any] = tuple;
            auto cluster_node = std::any_cast<T1*>(node_any);
            if (cluster_node->uuid() == node->uuid()){
               delete cluster_node;
               parent_node->child_nodes_by_ref().erase(node->uuid());
            }
        }

        node = nullptr;
    }


public slots:
    void new_cluster();
    void new_station();
    void new_server();
    void new_disk(ClusterManager::Server*);
    void new_audio_folder(ClusterManager::StorageDisk*);
    void delete_cluster();
    void delete_server();
    void delete_disk();
    void delete_folder();

    void toggle_new_station_button(boolean);
    void on_item_clicked(QTreeWidgetItem* item, int col);
    void context_menu_requested(QPoint);
    void print_tree();

    void save_data();

private:
    Ui::ClusterManagerDlg* ui;

    std::unique_ptr<RootNode> m_root_node;
    QMdiArea* m_mdi_area;

    std::unique_ptr<ClusterConfiguration> m_cluster_config;

    ClusterNode* m_current_cluster_node;
    StationNode* m_current_station_node;
    ServerNode* m_current_audio_server_node;
    DiskNode* m_current_disk_node;
    AudioFolderNode* m_current_folder_node;

    std::unique_ptr<QMenu> m_root_context_menu;
    std::unique_ptr<QMenu> m_cluster_context_menu;
    std::unique_ptr<QMenu> m_audio_server_context_menu;
    std::unique_ptr<QMenu> m_disk_context_menu;
    std::unique_ptr<QMenu> m_folder_context_menu;

    std::unique_ptr<QAction> m_act_cluster;
    std::unique_ptr<QAction> m_act_user;
    std::unique_ptr<QAction> m_act_app;

    std::unique_ptr<QAction> m_context_action;
    std::unique_ptr<QAction> m_act_station;
    std::unique_ptr<QAction> m_act_server;
    std::unique_ptr<QAction> m_act_delete_cluster;
    std::unique_ptr<QAction>m_act_delete_server;
    std::unique_ptr<QAction>m_act_delete_disk;

    std::unique_ptr<QAction> m_act_folder;
    std::unique_ptr<QAction> m_act_delete_folder;

    std::unique_ptr<QAction> m_act_audio_server;

};




#endif // CLUSTERMANAGERDLG_H
