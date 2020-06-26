#ifndef ENTITYFIELDS_H
#define ENTITYFIELDS_H

#include<string>
#include<variant>
#include<QVariant>
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

        void setSearchable(bool value);
        bool searchable();

        virtual std::string displayName() const = 0;

        void setMandatory(bool value);
        bool mandatory() const;

        virtual std::string valueToString()const = 0;
        virtual std::string dbValueFormatter()=0;

        virtual void stringToValue(std::string val)=0;

    private:
        std::string mFieldName;
        std::string mFieldLabel;
        std::string mDBColumnName;
        std::string mDisplayName;
        bool mVisible;
        bool mFormOnly;
        bool mSearchable;
        bool mMandatory;
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
    private:
        bool mValue;
};

class StringField : public Field{
    public:
        typedef std::string type;

        StringField();
        ~StringField() override;
        StringField(const std::string aName, const std::string aLabel);

        StringField(const StringField& sf);
        StringField& operator=(const StringField& sf);

        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        std::string value();

        std::string displayName() const override;
    private:
        std::string mValue;
};

class TextField :public Field{
    public:
        typedef std::string type;
        TextField();
        ~TextField() override;
        TextField(const std::string aName, const std::string aLabel);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        std::string value();

        std::string displayName() const override;
    private:
        std::string mValue;
};

class ForeignKeyField : public Field {
public:
        ForeignKeyField();
        ForeignKeyField(const std::string aName, const std::string aLabel,
                        BaseEntity* fkEntity, const std::string displayField);
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

        void cacheData();
        std::size_t cacheCount();

        EntityDataModel* dataModel() const;

    private:
        int mValue;
        int mIndex;
        std::string mCurrText;
        std::string mDisplayField;
        static std::map<std::string, std::unique_ptr<EntityDataModel>> lookups;
};


using IF = std::tuple<IntegerField, int>;
using SF = std::tuple<StringField, std::string>;

#endif // ENTITYFIELDS_H
