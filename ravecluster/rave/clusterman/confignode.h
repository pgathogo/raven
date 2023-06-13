#ifndef CONFIGNODE_H
#define CONFIGNODE_H

#include <any>
#include <QObject>
#include <QTreeWidgetItem>

enum class ConfigItemType{
    Root=0,
    Cluster,
    Station,
    StationGroup,
    ServerGroup,
    AudioServer,
    DBServer,
    Disk,
    AudioFolder,
    ClusterGroup,
    Role,
    User,
    RoleAndUserGroup,
    RoleGroup,
    UserGroup,
    Application,
    ApplicationGroup

};

enum class NodeType{
    Group=0,
    Leaf
};


namespace ClusterManager
{

    struct ConfigItem{
       ConfigItemType config_item_type;
       NodeType node_type;
       QString node_text;
       QString node_icon_file;
       int id{-1};
    };

    inline QString node_type_to_string(NodeType node_type)
    {
        switch(node_type){
        case NodeType::Group:
            return "Group";
        case NodeType::Leaf:
            return "Leaf";
        default:
            return "Unknown Node Type";
        }
    }

    inline QString item_type_to_string(ConfigItemType item_type)
    {
        switch(item_type){
        case ConfigItemType::Root : {
            return "Root";
        }
        case ConfigItemType::Cluster: {
            return "Cluster";
        }
        case ConfigItemType::AudioServer: {
           return "AudioServer";
        }
        case ConfigItemType::DBServer: {
            return "DBServer";
        }
        case ConfigItemType::StationGroup: {
            return "StationGroup";
        }
        case ConfigItemType::Station: {
            return "Station";
        }
        case ConfigItemType::Disk: {
            return "Disk";
        }
        case ConfigItemType::ServerGroup:{
            return "ServerGroup";
        }
        case ConfigItemType::AudioFolder: {
            return "Folder";
        }
        case ConfigItemType::ClusterGroup: {
            return "";
        }
        case ConfigItemType::Role: {
            return "Role";
        }
        case ConfigItemType::User: {
            return "User";
        }
        case ConfigItemType::RoleAndUserGroup: {
            return "";
        }
        case ConfigItemType::RoleGroup: {
            return "";
        }
        case ConfigItemType::UserGroup: {
            return "";
        }
        case ConfigItemType::Application: {
            return "Application";
        }
        case ConfigItemType::ApplicationGroup: {
            return "";
        }
        default:{
            return "Unknow Item Type";
        }

      };
    }

    class RootConfigNode : public QObject, public QTreeWidgetItem { };
    class ClusterGroupConfig: public QObject, public QTreeWidgetItem {};

    class RoleAndUserConfig : public QObject, public QTreeWidgetItem {};
    class RoleConfig: public QObject, public QTreeWidgetItem {};
    class UserGroupConfig: public QObject, public QTreeWidgetItem {};

    class AppGroupConfig: public QObject, public QTreeWidgetItem {};

    class UserConfig: public QObject, public QTreeWidgetItem{};
    class ApplicationConfig: public QObject, public QTreeWidgetItem{};

    class StationGroupConfig: public QObject, public QTreeWidgetItem{};
    class ServerGroupConfig: public QObject, public QTreeWidgetItem{};

    class BaseNode: public QObject, public QTreeWidgetItem {};

    template<typename T>
    class ConfigNode :   public QObject, public QTreeWidgetItem
    {
    public:
        ConfigNode(ConfigItem);
        int id();
        int temp_id();
        QString uuid();
        int parent_id();
        NodeType node_type();
        void set_id(int);
        void set_temp_id(int);
        void set_node_entity(std::shared_ptr<T>);
        void set_config_item_type(ConfigItemType ci_type);
        void set_node_type(NodeType);
        T* node_entity();  // FIXME: Replace this with node_entity_shared
        std::shared_ptr<T> node_entity_shared();
        void set_parent_id(int parent_id);
        void setText(int column, const QString& aText);
        void addChild(QTreeWidgetItem* child);
        void set_uuid(QString);
        void add_child(QString, ConfigItemType, std::any);
        void update_node_text(QString);
        std::map<QString, std::tuple<ConfigItemType, std::any>> child_nodes();
        std::map<QString, std::tuple<ConfigItemType, std::any>>& child_nodes_by_ref();

        //void add_group(std::unique_ptr<QTreeWidgetItem>);
//        template<typename T2>
//        void add_group(QString, std::any);

    private:
        int m_id{-1}; // from table id else -1
        int m_temp_id {-1}; // t_+max(temp_id)+1
        int m_parent_id{-1};
        QString m_uuid;
        ConfigItemType m_config_item_type;
        std::shared_ptr<T> m_node_entity;
        NodeType m_node_type;
        std::map<QString, std::tuple<ConfigItemType, std::any>> m_child_nodes;
        std::map<QString, std::any>m_group_nodes;
        //std::vector<std::unique_ptr<QTreeWidgetItem>>m_group_nodes;
      };

        template<typename T>
        ConfigNode<T>::ConfigNode(ConfigItem config_item)
             :m_config_item_type{config_item.config_item_type}
             ,m_node_type{config_item.node_type}
        {
            m_node_entity = std::make_shared<T>();
        }

        template<typename T>
        int ConfigNode<T>::id()
        {
            return m_id;
        }
        template<typename T>
        int ConfigNode<T>::temp_id()
        {
            return m_temp_id;
        }

        template<typename T>
        QString ConfigNode<T>::uuid()
        {
            return m_uuid;
        }
        template<typename T>
        int ConfigNode<T>::parent_id()
        {
            return m_parent_id;
        }
        template<typename T>
        NodeType ConfigNode<T>::node_type()
        {
            return m_node_type;
        }

        template<typename T>
        void ConfigNode<T>::set_id(int id)
        {
            m_id = id;
        }

        template<typename T>
        void ConfigNode<T>::set_temp_id(int t_id)
        {
            m_temp_id = t_id;
        }

        template<typename T>
        void ConfigNode<T>::set_node_entity(std::shared_ptr<T> node_item)
        {
            m_node_entity = std::move(node_item);
        }

        template<typename T>
        void ConfigNode<T>::set_config_item_type(ConfigItemType ci_type)
        {
            m_config_item_type = ci_type;
        }

        template<typename T>
        T* ConfigNode<T>::node_entity()
        {
            return m_node_entity.get();
        }

        template<typename T>
        std::shared_ptr<T> ConfigNode<T>::node_entity_shared()
        {
            return m_node_entity;
        }

        template<typename T>
        void ConfigNode<T>::set_parent_id(int parent_id)
        {
            m_parent_id = parent_id;
        }

        template<typename T>
        void ConfigNode<T>::setText(int column, const QString& aText)
        {
            QTreeWidgetItem::setText(column, aText);
        }

        template<typename T>
        void ConfigNode<T>::add_child(QString key, ConfigItemType ctype, std::any value)
        {
            //qDebug() << "add_child: "<< ClusterManager::item_type_to_string(m_config_item_type);
            m_child_nodes[key] = std::make_tuple(ctype, value);
        }

        template<typename T>
        std::map<QString, std::tuple<ConfigItemType, std::any>> ConfigNode<T>::child_nodes()
        {
            return m_child_nodes;
        }

        template<typename T>
        std::map<QString, std::tuple<ConfigItemType, std::any>>& ConfigNode<T>::child_nodes_by_ref()
        {
            return m_child_nodes;
        }

//        template<typename T>
//        void ConfigNode<T>::add_group(QString key, std::any value)
//        {
//            m_group_nodes[key] = value;

//        }

        template<typename T>
        void ConfigNode<T>::addChild(QTreeWidgetItem* child)
        {
            QTreeWidgetItem::addChild(child);
        }

        template<typename T>
        void ConfigNode<T>::set_uuid(QString uid)
        {
            m_uuid = uid;
        }

        template<typename T>
        void ConfigNode<T>::set_node_type(NodeType n_type)
        {
            m_node_type = n_type;
        }
        template<typename T>
        void ConfigNode<T>::update_node_text(QString text)
        {
            setText(0, text);
        }

}

#endif // CONFIGNODE_H
