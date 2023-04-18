#ifndef CLUSTER_H
#define CLUSTER_H


#include "../../../rave/framework/baseentity.h"

namespace ClusterManager
{
    class Cluster : public BaseEntity
    {
    public:
        Cluster();
        ~Cluster() override;

        StringField* cluster_name() const;
        TextField* notes() const;

        void set_cluster_name(std::string);
        void set_notes(std::string);

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::unique_ptr<BaseEntity> mapFields(StringMap*) override;
        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string searchColumn() const override;
        void populateEntity() override;
        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;

     private:
        StringField* m_cluster_name;
        TextField* m_notes;

        QStringList m_header;
        std::string m_table_name;
    };
}

#endif // CLUSTER_H
