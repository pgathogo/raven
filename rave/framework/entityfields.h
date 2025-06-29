#ifndef ENTITYFIELDS_H
#define ENTITYFIELDS_H

#include<string>
#include<variant>
#include<QVariant>
#include<QDate>
#include<memory>

using FieldValueType = std::variant<std::string, int>;

class FormField;
class BaseEntity;
class EntityDataModel;

class Field{
    public:
        Field();
        Field(const std::string aName, const std::string aLabel);
        virtual ~Field();
        std::string fieldName() const;
        std::string fieldLabel() const;
        std::string dbColumnName() const;
        void setDBColumnName(std::string aName);
        void setName(const std::string aName);
        void setLabel(const std::string aLabel);

        bool visible() const;
        void setVisible(bool value);

        bool formOnly() const;
        void setFormOnly(bool value);

        bool readOnly() const;
        void setReadOnly(bool value);

        void setSearchable(bool value);
        bool searchable();

        void set_nullable(bool value);
        bool nullable();

        void set_parent(BaseEntity*);
        BaseEntity* parent() const;

        virtual std::string displayName() const = 0;

        int display_width();
        void set_display_width(int);

        void setMandatory(bool value);
        bool mandatory() const;

        virtual std::string valueToString()const = 0;
        virtual std::string dbValueFormatter()=0;

        virtual void stringToValue(std::string val)=0;

        virtual std::string field_type();


        template<typename T>
        std::string qualified_column_name()
        {
            auto actual_parent = dynamic_cast<const T*>(parent());
            return actual_parent->tableName()+"."+this->dbColumnName();

        }

    private:
        std::string mFieldName;
        std::string mFieldLabel;
        std::string mDBColumnName;
        std::string mDisplayName;
        bool mVisible;
        bool mFormOnly;  // for display only
        bool mSearchable;
        bool mMandatory;
        bool mReadOnly;  // Read from db but no write
        bool m_nullable;
        int m_display_width;
        BaseEntity* m_parent;
};

class IntegerField : public Field{
    public:
        typedef int type;
        IntegerField();
        ~IntegerField() override;
        IntegerField(const std::string aName, const std::string aLabel);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(int val);

        void stringToValue(std::string val) override;

        int value();
        std::string displayName() const override;
        std::string field_type() override;

    private:
        int mValue;
};

class DecimalField : public Field{
    public:
        DecimalField();
        DecimalField(const std::string aName, const std::string aLabel);
        ~DecimalField() override;
        std::string valueToString() const override;
        std::string dbValueFormatter() override;
        void stringToValue(const std::string val) override;
        void setValue(double val);
        double value();
        std::string displayName() const override;
        std::string field_type() override;
    private:
        double mValue;
};

class BooleanField : public Field{
    public:
        BooleanField();
        BooleanField(const std::string aName, const std::string aLabel);
        ~BooleanField() override;
        std::string valueToString() const override;
        std::string dbValueFormatter() override;
        void stringToValue(const std::string val) override;
        void setValue(bool val);
        bool value() const ;
        std::string displayName() const override;
        std::string field_type() override;
    private:
        bool mValue;
};

class StringField : public Field{
    public:
        typedef std::string type;

        StringField();
        StringField(const std::string aName, const std::string aLabel);
        ~StringField() override;

        StringField(const StringField& sf);
        StringField& operator=(const StringField& sf);

        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        std::string value();

        std::string displayName() const override;
        std::string field_type() override;

        std::string value_tolower();
        QString to_qstring();
    private:
        std::string mValue;
};

class TextField :public Field{
    public:
        typedef std::string type;
        TextField();
        TextField(const std::string aName, const std::string aLabel);
        ~TextField() override;
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        std::string value();

        std::string displayName() const override;
        std::string field_type() override;
        QString to_qstring();
    private:
        std::string mValue;
};

class ForeignKeyField : public Field {
public:
        ForeignKeyField();
        ForeignKeyField(const std::string aName, const std::string aLabel,
                        std::unique_ptr<BaseEntity> fkEntity,
                        const std::string displayField,
                        const std::string filter = "");
        ~ForeignKeyField() override;

        std::string valueToString() const override;
        std::string dbValueFormatter() override;
        void stringToValue(std::string val) override;

        std::string displayName() const override;

        void setValue(int val);
        int value() const ;

        void setIndex(int i);
        int index() const;

        void setCurrText(std::string txt);
        std::string currText() const;

        void setCurrentText();

        void set_fk_entity(std::unique_ptr<BaseEntity>);

        void cacheData();
        std::size_t cacheCount();

        EntityDataModel* dataModel() const;

        std::string sourceTableName() const;

        BaseEntity* entity();
        BaseEntity* currentEntity();

        BaseEntity* fk_entity();
        std::unique_ptr<BaseEntity> const& fk_entity(const std::string);

        BaseEntity* unique_fk_entity();
        std::string field_type() override;

        std::unique_ptr<BaseEntity> const& data_model_entity() const;

    private:
        int mValue;
        int mIndex;
        std::string mCurrText;
        std::string mDisplayField;
        BaseEntity* m_fk_entity;
        std::unique_ptr<BaseEntity> m_unique_fk_entity;
        static std::map<std::string, std::unique_ptr<EntityDataModel>> lookups;
};

class DateField : public Field{
public:
    DateField();
    DateField(const std::string aName, const std::string aLabel);
    ~DateField() override;

    std::string valueToString() const override;
    std::string dbValueFormatter() override;

    void setValue(QDate);
    void stringToValue(std::string val) override;
    QDate value();

    std::string displayName() const override;
    std::string field_type() override;
private:
    QDate mValue;
};

class DateTimeField : public Field{
public:
    DateTimeField();
    DateTimeField(const std::string aName, const std::string aLabel);
    ~DateTimeField() override;

    std::string valueToString() const override;
    std::string dbValueFormatter() override;

    void setValue(QDateTime);
    void stringToValue(std::string val) override;
    QDateTime value();

    std::string displayName() const override;
    std::string field_type() override;
private:
    QDateTime mValue;
};

class TimeField : public Field{
public:
    TimeField();
    TimeField(const std::string aName, const std::string aLabel);
    ~TimeField() override;

    std::string valueToString() const override;
    std::string dbValueFormatter() override;

    void setValue(QTime);
    void stringToValue(std::string val) override;
    QTime value();
    std::string displayName() const override;
    std::string field_type() override;
private:
    QTime mValue;
};

using IF = std::tuple<IntegerField, int>;
using SF = std::tuple<StringField, std::string>;

#endif // ENTITYFIELDS_H
