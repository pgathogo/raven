#include <sstream>
#include <QTreeWidgetItem>
#include <QUuid>
#include <QAction>
#include <QMenu>

#include "clustermanagerdlg.h"
#include "ui_clustermanagerdlg.h"

#include "../../../framework/choicefield.h"
#include "../../../rave/security/roleform.h"
#include "../../../rave/security/userform.h"
#include "../../../rave/security/user.h"
#include "../../../rave/security/role.h"
#include "../../../rave/security/roleuser.h"
#include "../../../rave/security/content.h"
#include "../../../rave/security/contentform.h"

#include "../../../rave/security/rolemember.h"


#include "cluster.h"
#include "station.h"
#include "server.h"
#include "storagedisk.h"
#include "audiofolder.h"

#include "clusterform.h"
#include "stationform.h"
#include "serverform.h"
#include "storagediskform.h"
#include "audiofolderform.h"

int ClusterConfiguration::temp_id{100};

int ClusterConfiguration::next_temp_id()
{
    return ++temp_id;
}


ClusterManagerDlg::ClusterManagerDlg(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::ClusterManagerDlg)
    ,m_root_context_menu{nullptr}
    ,m_cluster_context_menu{nullptr}
    ,m_act_cluster{nullptr}
    ,m_act_user_group{nullptr}
    ,m_act_app{nullptr}
    ,m_context_action{nullptr}
    ,m_act_group_station{nullptr}
    ,m_act_server{nullptr}
    ,m_act_edit_role{nullptr}
    ,m_act_delete_role{nullptr}
    ,m_act_user{nullptr}
{
    ui->setupUi(this);
    setWindowTitle("Cluster Management");

    m_content_edm = std::make_unique<EntityDataModel>(std::make_shared<Content>());

    connect(ui->btnNewCluster, &QPushButton::clicked, this, &ClusterManagerDlg::new_cluster);
    //connect(ui->btnNewStation, &QPushButton::clicked, this, &ClusterManagerDlg::new_station);
//    connect(ui->btnNewAudioServer, &QPushButton::clicked, this, &ClusterManagerDlg::new_server);
    connect(ui->btnSave, &QPushButton::clicked, this, &ClusterManagerDlg::save_data);

    toggle_new_station_button(false);

    make_root_node();

    make_group_nodes();

    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &ClusterManagerDlg::on_item_clicked);
    connect(ui->btnPrintTree, &QPushButton::clicked, this, &ClusterManagerDlg::print_tree);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &ClusterManagerDlg::context_menu_requested);

    connect(ui->btnTest, &QPushButton::clicked, this, &ClusterManagerDlg::load_data);

    connect(ui->btnClear, &QPushButton::clicked, this, &ClusterManagerDlg::clear_configuration);

}

QMap<QString, QVariant> ClusterManagerDlg::make_node_data(ConfigItemType node_type)
{
    QMap<QString, QVariant> node_data;
    auto uuid = QUuid().createUuid();
    QString uuid_str = uuid.toString(QUuid::WithoutBraces).left(8);

    node_data["type"] = static_cast<int>(node_type);
    node_data["uuid"] = uuid_str;

    return node_data;
}

void ClusterManagerDlg::make_root_node()
{
    ClusterManager::ConfigItem root_item;
    root_item.config_item_type = ConfigItemType::Root;
    root_item.node_type = NodeType::Group;
    root_item.node_text = "Cluster Management (Server Name)";

    QMap<QString, QVariant> item_data = make_node_data(root_item.config_item_type);

    /*
    auto uuid = QUuid().createUuid();
    QString uuid_str = uuid.toString(QUuid::WithoutBraces).left(8);
    item_data["type"] = static_cast<int>(root_item.config_item_type);
    item_data["uuid"] = uuid_str;
    */

    m_root_node = std::make_unique<RootNode>(root_item);
    m_root_node->set_config_item_type(root_item.config_item_type);
    m_root_node->setText(0, root_item.node_text);
    m_root_node->setData(0, Qt::UserRole, item_data);
    ui->treeWidget->addTopLevelItem(m_root_node.get());

    m_cluster_config = std::make_unique<ClusterConfiguration>();
    m_cluster_config->cluster_nodes[item_data["uuid"].toString()] = m_root_node.get();
    m_cluster_config->cluster_nodes[item_data["uuid"].toString()] = m_root_node.get();
}

void ClusterManagerDlg::make_group_nodes()
{
    // Cluster Group
    auto context = node_context<ConfigItemType::ClusterGroup, NodeType::Group>(
                "Clusters");
    auto node_data = make_node_data(context.config_item_type);

    m_cluster_group_node = std::make_unique<ClusterGroupNode>(context);
    m_cluster_group_node->set_config_item_type(context.config_item_type);
    m_cluster_group_node->setText(0, context.node_text);
    m_cluster_group_node->setData(0, Qt::UserRole, node_data);
    m_cluster_group_node->setIcon(0, QIcon(":images/icons/master_cluster.png"));


    // Users Group
    auto role_user_group_context = node_context<ConfigItemType::RoleAndUserGroup, NodeType::Group>(
                "Roles/Users");
    auto user_node_data = make_node_data(role_user_group_context.config_item_type);
    m_role_user_group_node = std::make_unique<RoleAndUserGroupNode>(role_user_group_context);


    m_role_user_group_node->set_config_item_type(role_user_group_context.config_item_type);
    m_role_user_group_node->setText(0, role_user_group_context.node_text);
    m_role_user_group_node->setData(0, Qt::UserRole, user_node_data);
    m_role_user_group_node->setIcon(0, QIcon(":images/icons/master_users.png"));

    // Role Group Node

    auto role_group_context = node_context<ConfigItemType::RoleGroup, NodeType::Group>(
                "Roles");
    auto role_group_data = make_node_data(role_group_context.config_item_type);

    auto role_group = std::make_shared<SECURITY::Role>();
    m_current_role_group_node = make_node<std::shared_ptr<SECURITY::Role>, RoleNode, RoleAndUserGroupNode>(
                std::move(role_group), m_role_user_group_node.get(), role_group_context);

//    m_role_group_node = std::make_unique<RoleGroupNode>(role_group_context);
//    m_role_group_node->set_config_item_type(role_group_context.config_item_type);
//    m_role_group_node->setText(0, role_group_context.node_text);
//    m_role_group_node->setData(0, Qt::UserRole, role_group_data);

    m_role_user_group_node->addChild(m_role_group_node.get());


    // User Group Node
    auto user_group_context = node_context<ConfigItemType::UserGroup, NodeType::Group>(
                "Users");
    auto user_group_data = make_node_data(user_group_context.config_item_type);
    auto user_group = std::make_shared<SECURITY::User>();
    m_current_user_node = make_node<std::shared_ptr<SECURITY::User>, UserNode, RoleAndUserGroupNode>(
                std::move(user_group), m_role_user_group_node.get(), user_group_context);


    // Applications Group
    auto app_context = node_context<ConfigItemType::ApplicationGroup, NodeType::Group>(
                "Applications");
    auto app_node_data = make_node_data(app_context.config_item_type);
    m_app_group_node = std::make_unique<AppGroupNode>(app_context);
    m_app_group_node->set_config_item_type(app_context.config_item_type);
    m_app_group_node->setText(0, app_context.node_text);
    m_app_group_node->setData(0, Qt::UserRole, app_node_data);

    m_root_node->addChild(m_cluster_group_node.get());
    m_root_node->addChild(m_role_user_group_node.get());
    m_root_node->addChild(m_app_group_node.get());
}

void ClusterManagerDlg::make_cluster_node(std::shared_ptr<ClusterManager::Cluster> cluster)
{
    auto context = node_context<ConfigItemType::Cluster, NodeType::Group>(
            stoq(cluster->cluster_name()->value()));

    EntityDataModel edm;
    context.id = edm.createEntityDB(*cluster);

    auto cluster_node = make_node<std::shared_ptr<ClusterManager::Cluster>, ClusterNode, ClusterGroupNode>(
            std::move(cluster), m_cluster_group_node.get(), context);

    // Station Group Node
    QString group_name = "Stations";
    auto station_context = node_context<ConfigItemType::StationGroup, NodeType::Group>(
                group_name);
    auto station_group_data = make_node_data(station_context.config_item_type);

    auto station = std::make_shared<ClusterManager::Station>();

    make_node<std::shared_ptr<ClusterManager::Station>, StationNode, ClusterNode>(
                std::move(station), cluster_node, station_context);

   // Servers Group Node
   QString server_group_name = "Servers";
   auto server_context = node_context<ConfigItemType::ServerGroup, NodeType::Group>(
               server_group_name);
   auto server_group_data = make_node_data(server_context.config_item_type);
   auto server = std::make_shared<ClusterManager::Server>();
   make_node<std::shared_ptr<ClusterManager::Server>, ServerNode, ClusterNode>(
               std::move(server), cluster_node, server_context);
}



void ClusterManagerDlg::toggle_new_station_button(boolean state)
{
    ui->btnNewStation->setEnabled(state);
}

ClusterManagerDlg::~ClusterManagerDlg()
{
    delete ui;
}

void ClusterManagerDlg::setMdiArea(QMdiArea* mdi)
{
    m_mdi_area = mdi;
}

void ClusterManagerDlg::new_cluster()
{
    auto cluster = std::make_shared<ClusterManager::Cluster>();

    std::unique_ptr<ClusterForm> cform =
            std::make_unique<ClusterForm>(cluster.get());

    if (cform->exec() > 0){
        make_cluster_node(std::move(cluster));
    }

    m_act_cluster = nullptr;
    m_act_user_group = nullptr;
    m_act_app = nullptr;
    m_root_context_menu = nullptr;
}

void ClusterManagerDlg::new_role(RoleNode* role_group_node)
{
    auto role = std::make_shared<SECURITY::Role>();
    std::unique_ptr<RoleForm> rform = std::make_unique<RoleForm>(role.get());

    if (rform->exec() > 0 )
    {
       std::string create_stmt = role->make_create_stmt();
       try {
           EntityDataModel edm;
           edm.executeRawSQL(create_stmt);

           // Add role members
           auto& role_members = rform->role_members();
           add_members_to_role(role, role_members);

           auto role_context = node_context<ConfigItemType::Role, NodeType::Leaf>(
                       stoq(role->role_name()->value()));

           make_node<std::shared_ptr<SECURITY::Role>, RoleNode, RoleNode>(
                       std::move(role), role_group_node, role_context);

                       //std::move(role), m_current_role_group_node, role_context);
       } catch (DatabaseException& de) {
           showMessage(de.errorMessage());
       }

    }
}

void ClusterManagerDlg::add_members_to_role(std::shared_ptr<SECURITY::Role> role, std::vector<EntityRecord> const& role_members)
{
    std::vector<std::string> members;

    for(auto& [name, base_entity]: role_members)
    {
       ManyToMany* mtom = dynamic_cast<ManyToMany*>(base_entity.get());
       if (mtom->dbAction() == DBAction::dbaCREATE){
           members.push_back(name);
       }
    }

    if (members.size() > 0)
    {
       std::string grant_stmt = role->make_grant_member_stmt(members);
       EntityDataModel edm;
       edm.executeRawSQL(grant_stmt);
    }

}

void ClusterManagerDlg::new_user(UserNode* user_group_node)
{
    std::shared_ptr<SECURITY::User> user = std::make_shared<SECURITY::User>();
    std::unique_ptr<UserForm> uform = std::make_unique<UserForm>(user.get());
    if (uform->exec() > 0)
    {
        std::string create_stmt = user->make_create_stmt();
        try {
            EntityDataModel edm;
            edm.executeRawSQL(create_stmt);

            auto user_context = node_context<ConfigItemType::User, NodeType::Leaf>(
                        stoq(user->role_name()->value()));

            make_node<std::shared_ptr<SECURITY::User>, UserNode, UserNode>(
                        std::move(user), user_group_node, user_context);

        } catch (DatabaseException& de) {
            showMessage(de.errorMessage());
        }
    }

}

void ClusterManagerDlg::new_application(AppGroupNode* app_group_node)
{
    auto content = std::make_unique<Content>();

    std::unique_ptr<ContentForm> cform =
            std::make_unique<ContentForm>(content.get(), *m_content_edm.get());

    if (cform->exec() > 0){

    }

}

void ClusterManagerDlg::edit_user(UserNode* user_node)
{

    edit_node<UserNode, UserForm, SECURITY::User>(user_node, &SECURITY::User::role_name);

}

void ClusterManagerDlg::edit_role(RoleNode* role_node)
{
    auto role = role_node->node_entity_shared();

    role->roleMember().setParentId(role->oid()->value());

    auto role_form = std::make_unique<RoleForm>(role.get());

    if (role_form->exec() > 0)
    {
        try{
            auto& role_members = role_form->role_members();
            add_members_to_role(role, role_members);
        }catch(DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }

}

void ClusterManagerDlg::delete_role(RoleNode* role_node)
{
    SECURITY::Role* role = role_node->node_entity();

    int answer = QMessageBox::warning(this, tr("Cluster Manager"),
                                      tr("Delete the selected role?"),
                                      QMessageBox::Yes | QMessageBox::No);
    switch(answer)
    {
    case QMessageBox::Yes:

        bool result = drop_role(role);
        if (result){
            delete_node<RoleNode, RoleNode>(role_node, m_current_role_group_node);
            m_act_delete_role = nullptr;
            m_role_context_menu = nullptr;
        }

        break;
    }
}


bool ClusterManagerDlg::drop_role(SECURITY::Role* role)
{
    bool result = false;

    try{
        std::string drop_role_stmt = role->make_drop_role_stmt(role->role_name()->value());
        qDebug() << stoq(drop_role_stmt);

        EntityDataModel edm;
        edm.executeRawSQL(drop_role_stmt);
        result = true;
    }catch (DatabaseException& de){
        showMessage(de.errorMessage());
    }

    return result;
}

void ClusterManagerDlg::edit_server(ServerNode* server_node)
{
    edit_node<ServerNode, ServerForm, ClusterManager::Server>(server_node, &ClusterManager::Server::server_name);

    m_act_audio_server = nullptr;
    m_act_edit_audio_server = nullptr;

}


void ClusterManagerDlg::edit_folder(AudioFolderNode* folder_node)
{
    edit_node<AudioFolderNode, AudioFolderForm, ClusterManager::AudioFolder>(folder_node,
                                                                             &ClusterManager::AudioFolder::folder_name);
}

void ClusterManagerDlg::edit_disk(DiskNode* disk_node)
{
    edit_node<DiskNode, StorageDiskForm, ClusterManager::StorageDisk>(disk_node, &ClusterManager::StorageDisk::disk_name);

    m_act_folder = nullptr;
    m_act_edit_disk = nullptr;
    m_act_delete_disk = nullptr;

}


void ClusterManagerDlg::new_station(int group_id, int cluster_id)
{
    auto station = std::make_shared<ClusterManager::Station>();

    std::unique_ptr<StationForm> sform = std::make_unique<StationForm>(station.get());

    if (sform->exec() > 0 ){

        ClusterManager::ConfigItem station_item;
        auto item_type = ConfigItemType::Station;
        station_item.config_item_type = item_type;
        station_item.node_type = NodeType::Leaf;
        station_item.node_text = ClusterManager::item_type_to_string(item_type)+": "+
                    stoq(station->station_name()->value());

        EntityDataModel edm;
        station->set_cluster(cluster_id);
        int id = edm.createEntityDB(*station);
        station_item.id = id;

        auto group_node = get_cluster_node_by_id<StationNode, NodeType::Group>(group_id);

        make_node<std::shared_ptr<ClusterManager::Station>, StationNode, StationNode>(
                    std::move(station), group_node, station_item);

    }

    m_act_group_station = nullptr;
    m_cluster_context_menu = nullptr;

}

void ClusterManagerDlg::new_server(int group_id, int cluster_id)
{
    auto server = std::make_shared<ClusterManager::Server>();

    std::unique_ptr<ServerForm> sform = std::make_unique<ServerForm>(server.get(), this);

    if (sform->exec()  > 0)
    {
        ClusterManager::ConfigItem server_item;

        auto item_type =  (server->server_type()->valueToString() == "ADS" ) ?
                    ConfigItemType::AudioServer : ConfigItemType::DBServer;

        server_item.config_item_type = item_type;
        server_item.node_type = NodeType::Leaf;
        server_item.node_text = ClusterManager::item_type_to_string(item_type)+": "+
                stoq(server->server_name()->value());

        EntityDataModel edm;
        server->set_cluster(cluster_id);
        int id = edm.createEntityDB(*server);
        server_item.id = id;

        auto group_node = get_cluster_node_by_id<ServerNode, NodeType::Group>(group_id);

        make_node<std::shared_ptr<ClusterManager::Server>, ServerNode, ServerNode>(
                    std::move(server), group_node, server_item );
    }

    m_act_server = nullptr;
    m_cluster_context_menu = nullptr;
}

void ClusterManagerDlg::edit_cluster()
{
    auto selected_cluster = m_current_cluster_node->node_entity();
    std::unique_ptr<ClusterForm> cform =
            std::make_unique<ClusterForm>(selected_cluster);
    if (cform->exec() > 0){
        EntityDataModel edm;
        // TODO: Add try catch for database action
        edm.updateEntity(*selected_cluster);
        m_current_cluster_node->update_node_text(stoq(selected_cluster->cluster_name()->value()));
    }
}

void ClusterManagerDlg::edit_station(StationNode* station_node)
{
    auto station = station_node->node_entity();

    std::unique_ptr<StationForm> sform =
           std::make_unique<StationForm>(station);

    if (sform->exec()){
        EntityDataModel edm;
        edm.updateEntity(*station);

        station_node->setText(0, stoq(station->station_name()->value()));
    }

    m_station_context_menu = nullptr;
    m_act_station = nullptr;

}


void ClusterManagerDlg::delete_cluster()
{
    delete_node<ClusterNode, RootNode>(m_current_cluster_node, m_root_node.get());
    m_act_delete_cluster = nullptr;
    m_cluster_context_menu = nullptr;
}


void ClusterManagerDlg::delete_server()
{
    delete_node<ServerNode, ClusterNode>(m_current_audio_server_node, m_current_cluster_node);

    m_act_delete_server = nullptr;
    m_cluster_context_menu = nullptr;
}

void ClusterManagerDlg::delete_disk()
{
    delete_node<DiskNode, ServerNode>(m_current_disk_node, m_current_audio_server_node);
    m_act_delete_disk = nullptr;
    m_audio_server_context_menu = nullptr;

}

void ClusterManagerDlg::delete_folder()
{
    delete_node<AudioFolderNode, DiskNode>(m_current_folder_node, m_current_disk_node);

    m_act_delete_folder = nullptr;
    m_folder_context_menu = nullptr;
}



void ClusterManagerDlg::new_disk(ClusterManager::Server* server)
{
    auto storage_disk = std::make_shared<ClusterManager::StorageDisk>();

    auto server_name = server->server_name()->value();

    std::unique_ptr<StorageDiskForm> sform =
            std::make_unique<StorageDiskForm>(server_name, storage_disk.get(), this);
    if (sform->exec() > 0)
    {
        ClusterManager::ConfigItem disk_item;
        auto item_type = ConfigItemType::Disk;
        disk_item.config_item_type = item_type;
        disk_item.node_type = NodeType::Leaf;
        disk_item.node_text = ClusterManager::item_type_to_string(item_type)+": "+
                stoq(storage_disk->disk_name()->value());

        EntityDataModel edm;
        storage_disk->set_audio_server(m_current_audio_server_node->id());
        int id = edm.createEntityDB(*storage_disk);
        disk_item.id = id;

        make_node<std::shared_ptr<ClusterManager::StorageDisk>, DiskNode, ServerNode>(
                    std::move(storage_disk), m_current_audio_server_node, disk_item );

    }

    m_act_audio_server = nullptr;
    m_act_edit_audio_server = nullptr;
    m_act_delete_server = nullptr;
    m_audio_server_context_menu = nullptr;

}

void ClusterManagerDlg::new_audio_folder(ClusterManager::StorageDisk* storage_disk)
{

    auto audio_folder = std::make_shared<ClusterManager::AudioFolder>();

    auto disk_name = storage_disk->disk_name()->value();

    std::unique_ptr<AudioFolderForm> folder_form =
            std::make_unique<AudioFolderForm>(disk_name, audio_folder.get(), this);

    if (folder_form->exec() > 0) {
        ClusterManager::ConfigItem folder_item;
        auto folder_type = ConfigItemType::AudioFolder;
        folder_item.config_item_type = folder_type;
        folder_item.node_type = NodeType::Leaf;
        folder_item.node_text = ClusterManager::item_type_to_string(folder_type)+": "+
                stoq(audio_folder->folder_name()->value());

        EntityDataModel edm;
        audio_folder->set_disk(m_current_disk_node->id());
        int id = edm.createEntityDB(*audio_folder);
        folder_item.id = id;

        make_node<std::shared_ptr<ClusterManager::AudioFolder>, AudioFolderNode, DiskNode>(
                    std::move(audio_folder), m_current_disk_node, folder_item);

    }

    m_act_folder = nullptr;
    m_act_delete_disk = nullptr;
    m_disk_context_menu = nullptr;

}


void ClusterManagerDlg::on_item_clicked(QTreeWidgetItem* item, int col)
{

   auto data = item->data(0, Qt::UserRole).toMap();
   int node_type = data["type"].toInt();
   QString uuid = data["uuid"].toString();

   switch(node_type)
   {
    case static_cast<int>(ConfigItemType::Cluster):
    {
        m_current_cluster_node = get_cluster_node<ClusterNode>(uuid);

        toggle_new_station_button(true);
        qDebug() << "Cluster Selected";
        break;
    }
    case static_cast<int>(ConfigItemType::StationGroup):
   {
       auto station_group = get_cluster_node<StationNode>(uuid);
       break;
   }
    case static_cast<int>(ConfigItemType::Station):
    {
        m_current_station_node = get_cluster_node<StationNode>(uuid);
        toggle_new_station_button(false);
        qDebug() << "Station Selected";
       break;
    }

    case static_cast<int>(ConfigItemType::AudioServer):
   {
       qDebug() << "Server Clicked **";
       m_current_audio_server_node = get_cluster_node<ServerNode>(uuid);
       break;
   }
  }
}

void ClusterManagerDlg::context_menu_requested(QPoint pos)
{
   QTreeWidgetItem* selected_item = ui->treeWidget->currentItem();
   auto data = selected_item->data(0, Qt::UserRole).toMap();

   int node_type = data["type"].toInt();
   QString uuid = data["uuid"].toString();

   switch(node_type)
   {
     case static_cast<int>(ConfigItemType::ClusterGroup):
     {
       auto root_node = get_cluster_node<ClusterGroupNode>(uuid);

       if (m_root_context_menu != nullptr){
           m_root_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
           return;
       }

       show_root_context_menu(uuid, pos);
       break;

     }

     case static_cast<int>(ConfigItemType::RoleGroup):
   {
       if (m_role_group_context_menu != nullptr){
           m_role_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
           return;
       }
       show_role_group_context_menu(uuid, pos);
       break;
   }
   case static_cast<int>(ConfigItemType::Role):
   {
       if (m_role_context_menu != nullptr){
          m_role_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
       }

       show_role_context_menu(uuid, pos);
       break;
   }


     case static_cast<int>(ConfigItemType::UserGroup):
     {
       if (m_user_group_context_menu != nullptr){
           m_user_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
           return;
       }
       show_user_group_context_menu(uuid, pos);
       break;
     }
     case static_cast<int>(ConfigItemType::User):
   {
       if (m_user_context_menu != nullptr){
           m_user_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
       }

       show_user_context_menu(uuid, pos);
       break;
   }
     case static_cast<int>(ConfigItemType::ApplicationGroup):
     {
       if (m_app_group_context_menu != nullptr){
           m_app_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
       }

       show_app_group_context_menu(uuid, pos);
       break;
     }

     case static_cast<int>(ConfigItemType::Cluster):
     {
        m_current_cluster_node = get_cluster_node<ClusterNode>(uuid);

       if (m_cluster_context_menu != nullptr){
           m_cluster_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
       }

       show_cluster_context_menu(uuid, pos);
       break;

     }

    case static_cast<int>(ConfigItemType::StationGroup):
   {
       if (m_station_group_context_menu != nullptr) {
           m_station_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
       }

       show_station_group_context_menu(uuid, pos);
       break;
   }

   case static_cast<int>(ConfigItemType::Station):
   {
       if (m_station_context_menu != nullptr){
           m_station_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
       }

       show_station_context_menu(uuid, pos);
       break;
   }

   case static_cast<int>(ConfigItemType::ServerGroup):
   {
       if (m_server_group_context_menu != nullptr) {
           m_server_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
       }
       show_server_group_context_menu(uuid, pos);
       break;
   }

    case static_cast<int>(ConfigItemType::AudioServer):
   {
       m_current_audio_server_node = get_cluster_node<ServerNode>(uuid);

       show_audio_server_context_menu(uuid, pos);
       break;
   }

   case static_cast<int>(ConfigItemType::Disk):{
       m_current_disk_node = get_cluster_node<DiskNode>(uuid);

       if (m_disk_context_menu != nullptr){
           m_disk_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
           return;
       }

       show_disk_context_menu(uuid, pos);
       break;
   }
   case static_cast<int>(ConfigItemType::AudioFolder):{
       m_current_folder_node = get_cluster_node<AudioFolderNode>(uuid);

       if (m_folder_context_menu != nullptr){
           m_folder_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
           return;
       }

       show_folder_context_menu(uuid, pos);
       break;
   }


   }

}

void ClusterManagerDlg::show_root_context_menu(QString node_id, QPoint pos)
{
    m_root_context_menu = std::make_unique<QMenu>();

    if (m_act_cluster == nullptr)
       m_act_cluster = std::make_unique<QAction>("New Cluster...");

    connect(m_act_cluster.get(), &QAction::triggered, this, [this](){ new_cluster(); });

    m_root_context_menu->addAction(m_act_cluster.get());

    m_root_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_role_group_context_menu(QString node_id, QPoint pos)
{
    m_role_group_context_menu = std::make_unique<QMenu>();
    if (m_act_group_role == nullptr)
        m_act_group_role = std::make_unique<QAction>("New Role...");

    auto role_group_node = get_cluster_node_by_id<RoleNode, NodeType::Group>(m_current_role_group_node->id());
    if (role_group_node == nullptr)
        return;
    connect(m_act_group_role.get(), &QAction::triggered, this, [this, role_group_node](){ new_role(role_group_node);});
    m_role_group_context_menu->addAction(m_act_group_role.get());
    m_role_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_user_group_context_menu(QString node_id, QPoint pos)
{
    m_user_group_context_menu = std::make_unique<QMenu>();

    if (m_act_user_group == nullptr )
       m_act_user_group = std::make_unique<QAction>("New User...");

    auto user_group_node = get_cluster_node_by_id<UserNode, NodeType::Group>(m_current_user_node->id());

    if (user_group_node == nullptr)
        return;

    connect(m_act_user_group.get(), &QAction::triggered, this, [this, user_group_node](){new_user(user_group_node);});
    m_user_group_context_menu->addAction(m_act_user_group.get());

    m_user_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
}



void ClusterManagerDlg::show_user_context_menu(QString node_uuid, QPoint pos)
{
    m_user_context_menu = std::make_unique<QMenu>();

    m_act_user = nullptr;

    m_act_user = std::make_unique<QAction>("Edit User...");

    auto user_node = get_cluster_node<UserNode>(node_uuid);

    if (user_node == nullptr)
        return;

    connect(m_act_user.get(), &QAction::triggered, this, [this, user_node](){edit_user(user_node);});
    m_user_context_menu->addAction(m_act_user.get());
    m_user_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
}

void ClusterManagerDlg::show_role_context_menu(QString node_id, QPoint pos)
{
    m_role_context_menu = std::make_unique<QMenu>();

    m_act_edit_role = nullptr;
    m_act_delete_role = nullptr;

    m_act_edit_role = std::make_unique<QAction>("Edit Role...");
    m_act_delete_role = std::make_unique<QAction>("Delete Role...");

    auto role_node = get_cluster_node<RoleNode>(node_id);

    if (role_node == nullptr)
        return;

    connect(m_act_edit_role.get(), &QAction::triggered, this, [this, role_node](){edit_role(role_node);});
    connect(m_act_delete_role.get(), &QAction::triggered, this, [this, role_node](){delete_role(role_node);});

    m_role_context_menu->addAction(m_act_edit_role.get());
    m_role_context_menu->addSeparator();
    m_role_context_menu->addAction(m_act_delete_role.get());
    m_role_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_app_group_context_menu(QString node_id, QPoint pos)
{
    m_app_group_context_menu = std::make_unique<QMenu>();

    if (m_act_app == nullptr)
       m_act_app = std::make_unique<QAction>("New Application...");

    auto app_group_node = get_cluster_node<AppGroupNode>(node_id);
    connect(m_act_app.get(), &QAction::triggered, this, [this, app_group_node](){new_application(app_group_node);});

    m_app_group_context_menu->addAction(m_act_app.get());

    m_app_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
}


void ClusterManagerDlg::show_cluster_context_menu(QString node_id, QPoint pos)
{
    m_cluster_context_menu = std::make_unique<QMenu>();

    if (m_act_edit_cluster == nullptr)
        m_act_edit_cluster = std::make_unique<QAction>("Edit Cluster...");

    if (m_act_delete_cluster == nullptr)
        m_act_delete_cluster = std::make_unique<QAction>("Delete Cluster");

    connect(m_act_edit_cluster.get(), &QAction::triggered, this, &ClusterManagerDlg::edit_cluster);
    connect(m_act_delete_cluster.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_cluster);

    m_cluster_context_menu->addAction(m_act_edit_cluster.get());
    m_cluster_context_menu->addSeparator();
    m_cluster_context_menu->addAction(m_act_delete_cluster.get());

    m_cluster_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_station_context_menu(QString node_id, QPoint pos)
{
    m_current_station_node = get_cluster_node<StationNode>(node_id);

    m_station_context_menu = std::make_unique<QMenu>();

    if (m_act_station == nullptr)
        m_act_station = std::make_unique<QAction>("Edit station...");

    connect(m_act_station.get(), &QAction::triggered, this, [this](){
      edit_station(m_current_station_node);
    });

    m_station_context_menu->addAction(m_act_station.get());
    m_station_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_station_group_context_menu(QString node_id, QPoint pos)
{
    m_current_station_node = get_cluster_node<StationNode>(node_id);

    m_station_group_context_menu = std::make_unique<QMenu>();
    if(m_act_group_station == nullptr)
        m_act_group_station = std::make_unique<QAction>("New Station...");

    connect(m_act_group_station.get(), &QAction::triggered, this, [this](){
        new_station(m_current_station_node->id(), m_current_station_node->parent_id());});

    m_station_group_context_menu->addAction(m_act_group_station.get());

    m_station_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_server_group_context_menu(QString node_id, QPoint pos)
{
   m_current_server_group_node = get_cluster_node<ServerNode>(node_id);

   m_server_group_context_menu = std::make_unique<QMenu>() ;
    if (m_act_server == nullptr)
       m_act_server = std::make_unique<QAction>("New Server...");

    connect(m_act_server.get(), &QAction::triggered, this, [this]() {
        new_server(m_current_server_group_node->id(),m_current_server_group_node->parent_id());});

    m_server_group_context_menu->addAction(m_act_server.get());
    m_server_group_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
}

void ClusterManagerDlg::show_audio_server_context_menu(QString node_id, QPoint pos)
{
    auto node = get_cluster_node<ServerNode>(node_id);

    if (node == nullptr)
        return;

    if (m_audio_server_context_menu == nullptr)
        m_audio_server_context_menu = std::make_unique<QMenu>();

    if (m_act_audio_server == nullptr)
       m_act_audio_server = std::make_unique<QAction>("New Disk...");

    if (m_act_edit_audio_server == nullptr)
        m_act_edit_audio_server = std::make_unique<QAction>("Edit Server...");

    if (m_act_delete_server == nullptr)
        m_act_delete_server = std::make_unique<QAction>("Delete Server");

    ClusterManager::Server* server = node->node_entity();

    connect(m_act_audio_server.get(), &QAction::triggered, this, [server, this](){ new_disk(server); });
    connect(m_act_edit_audio_server.get(), &QAction::triggered, this,[node, this](){edit_server(node);});
    connect(m_act_delete_server.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_server);

    if (server->server_type()->value() == "ADS"){
        m_audio_server_context_menu->addAction(m_act_audio_server.get());
        m_audio_server_context_menu->addSeparator();
    }else {
        m_audio_server_context_menu->removeAction(m_act_audio_server.get());
    }

    m_audio_server_context_menu->addAction(m_act_edit_audio_server.get());
    m_audio_server_context_menu->addAction(m_act_delete_server.get());

    m_audio_server_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_disk_context_menu(QString node_id, QPoint pos)
{
    m_disk_context_menu = std::make_unique<QMenu>();

    auto node = get_cluster_node<DiskNode>(node_id);
    if (node == nullptr)
        return;

    if (m_act_folder == nullptr)
        m_act_folder = std::make_unique<QAction>("New Folder...");

    if (m_act_edit_disk == nullptr)
        m_act_edit_disk = std::make_unique<QAction>("Edit Disk...");

    if (m_act_delete_disk == nullptr)
        m_act_delete_disk = std::make_unique<QAction>("Delete Disk");

    //ClusterManager::StorageDisk* storage_disk = m_current_disk_node->node_entity();
    ClusterManager::StorageDisk* storage_disk = node->node_entity();

    connect(m_act_folder.get(), &QAction::triggered, this, [storage_disk, this](){
        new_audio_folder(storage_disk); });

    connect(m_act_edit_disk.get(), &QAction::triggered, this, [this, node](){edit_disk(node);});

    connect(m_act_delete_disk.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_disk);

    m_disk_context_menu->addAction(m_act_folder.get());
    m_disk_context_menu->addAction(m_act_edit_disk.get());
    m_disk_context_menu->addSeparator();
    m_disk_context_menu->addAction(m_act_delete_disk.get());
    m_disk_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
}

void ClusterManagerDlg::show_folder_context_menu(QString node_id, QPoint pos)
{
    m_folder_context_menu = std::make_unique<QMenu>();

    auto node = get_cluster_node<AudioFolderNode>(node_id);

    if (node == nullptr)
        return;

    if (m_act_edit_folder == nullptr)
        m_act_edit_folder = std::make_unique<QAction>("Edit Folder");

    if (m_act_delete_folder == nullptr)
        m_act_delete_folder = std::make_unique<QAction>("Delete Folder");

    ClusterManager::AudioFolder* folder = node->node_entity();
    connect(m_act_edit_folder.get(), &QAction::triggered, this, [this, node](){ edit_folder(node);} );
    connect(m_act_delete_folder.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_folder);


    m_folder_context_menu->addAction(m_act_edit_folder.get());
    m_folder_context_menu->addAction(m_act_delete_folder.get());
    m_folder_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}


void ClusterManagerDlg::print_tree()
{
    tree_printer(m_root_node->child_nodes());
}

void ClusterManagerDlg::tree_printer(std::map<QString, std::tuple<ConfigItemType, std::any>> nodes)
{
    for(auto&[key, value]: nodes){
        auto&[node_type, node_any] = value;

       switch(node_type){
        case ConfigItemType::Cluster:{
           auto node = std::any_cast<ClusterNode*>(node_any);

           qDebug() << "Cluster: " << stoq(node->node_entity()->cluster_name()->displayName());

           tree_printer(node->child_nodes());
           break;
        }
        case ConfigItemType::AudioServer:{
            auto node =std::any_cast<ServerNode*>(node_any);
            auto node_entity = node->node_entity();
            qDebug() << " -AudioServer: " << stoq(node_entity->server_name()->displayName());
            tree_printer(node->child_nodes());
            break;
        }
        case ConfigItemType::DBServer:{
            auto node =std::any_cast<ServerNode*>(node_any);
            auto node_entity = node->node_entity();
            qDebug() << " -DB-Server: " << stoq(node_entity->server_name()->displayName());
            tree_printer(node->child_nodes());
            break;
        }
        case ConfigItemType::Disk:{
            auto node =std::any_cast<DiskNode*>(node_any);
            auto node_entity = node->node_entity();
            qDebug() << "  -Disk: " << stoq(node_entity->disk_name()->displayName());
            tree_printer(node->child_nodes());
            break;
        }
        case ConfigItemType::Station:{
            auto node =std::any_cast<StationNode*>(node_any);
            auto node_entity = node->node_entity();
            qDebug() << " -Station: " << stoq(node_entity->station_name()->displayName());
            tree_printer(node->child_nodes());
            break;
        }
        case ConfigItemType::AudioFolder:{
            auto node =std::any_cast<AudioFolderNode*>(node_any);
            auto node_entity = node->node_entity();
            qDebug() << "   -Folder: " << stoq(node_entity->folder_name()->displayName());
            tree_printer(node->child_nodes());
            break;
        }
        default:
            break;
      }

    }

}

void ClusterManagerDlg::save_data()
{
    save_tree(m_root_node->child_nodes(), 0);
    showMessage("Data Saved.");
}

void ClusterManagerDlg::save_tree(std::map<QString, std::tuple<ConfigItemType, std::any>> nodes, int parent_id)
{
    EntityDataModel edm;

    for(auto&[key, value]: nodes){
        auto&[node_type, node_any] = value;

       switch(node_type){
        case ConfigItemType::Cluster:{
           auto node = std::any_cast<ClusterNode*>(node_any);

           auto node_entity = node->node_entity();

           int id = edm.createEntityDB(*node_entity);

           qDebug() << "Cluster: Parent ID: "<< parent_id << "ID: "<< id << ": " << stoq(node->node_entity()->cluster_name()->displayName());

           save_tree(node->child_nodes(), id);

           break;
        }
        case ConfigItemType::AudioServer:{
            auto node =std::any_cast<ServerNode*>(node_any);
            auto node_entity = node->node_entity();
            node_entity->set_cluster(parent_id);

            int id = edm.createEntityDB(*node_entity);

            qDebug() << " -AudioServer: Parent ID: " << parent_id << " ID: "<< id << ": "<< stoq(node_entity->server_name()->displayName());
            tree_printer(node->child_nodes());
            break;
        }
        case ConfigItemType::DBServer:{
            auto node =std::any_cast<ServerNode*>(node_any);
            auto node_entity = node->node_entity();
            node_entity->set_cluster(parent_id);
            int id = edm.createEntityDB(*node_entity);

            qDebug() << " -DB-Server: Parent ID: " << parent_id << " ID: " << id << ": "<< stoq(node_entity->server_name()->displayName());

            save_tree(node->child_nodes(), id);
            break;
        }
        case ConfigItemType::Disk:{
            auto node =std::any_cast<DiskNode*>(node_any);
            auto node_entity = node->node_entity();
            node_entity->set_audio_server(parent_id);
            int id = edm.createEntityDB(*node_entity);

            qDebug() << "  -Disk: Parent ID: " << parent_id << " ID: " << id << ": "<< stoq(node_entity->disk_name()->displayName());
            save_tree(node->child_nodes(), id);
            break;
        }
        case ConfigItemType::Station:{
            auto node =std::any_cast<StationNode*>(node_any);
            auto node_entity = node->node_entity();
            node_entity->set_cluster(parent_id);

            int id = edm.createEntityDB(*node_entity);

            qDebug() << " -Station: Parent ID: " << parent_id << " ID: "<< id << ": "<< stoq(node_entity->station_name()->displayName());
            save_tree(node->child_nodes(), id);
            break;
        }
        case ConfigItemType::AudioFolder:{
            auto node =std::any_cast<AudioFolderNode*>(node_any);
            auto node_entity = node->node_entity();
            node_entity->set_disk(parent_id);

            int id = edm.createEntityDB(*node_entity);

            qDebug() << "   -Folder: Parent ID: " << parent_id << " ID: "<< id << ": "<< stoq(node_entity->folder_name()->displayName());
            save_tree(node->child_nodes(), id);
            break;
        }
        default:
            break;
      }

    }

}

void ClusterManagerDlg::load_data()
{
    //clear_configuration();
    load_cluster_data();
    load_roles_data();
    load_users_data();
    load_content_data();

}

void ClusterManagerDlg::load_content_data()
{
    m_content_edm->all();

}

void ClusterManagerDlg::clear_configuration()
{
    std::vector<QString> keys;

    for (auto it=m_cluster_config->cluster_nodes.begin(); it != m_cluster_config->cluster_nodes.end(); ++it){
        const auto&[key, value] {*it};
        keys.push_back(key);
        auto t = std::get<0>(value);
        auto* node = dynamic_cast<decltype(t)>(t);
        delete node;
    }

    for(auto& k : keys)
        m_cluster_config->cluster_nodes.erase(k);

}

void ClusterManagerDlg::load_cluster_data_new()
{
//    struct cluster_tree{
//        int node_id;
//        int parent_id;
//        std::string node_name;
//    };


}

void ClusterManagerDlg::load_cluster_data()
{
    std::stringstream sql;

    sql << "select a.id as cluster_id, a.cluster_name, "
        << " b.id as station_id, b.station_name, b.cluster_id as station_cluster_id, "
        << "      c.id as server_id, c.server_name, c.server_type, c.server_ip, c.cluster_id as server_cluster_id, "
        << "      d.id as disk_id, d.disk_name, d.capacity, d.audio_server_id, "
        << "      e.id as folder_id, e.folder_name, e.disk_id as folder_disk_id "
        << " from rave_cluster a "
        << " left outer join rave_station b on a.id = b.cluster_id "
        << " left  join rave_server c on a.id = c.cluster_id "
        << " left outer join rave_storagedisk d on c.id = d.audio_server_id "
        << " left outer join rave_audiofolder e on d.id = e.disk_id "
        << " order by a.id ";

    EntityDataModel edm;

    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    int server_group_id = -1;

    if (provider->cacheSize() > 0){
        provider->cache()->first();
       do{
            auto itb = provider->cache()->currentElement()->begin();
            auto ite = provider->cache()->currentElement()->end();

            auto cluster = std::make_shared<ClusterManager::Cluster>();
            auto station = std::make_shared<ClusterManager::Station>();
            auto server = std::make_shared<ClusterManager::Server>();
            auto disk = std::make_shared<ClusterManager::StorageDisk>();
            auto folder = std::make_shared<ClusterManager::AudioFolder>();

            for(; itb != ite; ++itb){

                std::string field = (*itb).first;
                std::string value = (*itb).second;

                // Cluster
                if (field == "cluster_id"){
                    cluster->setId(std::stoi(value));
                }

                if (field == "cluster_name"){
                    cluster->set_cluster_name(value);
                }

                // Station
                if (field == "station_id" && (!value.empty()) ){
                        station->setId(std::stoi(value));
                }

                if (field == "station_name" && station->id() > -1){
                    station->set_station_name(value) ;
                }


                if (field == "station_cluster_id" && (!value.empty()) ){
                    station->set_cluster(std::stoi(value));
                }

                // Server
                if (field == "server_id" && (!value.empty()) ){
                        server->setId(std::stoi(value));
                }

                if (field == "server_name" && server->id() > -1 ){
                   server->set_server_name(value) ;
                }

                if (field == "server_type" && server->id() > -1 ){
                    server->set_server_type(value);
                   if (value == "ADS"){
                        disk->set_audio_server(server->id());
                    }
                }

                if (field == "server_ip" && server->id() > -1 ){
                    server->set_server_ip(value);
                }

                // Disk
                if (field == "disk_id" && (!value.empty()) ){
                        disk->setId(std::stoi(value));
                }

                if (field == "disk_name" && disk->id() > -1 ){
                    disk->set_disk_name(value);
                }

                if (field == "capacity" && disk->id() > -1 ){
                    disk->set_capacity(std::stod(value));
                }

                // Folder
                if (field == "folder_id" && (!value.empty()) ){
                    folder->setId(std::stoi(value));
                    if (disk->id() > -1)
                        folder->set_disk(disk->id());
                }

                if (field == "folder_name" && folder->id() > -1 ){
                    folder->set_folder_name(value);
                }

            }

            // Cluster
            auto cluster_node = get_cluster_node_by_id<ClusterNode, NodeType::Leaf>(cluster->id());
            int station_group_id = -1;
            int temp_c_id = -1;

            if (cluster_node == nullptr)
            {
                server_group_id = -1;

                temp_c_id = cluster->id();

                auto item = node_context<ConfigItemType::Cluster, NodeType::Leaf>(
                            stoq(cluster->cluster_name()->value()+"("+std::to_string(cluster->id())+")"),
                            "cluster.png");

                cluster_node = make_node<std::shared_ptr<ClusterManager::Cluster>, ClusterNode, ClusterGroupNode>(
                            std::move(cluster), m_cluster_group_node.get(), item);

                // Stations Group Node
                QString group_name = "Stations";
                auto station_context = node_context<ConfigItemType::StationGroup, NodeType::Group>(
                            group_name, "radio_tower.png");
                auto station = std::make_shared<ClusterManager::Station>();
                auto station_group_node = make_node<std::shared_ptr<ClusterManager::Station>, StationNode, ClusterNode>(
                            std::move(station), cluster_node, station_context);
                station_group_id = station_group_node->id();

                // Servers Group Node
                QString server_group_name = "Servers";
                auto server_context = node_context<ConfigItemType::ServerGroup, NodeType::Group>(
                            server_group_name);
                auto server = std::make_shared<ClusterManager::Server>();
                auto server_group_node = make_node<std::shared_ptr<ClusterManager::Server>, ServerNode, ClusterNode>(
                            std::move(server), cluster_node, server_context);
                server_group_id = server_group_node->id();
            }

            // Station
            if (station->id() > -1 )
            {
                auto station_node = get_cluster_node_by_id<StationNode, NodeType::Leaf>(station->id());
                if (station_node == nullptr)
                {
                    auto station_item = node_context<ConfigItemType::Station, NodeType::Leaf>(
                            stoq(station->station_name()->value()), "mic.png");

                    if (station_group_id != -1)
                {
                        auto station_group_node = get_cluster_node_by_id<StationNode, NodeType::Group>(station_group_id);
                        make_node<std::shared_ptr<ClusterManager::Station>, StationNode, StationNode>(
                            std::move(station), station_group_node, station_item);
                   }
                }

            }

            // Audio Server
            ServerNode* audio_server_node = nullptr;
            DiskNode* disk_node = nullptr;

            if (server->id() > -1 )
            {
                audio_server_node = get_cluster_node_by_id<ServerNode, NodeType::Leaf>(server->id());

                if (audio_server_node == nullptr)
                {
                    auto server_item = node_context<ConfigItemType::AudioServer, NodeType::Leaf>(
                            stoq(server->server_name()->value()));

                    if (server_group_id != -1)
                    {
                        auto server_group_node = get_cluster_node_by_id<ServerNode, NodeType::Group>(server_group_id);
                        if (server_group_node != nullptr)
                        {

                        server_item.node_text = server_item.node_text+"("+QString::number(server_group_id)+")";
                        audio_server_node = make_node<std::shared_ptr<ClusterManager::Server>, ServerNode, ServerNode>(
                            std::move(server), server_group_node, server_item);
                        }

                    }

                }


                // Disk
                auto disk_node = get_cluster_node_by_id<DiskNode, NodeType::Leaf>(disk->id());
                if (disk_node == nullptr)
                {
                    auto disk_item = node_context<ConfigItemType::Disk, NodeType::Leaf>(
                        stoq(disk->disk_name()->value()));
                    disk_node = make_node<std::shared_ptr<ClusterManager::StorageDisk>, DiskNode, ServerNode>(
                        std::move(disk), audio_server_node, disk_item);
                }

            }

            // Folder
            auto folder_node = get_cluster_node_by_id<AudioFolderNode, NodeType::Leaf>(folder->id());
            if (folder_node == nullptr)
            {
                if (disk_node != nullptr)
                {
                        auto folder_item = node_context<ConfigItemType::AudioFolder, NodeType::Leaf>(
                            stoq(folder->folder_name()->value()));
                        make_node<std::shared_ptr<ClusterManager::AudioFolder>, AudioFolderNode, DiskNode>(
                            std::move(folder), disk_node, folder_item);
                }

            }

        provider->cache()->next();
       } while(!provider->cache()->isLast());

   }

}

void ClusterManagerDlg::load_users_data()
{
    std::unique_ptr<EntityDataModel> edm = std::make_unique<EntityDataModel>(
                std::make_shared<SECURITY::User>());
    edm->all();

    for(auto const&[name, entity] : edm->modelEntities()){
        SECURITY::User* user_ptr= dynamic_cast<SECURITY::User*>(entity.get());

        std::shared_ptr<SECURITY::User> user = std::make_shared<SECURITY::User>();
        *user = *user_ptr;

        if (user->role_name()->value().substr(0,3) == "pg_")
            continue;

        if (!user->rol_can_login()->value())
            continue;

        auto item = node_context<ConfigItemType::User, NodeType::Leaf>(
                    stoq(user_ptr->role_name()->value()));

        auto user_node = make_node<std::shared_ptr<SECURITY::User>, UserNode,  UserNode>(
                    user, m_current_user_node, item);

    }


}


void ClusterManagerDlg::load_roles_data()
{
    std::unique_ptr<EntityDataModel> edm = std::make_unique<EntityDataModel>(
        std::make_shared<SECURITY::Role>());

    edm->all();

    for (auto const&[name, entity] : edm->modelEntities()){

        SECURITY::Role* role_ptr = dynamic_cast<SECURITY::Role*>(entity.get());


        if (role_ptr->role_name()->value().substr(0,3) == "pg_")
            continue;

        if (role_ptr->rol_can_login()->value())
            continue;

        std::shared_ptr<SECURITY::Role> role = std::make_shared<SECURITY::Role>();
        *role = *role_ptr;

        auto item = node_context<ConfigItemType::Role, NodeType::Leaf>(
                    stoq(role_ptr->role_name()->value()));

        auto role_node = make_node<std::shared_ptr<SECURITY::Role>, RoleNode, RoleNode>(
                    role, m_current_role_group_node, item);

    }
}


/*  Droping user

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM jdoe;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM jdoe;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM jdoe;
REVOKE ALL PRIVILEGES ON SCHEMA public FROM jdoe;
ALTER DEFAULT PRIVILEGES IN SCHEMA public REVOKE ALL ON SEQUENCES FROM jdoe;
ALTER DEFAULT PRIVILEGES IN SCHEMA public REVOKE ALL ON TABLES FROM jdoe;
ALTER DEFAULT PRIVILEGES IN SCHEMA public REVOKE ALL ON FUNCTIONS FROM jdoe;
REVOKE USAGE ON SCHEMA public FROM jdoe;
REASSIGN OWNED BY jdoe TO postgres;
DROP USER jdoe ;


*/

