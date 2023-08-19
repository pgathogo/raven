#ifndef BREAKLAYOUTLINE_H
#define BREAKLAYOUTLINE_H

#include <QTime>

#include "../framework/baseentity.h"

template<typename T>
class ChoiceField;


class BreakLayoutLine : public BaseEntity
{
public:
    BreakLayoutLine();
    ~BreakLayoutLine() override;

    ForeignKeyField* breakLayout() const;
    void setBreakLayout(int i);

    IntegerField* weekDay() const;
    void setWeekDay(int wd);

    TimeField* breakTime() const;
    void setBreakTime(QTime bt);

    IntegerField* breakHour() const;
    void setBreakHour(int hr);

    IntegerField* duration() const;
    void setDuration(int dur);

    IntegerField* maxSpots() const;
    void setMaxSpots(int ms);

    IntegerField* rowId() const;
    void setRowId(int id);

    ChoiceField<std::string>* break_fill_method() const;
    void set_break_fill_method(const std::string);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;

    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

private:
    ForeignKeyField* mBreakLayout;
    IntegerField* mWeekDay;
    TimeField* mBreakTime;
    IntegerField* mBreakHour;
    IntegerField* mDuration;
    IntegerField* mMaxSpots;
    IntegerField* mRowId;

    ChoiceField<std::string>* m_break_fill_method;

    QStringList mHeader;
    std::string mTableName;
};

#endif // BREAKLAYOUTLINE_H
