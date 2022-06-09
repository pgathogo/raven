#pragma once

#include "../framework/baseentity.h"

class AudioFolder : public BaseEntity
{
public:
    AudioFolder();
    ~AudioFolder() override;

    StringField* folder_name() const;
    IntegerField* parent_id() const;

    void set_folder_name(std::string);
    void set_parent_id(int);

    std::string tableName() const override;
    void setTableName(const std::string) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

private:
    StringField* m_folder_name;
    IntegerField* m_parent_id;

    QStringList m_header;
    std::string m_table_name;

};
