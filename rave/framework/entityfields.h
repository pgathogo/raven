#ifndef ENTITYFIELDS_H
#define ENTITYFIELDS_H

#include<string>
#include<variant>

using FieldValueType = std::variant<std::string>;

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
        void setValue(const FieldValueType aValue);
        void setStringValue(const std::string aValue);
        bool visible() const;
        void setVisible(bool value);
        bool formOnly() const;
        void setFormOnly(bool value);

        virtual std::string valueToString() const;
        virtual FieldValueType value() const;

    protected:
        FieldValueType getValue() const;
    private:
        std::string mFieldName;
        std::string mFieldLabel;
        std::string mDBColumnName;
        bool mVisible;
        bool mFormOnly;
        FieldValueType mValue;
};

class IntegerField : public Field{
    public:
        IntegerField();
        ~IntegerField() override;
        IntegerField(std::string aName, std::string aLabel);
        FieldValueType value() const override;
        std::string valueToString() const override;
};

class StringField : public Field{
    public:
        StringField();
        ~StringField() override;
        StringField(std::string aName, std::string aLabel);
        FieldValueType value() const override;
        std::string valueToString() const override;
    private:
        std::string testVal;
};

class TextField :public Field{
    public:
        TextField();
        ~TextField() override;
        TextField(std::string aName, std::string aLabel);
        FieldValueType value() const override;
        std::string valueToString() const override;
};

using IF = std::tuple<IntegerField, int>;
using SF = std::tuple<StringField, std::string>;

#endif // ENTITYFIELDS_H
