#ifndef ENTITYFIELDS_H
#define ENTITYFIELDS_H

#include<string>
#include<variant>
#include<QVariant>

using FieldValueType = std::variant<std::string, int>;


class Field{
    public:
        Field();
        Field(const std::string aName, std::string aLabel);
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

        void setDisplayName(const std::string dispName);
        std::string displayName() const;

        void setMandatory(bool value);
        bool mandatory() const;

        virtual std::string valueToString()const = 0;
        virtual std::string dbValueFormatter()=0;

        virtual void stringToValue(std::string val)=0;

        virtual QVariant value()=0;

    private:
        std::string mFieldName;
        std::string mFieldLabel;
        std::string mDBColumnName;
        bool mVisible;
        bool mFormOnly;
        bool mSearchable;
        std::string mDisplayName;
        bool mMandatory;
};

class IntegerField : public Field{
    public:
        typedef int type;
        IntegerField();
        ~IntegerField() override;
        IntegerField(std::string aName, std::string aLabel);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(int val);

        void stringToValue(std::string val) override;

        QVariant value() override;

    private:
        int mValue;
};

class StringField : public Field{
    public:
        typedef std::string type;
        StringField();
        ~StringField() override;
        StringField(std::string aName, std::string aLabel);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        QVariant value() override;
    private:
        std::string mValue;
};

class TextField :public Field{
    public:
        typedef std::string type;
        TextField();
        ~TextField() override;
        TextField(std::string aName, std::string aLabel);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        QVariant value() override;

    private:
        std::string mValue;
};

using IF = std::tuple<IntegerField, int>;
using SF = std::tuple<StringField, std::string>;

#endif // ENTITYFIELDS_H
