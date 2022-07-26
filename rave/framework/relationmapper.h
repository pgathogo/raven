#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include "entityfields.h"

using Nullable = bool;
using QueryResults = std::map<int, std::map<std::string, std::vector<std::tuple<std::string, std::string>>>>;
using MappedEntity = std::map<int, std::map<std::string, std::vector<std::unique_ptr<BaseEntity>>>>;

class BaseEntity;
class EntityDataModel;

namespace FRAMEWORK{

    class RelationMapper
    {
    public:
        RelationMapper();
        RelationMapper(EntityDataModel*);
        ~RelationMapper();

        std::string main_table();
        //std::vector<std::string>& related_tables();
        std::map<std::string, const std::type_info*>& related_tables();
        std::map<std::string, std::tuple<BaseEntity*, Nullable>>& foreign_key_fields();
        std::vector<std::string>& query_fields();

        void set_main_entity(BaseEntity*);
        void set_main_table(std::string);
        void append_related_tables(std::string, const std::type_info*);

        void append_foreign_key_fields(std::string,  std::tuple<BaseEntity*, Nullable>);

        void append_query_fields(std::string);
        void fetch_fk_fields(BaseEntity*);

        void clear_query_fields();
        void clear_related_tables();
        void clear_foreign_key_fields();
        void clear_query_results();
        void clear_mapped_entities();

        void print_related_tables();
        void print_foreign_key_fields();
        void print_query_fields();
        void print_filter_cache();

        void build_relation_tree();
        void inner_relation_tree(int);
        void print_relation_tree();


        RelationMapper& prepare_query();
        std::string query() const;

        std::string fk_field(std::string);
        std::vector<std::string> join_statements();
        std::string join_statements_asString();
        std::string columns();

        void map_data();
        void gather_data();
        void set_model(EntityDataModel*);
        void append_query_results(int,const std::string, const std::string, const std::string);
        void print_query_results();
        void map_data_object();
        void map_data_main_entity();
        void print_data_objects(const std::string);
        void data_to_object(int, const std::string, BaseEntity*, const std::vector<std::tuple<std::string, std::string>>&);
        void print_mapped_entities();
        MappedEntity const& mapped_entities() const;
        void print_join_statements_chain();

        template<typename T>
        struct is_string :
                public std::disjunction<
                std::is_same<char*, typename std::decay<T>::type>,
                std::is_same<const char*, typename std::decay<T>::type>,
                std::is_same<std::string, typename std::decay<T>::type>
                >{
        };

        template<typename T, typename... Types>
        void make_filter(T firstArg, Types... args)
        {
            auto [column, op, value] = firstArg;

            std::string filter = column;

            if constexpr(std::is_integral_v<decltype(value)> &&
                    !std::is_same_v<decltype(value), bool>){
                //filter +=" = ";
                filter += op;
                filter += std::to_string(value);
            }else if constexpr(std::is_integral_v<decltype(value)> &&
                               std::is_same_v<decltype(value), bool>){
                //filter += " = ";
                filter += op;
                switch (value){
                 case 0:
                    filter += "false";
                    break;
                 case 1:
                    filter += "true";
                    break;
                default:
                    filter += "false";
                }
            }else if constexpr(is_string<decltype(value)>::value){
                filter = "lower("+filter+")";
                if (str_tolower(op) == "like"){
                    filter += " LIKE ";
                    std::string str_value = value;
    //                for(auto s : str_value)
    //                    std::tolower(s);
                    str_value = str_tolower(str_value);
                    filter += "'%"+str_value+"%'";
                }else{
                    filter += op;
                    filter += value;
                }

            }else if constexpr(std::is_same_v<decltype(value), QDate>){
                //filter += " = ";
                filter += op;
                std::string str_value = value.toString("yyyy-MM-dd").toStdString();
                for(auto s : str_value)
                    std::tolower(s);
                filter += "'"+str_value+"'";
            }

            m_filter_cache.push_back(filter);

            if constexpr(sizeof...(args) > 0){
                    make_filter(args...);
            }
        }

        std::string str_tolower(std::string s){
            std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c);});
            return s;
        }

        template<typename T, typename...Types>
        RelationMapper* filter(T firstArg, Types... args)
        {
            m_filter_cache.clear();
            make_filter(firstArg, std::forward<Types>(args)...);

            size_t n = 1;
            std::string sf{""};
            for (auto f : m_filter_cache){
                sf += f;
                if ( n < m_filter_cache.size())
                    sf += " and ";
                ++n;
            }

            m_filter = " WHERE "+sf;
            m_query += m_filter;

            return this;
        }

        template<typename T, typename...Types>
        void make_chain_join_statments(T firstArg, Types...args)
        {
            auto [first_entity, second_entity] = firstArg;
            std::string first_entity_table_name = first_entity->tableName();
            std::string second_entity_table_name = second_entity->tableName();

            qDebug() << "Main Table: "<< QString::fromStdString(m_main_table);
            qDebug() << "FK Entity: "<< QString::fromStdString(fk_field(first_entity_table_name));
            qDebug() << " ***** ";

            std::stringstream str_stream;
            str_stream << " INNER JOIN "+first_entity_table_name+" ON ( "
                       << m_main_table+"."+fk_field(first_entity_table_name)+" = "
                       << first_entity_table_name+".id )";
            m_chain_joins.push_back(str_stream.str());

            str_stream.str("");

            str_stream << "INNER JOIN "+second_entity_table_name+" ON ( "
                       << first_entity_table_name+"."+fk_field(second_entity_table_name)+" = "
                       << second_entity_table_name+".id )";

            m_chain_joins.push_back(str_stream.str());

            if constexpr(sizeof...(args) > 0) {
                make_chain_join_statments(args...);
            }


        }


    private:
        BaseEntity* m_main_entity;
        std::string m_main_table{""};
        std::string m_filter{""};
        std::vector<std::string>m_query_fields;
        //std::vector<std::string> m_related_tables;
        std::map<std::string, const std::type_info*> m_related_tables;

        std::map<std::string,  std::tuple<BaseEntity*, Nullable>> m_foreign_key_fields;

        std::map<int, std::map<std::string, std::string>> m_relation_tree;
        std::string m_query;
        std::vector<std::string>m_filter_cache;
        EntityDataModel* m_model;
        QueryResults m_query_results;
        MappedEntity m_mapped_entities;
        std::vector<std::string> m_chain_joins;
    };


    struct FindByFieldName{
        FindByFieldName(std::string field_name)
            :m_field_name{field_name}{}
        bool operator()(std::tuple<std::string, std::unique_ptr<Field>> const& field){
            auto const&[f_name, field_obj] = field;
            return (f_name == m_field_name);
        }
        private:
            std::string m_field_name;
    };


}
