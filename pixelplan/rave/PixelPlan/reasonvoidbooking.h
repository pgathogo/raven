#ifndef REASONVOIDBOOKING_H
#define REASONVOIDBOOKING_H

#include <string>

#include "../../../rave/framework/baseentity.h"

namespace PIXELPLAN
{

    class ReasonVoidBooking : public BaseEntity
    {
    public:
        ReasonVoidBooking();
        ~ReasonVoidBooking() override;

        StringField* reason();
        void set_reason(const std::string);

        TextField* description();
        void set_description(const std::string);


        std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::string searchColumn() const override;

        void populateEntity() override;
        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;

    private:
        StringField* m_reason;
        TextField* m_description;
        QStringList m_header;
        std::string m_table_name;
    };

}

#endif
