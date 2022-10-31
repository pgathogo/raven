#ifndef CONFIGNODE_H
#define CONFIGNODE_H

#include <any>
#include <QObject>
#include <QTreeWidgetItem>

enum class ConfigItemType{
    Root=0,
    Cluster,
    Station,
    AudioServer,
    DBServer,
    Disk,
    AudioFolder,
    ClusterGroup,
    UserGroup,
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
       int id{-1};
    };

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
        case ConfigItemType::Station: {
            return "Station";
        }
        case ConfigItemType::Disk: {
            return "Disk";
        }
        case ConfigItemType::AudioFolder: {
            return "Folder";
        }
        case ConfigItemType::ClusterGroup: {
            return "";
        }
        case ConfigItemType::UserGroup: {
            return "";
        }
        case ConfigItemType::ApplicationGroup: {
            return "";
        }
        default:{
            return "Unknow Item Type";
            break;
        }

      };
    }

    class RootConfigNode : public QObject, public QTreeWidgetItem { };
    class ClusterGroupConfig: public QObject, public QTreeWidgetItem {};
    class UserGroupConfig: public QObject, public QTreeWidgetItem {};
    class AppGroupConfig: public QObject, public QTreeWidgetItem {};

    class BaseNode: public QObject, public QTreeWidgetItem
    {
    };

    template<typename T>
    class ConfigNode :   public QObject, public QTreeWidgetItem
    {
    public:
        ConfigNode(ConfigItem);
        int id();
        int temp_id();
        QString uuid();
        int parent_id();
        void set_id(int);
        void set_temp_id(int);
        void set_node_entity(std::unique_ptr<T>);
        void set_config_item_type(ConfigItemType ci_type);
        //std::unique_ptr<T> const& node_entity();
        T* node_entity();
        void set_parent_id(int parent_id);
        void setText(int column, const QString& aText);
        void addChild(QTreeWidgetItem* child);
        void set_uuid(QString);
        void add_child(QString, ConfigItemType, std::any);
        std::map<QString, std::tuple<ConfigItemType, std::any>> child_nodes();
        std::map<QString, std::tuple<ConfigItemType, std::any>>& child_nodes_by_ref();

    private:
        int m_id{-1}; // from table id else -1
        int m_temp_id {-1}; // t_+max(temp_id)+1
        int m_parent_id{-1};
        QString m_uuid;
        ConfigItemType m_config_item_type;
        std::unique_ptr<T> m_node_entity;
        NodeType m_node_type;
        std::map<QString, std::tuple<ConfigItemType, std::any>> m_child_nodes;
//        std::vector<QTreeWidgetItem*> m_child_nodes;
    };

        template<typename T>
        ConfigNode<T>::ConfigNode(ConfigItem config_item)
             :m_config_item_type{config_item.config_item_type}
             ,m_node_type{config_item.node_type}
        {
            m_node_entity = std::make_unique<T>();
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
        void ConfigNode<T>::set_node_entity(std::unique_ptr<T> node_item)
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

}

#endif // CONFIGNODE_H
