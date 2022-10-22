#include <QTreeWidgetItem>
#include <QUuid>
#include <QAction>
#include <QMenu>

#include "clustermanagerdlg.h"
#include "ui_clustermanagerdlg.h"

#include "../../../framework/choicefield.h"

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

int ClusterConfiguration::temp_id{0};

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
    ,m_act_user{nullptr}
    ,m_act_app{nullptr}
    ,m_context_action{nullptr}
    ,m_act_station{nullptr}
    ,m_act_server{nullptr}
{
    ui->setupUi(this);
    setWindowTitle("Cluster Management");

    connect(ui->btnNewCluster, &QPushButton::clicked, this, &ClusterManagerDlg::new_cluster);
    connect(ui->btnNewStation, &QPushButton::clicked, this, &ClusterManagerDlg::new_station);
    connect(ui->btnNewAudioServer, &QPushButton::clicked, this, &ClusterManagerDlg::new_server);
    connect(ui->btnSave, &QPushButton::clicked, this, &ClusterManagerDlg::save_data);

    toggle_new_station_button(false);

    m_cluster_config = std::make_unique<ClusterConfiguration>();

    ClusterManager::ConfigItem root_item;
    root_item.config_item_type = ConfigItemType::Root;
    root_item.node_type = NodeType::Group;
    root_item.node_text = "Cluster Management (Server Name)";

    QMap<QString, QVariant> item_data;
    auto uuid = QUuid().createUuid();
    QString uuid_str = uuid.toString(QUuid::WithoutBraces).left(8);
    item_data["type"] = static_cast<int>(root_item.config_item_type);
    item_data["uuid"] = uuid_str;

    m_root_node = std::make_unique<RootNode>(root_item);
    m_root_node->set_config_item_type(root_item.config_item_type);
    m_root_node->setText(0, root_item.node_text);
    m_root_node->setData(0, Qt::UserRole, item_data);
    ui->treeWidget->addTopLevelItem(m_root_node.get());

    m_cluster_config->cluster_nodes[uuid_str] = m_root_node.get();

    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, &ClusterManagerDlg::on_item_clicked);
    connect(ui->btnPrintTree, &QPushButton::clicked, this, &ClusterManagerDlg::print_tree);

    m_cluster_config->cluster_nodes[uuid_str] = m_root_node.get();

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &ClusterManagerDlg::context_menu_requested);

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
    auto cluster = std::make_unique<ClusterManager::Cluster>();

    std::unique_ptr<ClusterForm> cform =
            std::make_unique<ClusterForm>(cluster.get());

    if (cform->exec() > 0){
        ClusterManager::ConfigItem cluster_item;
        auto item_type = ConfigItemType::Cluster;
        cluster_item.config_item_type = item_type;
        cluster_item.node_type = NodeType::Group;
        cluster_item.node_text = ClusterManager::item_type_to_string(item_type)+": "+
                stoq(cluster->cluster_name()->value());

        EntityDataModel edm;
        int id = edm.createEntityDB(*cluster);
        cluster_item.id = id;

        make_node<std::unique_ptr<ClusterManager::Cluster>, ClusterNode, RootNode>(
                    std::move(cluster), m_root_node.get(), cluster_item);

        for (auto&[key, node_t]: m_root_node->child_nodes()){
            auto&[n_type, node] = node_t;
            qDebug() << "Parent ID: "<< std::any_cast<ClusterNode*>(node)->parent_id();
            qDebug() << "CHILD TYPE: "<< ClusterManager::item_type_to_string(n_type);

        }


    }

}

void ClusterManagerDlg::new_station()
{
    auto station = std::make_unique<ClusterManager::Station>();

    std::unique_ptr<StationForm> sform = std::make_unique<StationForm>(station.get());

    if (sform->exec() > 0 ){
        ClusterManager::ConfigItem station_item;
        auto item_type = ConfigItemType::Station;
        station_item.config_item_type = item_type;
        station_item.node_type = NodeType::Leaf;
        station_item.node_text = ClusterManager::item_type_to_string(item_type)+": "+
                    stoq(station->station_name()->value());

        EntityDataModel edm;
        station->set_cluster(m_current_cluster_node->id());
        int id = edm.createEntityDB(*station);
        station_item.id = id;

        make_node<std::unique_ptr<ClusterManager::Station>, StationNode, ClusterNode>(
                    std::move(station), m_current_cluster_node, station_item);

    }

}

void ClusterManagerDlg::new_server()
{
    auto server = std::make_unique<ClusterManager::Server>();

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
        server->set_cluster(m_current_cluster_node->id());
        int id = edm.createEntityDB(*server);
        server_item.id = id;

        make_node<std::unique_ptr<ClusterManager::Server>, ServerNode, ClusterNode>(
                    std::move(server), m_current_cluster_node, server_item );
    }

}

void ClusterManagerDlg::delete_cluster()
{
    delete_node<ClusterNode, RootNode>(m_current_cluster_node, m_root_node.get());
}


void ClusterManagerDlg::delete_server()
{
    delete_node<ServerNode, ClusterNode>(m_current_audio_server_node, m_current_cluster_node);

}

void ClusterManagerDlg::delete_disk()
{
    delete_node<DiskNode, ServerNode>(m_current_disk_node, m_current_audio_server_node);
}

void ClusterManagerDlg::delete_folder()
{
    delete_node<AudioFolderNode, DiskNode>(m_current_folder_node, m_current_disk_node);
}



void ClusterManagerDlg::new_disk(ClusterManager::Server* server)
{
    auto storage_disk = std::make_unique<ClusterManager::StorageDisk>();

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

        make_node<std::unique_ptr<ClusterManager::StorageDisk>, DiskNode, ServerNode>(
                    std::move(storage_disk), m_current_audio_server_node, disk_item );

    }

}

void ClusterManagerDlg::new_audio_folder(ClusterManager::StorageDisk* storage_disk)
{

    auto audio_folder = std::make_unique<ClusterManager::AudioFolder>();

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

        make_node<std::unique_ptr<ClusterManager::AudioFolder>, AudioFolderNode, DiskNode>(
                    std::move(audio_folder), m_current_disk_node, folder_item);

    }

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
     case static_cast<int>(ConfigItemType::Root):
     {
       auto root_node = get_cluster_node<RootNode>(uuid);

       if (m_root_context_menu != nullptr){
           m_root_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
           return;
       }

       show_root_context_menu(uuid, pos);
       break;

     }

     case static_cast<int>(ConfigItemType::Cluster):
     {
        m_current_cluster_node = get_cluster_node<ClusterNode>(uuid);

       if (m_cluster_context_menu != nullptr){
           m_cluster_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
          return;
       }

       show_cluster_context_menu(uuid, pos);
       break;

     }

    case static_cast<int>(ConfigItemType::AudioServer):
   {
       m_current_audio_server_node = get_cluster_node<ServerNode>(uuid);

       if (m_audio_server_context_menu != nullptr){
           m_audio_server_context_menu->popup(ui->treeWidget->mapToGlobal(pos));
           return;
       }

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
    if (m_act_user == nullptr )
       m_act_user = std::make_unique<QAction>("New User...");
    if (m_act_app == nullptr)
       m_act_app = std::make_unique<QAction>("New Application...");

    connect(m_act_cluster.get(), &QAction::triggered, this, [this](){ new_cluster(); });

    m_root_context_menu->addAction(m_act_cluster.get());
    m_root_context_menu->addAction(m_act_user.get());
    m_root_context_menu->addAction(m_act_app.get());

    m_root_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_cluster_context_menu(QString node_id, QPoint pos)
{
    m_cluster_context_menu = std::make_unique<QMenu>();

    if (m_act_station == nullptr)
       m_act_station = std::make_unique<QAction>("New Station...");

    if (m_act_server == nullptr)
       m_act_server = std::make_unique<QAction>("New Server...");

    if (m_act_delete_cluster == nullptr)
        m_act_delete_cluster = std::make_unique<QAction>("Delete Cluster");

    connect(m_act_station.get(), &QAction::triggered, this, &ClusterManagerDlg::new_station);
    connect(m_act_server.get(), &QAction::triggered, this, &ClusterManagerDlg::new_server);
    connect(m_act_delete_cluster.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_cluster);

    m_cluster_context_menu->addAction(m_act_station.get());
    m_cluster_context_menu->addAction(m_act_server.get());
    m_cluster_context_menu->addSeparator();
    m_cluster_context_menu->addAction(m_act_delete_cluster.get());

    m_cluster_context_menu->popup(ui->treeWidget->mapToGlobal(pos));

}

void ClusterManagerDlg::show_audio_server_context_menu(QString node_id, QPoint pos)
{
    m_audio_server_context_menu = std::make_unique<QMenu>();

     auto node = get_cluster_node<ServerNode>(node_id);

    if (node == nullptr)
        return;

    if (m_act_audio_server == nullptr)
       m_act_audio_server = std::make_unique<QAction>("New Disk...");
    if (m_act_delete_server == nullptr)
        m_act_delete_server = std::make_unique<QAction>("Delete Server");

    ClusterManager::Server* server = node->node_entity();

    connect(m_act_audio_server.get(), &QAction::triggered, this, [server, this](){
        new_disk(server);
    });

    connect(m_act_delete_server.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_server);

    m_audio_server_context_menu->addAction(m_act_audio_server.get());
    m_audio_server_context_menu->addSeparator();
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
    if (m_act_delete_disk == nullptr)
        m_act_delete_disk = std::make_unique<QAction>("Delete Disk");

    //ClusterManager::StorageDisk* storage_disk = m_current_disk_node->node_entity();
    ClusterManager::StorageDisk* storage_disk = node->node_entity();

    qDebug() << "SHOW DISK CONTENT:: "<< stoq(storage_disk->disk_name()->value());

    connect(m_act_folder.get(), &QAction::triggered, this, [storage_disk, this](){
        new_audio_folder(storage_disk);
    });
    connect(m_act_delete_disk.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_disk);

    m_disk_context_menu->addAction(m_act_folder.get());
    m_disk_context_menu->addSeparator();
    m_disk_context_menu->addAction(m_act_delete_disk.get());
}

void ClusterManagerDlg::show_folder_context_menu(QString node_id, QPoint pos)
{
    m_folder_context_menu = std::make_unique<QMenu>();
    auto node = get_cluster_node<AudioFolderNode>(node_id);
    if (node == nullptr)
        return;

    if (m_act_delete_folder == nullptr)
        m_act_delete_folder = std::make_unique<QAction>("Delete Folder");

    ClusterManager::AudioFolder* folder = node->node_entity();
    connect(m_act_delete_folder.get(), &QAction::triggered, this, &ClusterManagerDlg::delete_folder);

    m_folder_context_menu->addAction(m_act_delete_folder.get());

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
