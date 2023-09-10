#ifndef CLUSTERMANAGERDLG_H
#define CLUSTERMANAGERDLG_H

#include <variant>

#include <QDialog>
#include <QTreeWidgetItem>
#include <QUuid>
#include <QMap>
#include <QVariant>

#include "../../../framework/entitydatamodel.h"
#include "confignode.h"

namespace Ui {
class ClusterManagerDlg;
}

namespace SECURITY{
    class Role;
    class User;
}


namespace ClusterManager{
    class Cluster;
    class Server;
    class Station;
    class StorageDisk;
    class AudioFolder;
    class Database;
}

class QMdiArea;

using RootNode = ClusterManager::ConfigNode<ClusterManager::RootConfigNode>;
using ClusterNode = ClusterManager::ConfigNode<ClusterManager::Cluster>;
using StationNode = ClusterManager::ConfigNode<ClusterManager::Station>;
using ServerNode = ClusterManager::ConfigNode<ClusterManager::Server>;
using DiskNode = ClusterManager::ConfigNode<ClusterManager::StorageDisk>;
using AudioFolderNode = ClusterManager::ConfigNode<ClusterManager::AudioFolder>;
using DatabaseNode = ClusterManager::ConfigNode<ClusterManager::Database>;

using ClusterGroupNode = ClusterManager::ConfigNode<ClusterManager::ClusterGroupConfig>;

using RoleAndUserGroupNode = ClusterManager::ConfigNode<ClusterManager::RoleAndUserConfig>;
using RoleGroupNode = ClusterManager::ConfigNode<ClusterManager::RoleConfig>;
using UserGroupNode = ClusterManager::ConfigNode<ClusterManager::UserGroupConfig>;

using AppGroupNode = ClusterManager::ConfigNode<ClusterManager::AppGroupConfig>;

using RoleNode = ClusterManager::ConfigNode<SECURITY::Role>;
using UserNode = ClusterManager::ConfigNode<SECURITY::User>;

using AppNode = ClusterManager::ConfigNode<ClusterManager::ApplicationConfig>;

using StationGroupNode = ClusterManager::ConfigNode<ClusterManager::StationGroupConfig>;
using ServerGroupNode = ClusterManager::ConfigNode<ClusterManager::StationGroupConfig>;

using VarNodes =  std::map<QString, std::variant<RootNode*, ClusterNode*, StationNode*,
            ServerNode*, DiskNode*, AudioFolderNode*, ClusterGroupNode*, UserGroupNode*, AppGroupNode*,
            RoleNode*, UserNode*, AppNode*, StationGroupNode*, ServerGroupNode*>>;

using VarPairNodes =  std::pair<QString, std::variant<RootNode*, ClusterNode*, StationNode*,
            ServerNode*, DiskNode*, AudioFolderNode*, ClusterGroupNode*, UserGroupNode*, AppGroupNode*,
            RoleNode*, UserNode*, AppNode*, StationGroupNode*, ServerGroupNode*>>;

using EntityRecord = std::tuple<std::string, std::shared_ptr<BaseEntity>>;

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
    void show_station_context_menu(QString, QPoint);
    void show_station_group_context_menu(QString, QPoint);
    void show_server_group_context_menu(QString, QPoint);
    void show_root_context_menu(QString, QPoint);
    void show_role_group_context_menu(QString, QPoint);
    void show_user_group_context_menu(QString, QPoint);
    void show_role_context_menu(QString, QPoint);
    void show_user_context_menu(QString, QPoint);
    void show_app_group_context_menu(QString, QPoint);
    void show_audio_server_context_menu(QString, QPoint);
    void show_disk_context_menu(QString, QPoint);
    void show_folder_context_menu(QString, QPoint);
    void tree_printer(std::map<QString, std::tuple<ConfigItemType, std::any>>);
    void save_tree(std::map<QString, std::tuple<ConfigItemType, std::any>>, int);
    void make_root_node();
    void make_group_nodes();
    QMap<QString, QVariant> make_node_data(ConfigItemType);

    void make_cluster_node(std::shared_ptr<ClusterManager::Cluster>);
    void clear_configuration();
    void add_members_to_role(std::shared_ptr<SECURITY::Role>, std::vector<EntityRecord> const&);

    template<typename T1, typename T2, typename T3>
    T2* make_node(T1 node_entity, T3* parent_node, ClusterManager::ConfigItem config_item)
    {
        auto node = new T2(config_item);

        auto uuid = QUuid().createUuid();
        QString uuid_str =  uuid.toString(QUuid::WithoutBraces).left(8);

        QMap<QString, QVariant> item_data;
        item_data["type"] = static_cast<int>(config_item.config_item_type);
        item_data["uuid"] = uuid_str;

        node->setData(0, Qt::UserRole, item_data);
        node->setText(0, config_item.node_text);
        node->setIcon(0, QIcon(":images/icons/"+config_item.node_icon_file));
        node->set_node_type(config_item.node_type);

        //if (config_item.id == -1){
        if (node_entity->id() == -1){
            node->set_temp_id(m_cluster_config->next_temp_id());
            config_item.id = node->temp_id();
        }else{
            config_item.id = node_entity->id();
        }

        node->set_id(config_item.id);

        node->set_node_entity(std::move(node_entity));
        node->set_parent_id(parent_node->id());
        node->set_uuid(uuid_str);

        parent_node->add_child(uuid_str, config_item.config_item_type, node) ;

        parent_node->addChild(node);
        m_cluster_config->cluster_nodes[uuid_str] = node;

        return node;

    }

    template<typename T>
    T* get_cluster_node(QString uid)
    {
        if (auto it{m_cluster_config->cluster_nodes.find(uid)};
                it != std::end(m_cluster_config->cluster_nodes)){

            const auto&[key, value] {*it};

            auto node = dynamic_cast<T*>(std::get<T*>(value));

            qDebug() << "GET CLUSTER UID: "<< uid << "NODE ID: "<< node->uuid();

            if (node->uuid() == uid)
                return node;
        }

        return nullptr;
   }

//   template<typename T1, typename T2=VarPairNodes>
   template<typename T1, NodeType T3, typename T2=VarPairNodes>
    struct FindNodeById{
        FindNodeById(int id): m_id{id}{}
        bool operator()(T2 node){
            const auto&[key, value] = node;
            try{
                auto node_type = std::get<T1*>(value);
                auto cluster_node = dynamic_cast<T1*>(node_type);
                return (cluster_node->id() == m_id && cluster_node->node_type() == T3);
            }catch(std::bad_variant_access) {
                return false; }
        }
      private:
            int m_id{-1};
    };


   template<typename T1, NodeType T2>
   T1* get_cluster_node_by_id(int id)
   {
      auto it = std::find_if(m_cluster_config->cluster_nodes.begin(),
                             m_cluster_config->cluster_nodes.end(), FindNodeById<T1, T2>(id));
      if (it != m_cluster_config->cluster_nodes.end()){
          const auto&[key, value] {*it};
          auto node = dynamic_cast<T1*>(std::get<T1*>(value));
              return node;
      }
      return nullptr;
   }

   template<ConfigItemType T1, NodeType T2>
   ClusterManager::ConfigItem node_context(QString title, QString icon="")
   {
       ClusterManager::ConfigItem item;
       item.config_item_type = T1;
       item.node_type = T2;
       item.node_text = title;
       item.node_icon_file = icon;
       //item.node_text = ClusterManager::item_type_to_string(T1)+": "+title;
       return item;
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

    template<typename T1, typename T2, typename T3>
    void edit_node(T1* current_node, StringField* (T3::*mp)()const)
    {
        T3* node_entity = current_node->node_entity();
        auto edit_form = std::make_unique<T2>(node_entity);
        if (edit_form->exec() > 0){
            EntityDataModel edm;
            edm.updateEntity(*node_entity);
            current_node->update_node_text(stoq((node_entity->*mp)()->value()));
        }

    }

    inline void print(std::string msg)
    {
        // print
        qDebug() << QString::fromStdString(msg);
    }

public slots:
    void new_cluster();
    void new_station(int, int);
    void new_server(int, int);
    void new_disk(ClusterManager::Server*);
    void new_audio_folder(ClusterManager::StorageDisk*);
    void edit_cluster();
    void edit_station(StationNode*);
    void delete_cluster();
    void delete_server();
    void delete_disk();
    void delete_folder();
    void new_role(RoleNode*);
    void new_user(UserNode*);
    void edit_user(UserNode*);
    void attach_user_to_station(UserNode*);
    void edit_role(RoleNode*);
    void delete_role(RoleNode*);

    bool drop_role(SECURITY::Role*);

    void edit_folder(AudioFolderNode*);
    void edit_server(ServerNode*);
    void edit_disk(DiskNode*);
    void new_application(AppGroupNode*);

    void toggle_new_station_button(boolean);
    void on_item_clicked(QTreeWidgetItem* item, int col);
    void context_menu_requested(QPoint);
    void print_tree();

    void save_data();
    void load_data();
    void load_cluster_data();
    void load_cluster_data_new();
    void load_users_data();
    void load_roles_data();
    void load_content_data();

private:
    Ui::ClusterManagerDlg* ui;

    std::unique_ptr<RootNode> m_root_node;
    std::unique_ptr<ClusterGroupNode> m_cluster_group_node;
    std::unique_ptr<RoleAndUserGroupNode> m_role_user_group_node;
    std::unique_ptr<RoleGroupNode> m_role_group_node;
    std::unique_ptr<UserGroupNode> m_user_group_node;
    std::unique_ptr<StationGroupNode> m_station_group_node;
    std::unique_ptr<AppGroupNode> m_app_group_node;

    QMdiArea* m_mdi_area;

    std::unique_ptr<ClusterConfiguration> m_cluster_config;
    std::unique_ptr<EntityDataModel> m_content_edm;

    ClusterNode* m_current_cluster_node;
    StationNode* m_current_station_node;
    ServerNode* m_current_server_group_node;
    ServerNode* m_current_audio_server_node;
    DiskNode* m_current_disk_node;
    AudioFolderNode* m_current_folder_node;
    DatabaseNode* m_current_database_node;

    RoleNode* m_current_role_group_node;
    UserNode* m_current_user_node;
    RoleNode* m_current_role_node;

    std::unique_ptr<QMenu> m_root_context_menu;
    std::unique_ptr<QMenu> m_cluster_group_context_menu;
    std::unique_ptr<QMenu> m_role_group_context_menu;
    std::unique_ptr<QMenu> m_role_context_menu;
    std::unique_ptr<QMenu> m_user_context_menu;
    std::unique_ptr<QMenu> m_user_group_context_menu;
    std::unique_ptr<QMenu> m_app_group_context_menu;

    std::unique_ptr<QMenu> m_cluster_context_menu;
    std::unique_ptr<QMenu> m_audio_server_context_menu;
    std::unique_ptr<QMenu> m_disk_context_menu;
    std::unique_ptr<QMenu> m_folder_context_menu;

    std::unique_ptr<QMenu> m_station_context_menu;
    std::unique_ptr<QMenu> m_station_group_context_menu;
    std::unique_ptr<QMenu> m_server_group_context_menu;

    std::unique_ptr<QAction> m_act_cluster;
    std::unique_ptr<QAction> m_act_group_role;
    std::unique_ptr<QAction> m_act_edit_role;
    std::unique_ptr<QAction> m_act_delete_role;
    std::unique_ptr<QAction> m_act_user_group;
    std::unique_ptr<QAction> m_act_user;
    std::unique_ptr<QAction> m_act_attach_station;

    std::unique_ptr<QAction> m_act_app;

    std::unique_ptr<QAction> m_context_action;
    std::unique_ptr<QAction> m_act_station;
    std::unique_ptr<QAction> m_act_group_station;
    std::unique_ptr<QAction> m_act_server;
    std::unique_ptr<QAction> m_act_edit_cluster;
    std::unique_ptr<QAction> m_act_delete_cluster;
    std::unique_ptr<QAction>m_act_delete_server;
    std::unique_ptr<QAction>m_act_delete_disk;

    std::unique_ptr<QAction> m_act_folder;
    std::unique_ptr<QAction> m_act_edit_folder;
    std::unique_ptr<QAction> m_act_delete_folder;

    std::unique_ptr<QAction> m_act_audio_server;
    std::unique_ptr<QAction> m_act_edit_audio_server;
    std::unique_ptr<QAction> m_act_edit_disk;


};


#endif // CLUSTERMANAGERDLG_H
