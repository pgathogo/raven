#include <QDebug>
#include <QString>
#include "relationmapper.h"
#include "baseentity.h"
#include "entitydatamodel.h"
#include "../audio/artist.h"
#include "../utils/tools.h"

namespace FRAMEWORK{

    RelationMapper::RelationMapper()
        :m_model{nullptr}
    {
    }

    RelationMapper::RelationMapper(EntityDataModel* model)
        :m_model{model}
    {
    }

    RelationMapper::~RelationMapper()
    {
    }

    std::string RelationMapper::main_table()
    {
        return m_main_table;
    }

    //std::vector<std::string>& RelationMapper::related_tables()
    std::map<std::string, const std::type_info*>& RelationMapper::related_tables()
    {
        return m_related_tables;
    }

    std::map<std::string, std::tuple<BaseEntity*, Nullable>>& RelationMapper::foreign_key_fields()
    {
        return m_foreign_key_fields;
    }

    void RelationMapper::set_main_entity(BaseEntity* entity)
    {
        m_main_entity = entity;
    }

    void RelationMapper::set_main_table(std::string table)
    {
        m_main_table = table;
    }

    void RelationMapper::append_related_tables(std::string table, const std::type_info* type)
    {
        //m_related_tables.push_back(table);
        m_related_tables[table] = type;
    }

    void RelationMapper::append_foreign_key_fields(std::string key, std::tuple<BaseEntity*, Nullable> value)
    {
        m_foreign_key_fields[key] = value;
    }

    std::vector<std::string>& RelationMapper::query_fields()
    {
        return m_query_fields;
    }

    void RelationMapper::append_query_fields(std::string field)
    {
        m_query_fields.push_back(field);
    }

    void RelationMapper::clear_query_fields()
    {
        m_query_fields.clear();
    }

    void RelationMapper::clear_related_tables()
    {
        m_related_tables.clear();
    }

    void RelationMapper::clear_foreign_key_fields()
    {
        m_foreign_key_fields.clear();
    }

    void RelationMapper::clear_query_results()
    {
        m_query_results.clear();
    }

    void RelationMapper::clear_mapped_entities()
    {
        m_mapped_entities.clear();
    }

    void RelationMapper::print_related_tables()
    {
        qDebug() << " --- Related Tables --- ";
        for(auto const& [table, type]: m_related_tables)
            qDebug() << QString::fromStdString(table);
    }

    void RelationMapper::print_foreign_key_fields()
    {
        qDebug() << " --- Foreign Key Fields --- ";
        for(auto const& [name, field] : m_foreign_key_fields) {
            qDebug() << QString::fromStdString(name);
//            auto [entity, is_nullable] = field;
       }
    }

    void RelationMapper::print_query_fields()
    {
        qDebug() << " --- Query Fields --- ";
        for(auto f : m_query_fields)
            qDebug() << QString::fromStdString(f);
    }

    void RelationMapper::build_relation_tree()
    {
        int index = 0;

        if (m_related_tables.size() > 0){
            auto[table_name, type] = *m_related_tables.begin();
            m_relation_tree[index][m_main_table] = table_name;
        }else{
            m_relation_tree[index][m_main_table] = "";
            return;
        }

        if (m_related_tables.size() == 1 )
            return;

        inner_relation_tree(index);

    }

    void RelationMapper::inner_relation_tree(int index)
    {
        if (index < m_related_tables.size()){
            int next_index = index + 1;

            if (next_index < m_related_tables.size()){
                auto prev = m_related_tables.begin();
                std::advance(prev, index);
                auto[prev_table_name, prev_type] = *prev;

                auto it = m_related_tables.begin();
                std::advance(it, next_index);

                auto [table_name, type] = *it;
                m_relation_tree[next_index][prev_table_name] = table_name;
                inner_relation_tree(++index);
            }else{
                auto it = m_related_tables.begin();
                std::advance(it, index);
                auto [table_name, type] = *it;
                m_relation_tree[next_index][table_name]= "";
            }

        }

    }

    void RelationMapper::print_relation_tree()
    {
        for(auto const& [count, left_right] : m_relation_tree) {
            int cnt = count;
            for(auto const& [left, right] : left_right)
                qDebug() << cnt << " - " << stoq(left) << " => " << stoq(right);
        }
    }

    void RelationMapper::print_filter_cache()
    {
        qDebug() << " ------ filter_cache -----";
        qDebug() << stoq(m_filter);
    }


    std::string RelationMapper::fk_field(std::string t_name)
    {
        std::string fk_name;
        for(auto const& [key, field] : m_foreign_key_fields){
            auto const& [entity, is_nullable] = field;
            if (t_name == entity->tableName()){
                fk_name = key;
                break;
            }
        }

        return fk_name;
    }

    RelationMapper& RelationMapper::prepare_query()
    {
        std::string col_stmts = columns();

        auto join_stmt = join_statements_asString();

        m_query = "SELECT "+col_stmts+" FROM "+m_main_table+join_stmt;

        return *this;

    }

    std::string RelationMapper::columns()
    {
        std::string field_str{""};
        std::string field_alias{""};
        for (auto const& field : m_query_fields){
            if (field_str.size() > 0)
                field_str += ",";
            field_alias = field;
            std::replace(field_alias.begin(), field_alias.end(), '.', '9');
            field_str += field+' '+field_alias;
        }
        return field_str;
    }

    std::vector<std::string> RelationMapper::join_statements()
    {
        std::vector<std::string> joins;
        for (auto const& [count, left_right] : m_relation_tree){
            for(auto const& [left, right] : left_right){
                if (right.size() == 0)
                    break;
                std::string str = " FULL OUTER JOIN "+ right+" ON ( "+m_main_table+"."+fk_field(right)+" = "+right+".id )";
                joins.push_back(str);
            }
        }

        return joins;
    }

    std::string RelationMapper::join_statements_asString()
    {
        std::string joins{""};
        auto join_stmts = join_statements();
        for(auto const& stmt : join_stmts){
            joins += stmt;
        }

        return joins;
    }


    std::string RelationMapper::query() const
    {
        return m_query;
    }


    void RelationMapper::gather_data()
    {

        auto provider = m_model->getDBManager()->provider();

        if (provider->cacheSize() == 0)
            return;

        provider->cache()->first();

        std::string field_name;
        std::string field_value;

        int record_id = 0;

        do{

            auto it_begin = provider->cache()->currentElement()->begin();
            auto it_end = provider->cache()->currentElement()->end();

            for(; it_begin != it_end; ++it_begin){

                field_name = (*it_begin).first;
                field_value = (*it_begin).second;

                std::size_t pos = field_name.find("9");

                std::string table_name = field_name.substr(0, pos);
                std::string f_name = field_name.substr(pos+1);

                append_query_results(record_id, table_name, f_name, field_value);
            }

            provider->cache()->next();

            ++record_id;

        }while (!provider->cache()->isLast());

    }


    void RelationMapper::map_data_main_entity()
    {
        for(auto const&[record_id, record] : m_query_results){
            for(auto const&[table_name, fields] : record){
                if (table_name == m_main_entity->tableName()){
                    data_to_object(record_id, table_name, m_main_entity, fields);
                    break;
                }
            }
        }
    }

    void RelationMapper::map_data_object()
    {
        map_data_main_entity();

        for (auto const&[record_id, record] : m_query_results){
            for (auto const&[fk_field_name, entity_tuple] : m_foreign_key_fields){
                auto[entity, nullable] = entity_tuple;

                for (auto const&[table_name, fields] : record){
                    if (table_name != entity->tableName())
                        continue;
                    data_to_object(record_id, table_name, entity, fields);
                    break;
                }

            }
        }
    }

    void RelationMapper::data_to_object(int record_id, const std::string table_name,
                        BaseEntity* entity, const std::vector<std::tuple<std::string, std::string>>& fields)
    {
        auto entity_object = entity->cloneAsUnique();

        for (auto const& field_value : fields){

            auto [f, v] = field_value;

            auto iter = std::find_if(entity_object->fields().begin(), entity_object->fields().end(),
                                     FRAMEWORK::FindByFieldName(f));

            if (iter != entity_object->fields().end()){
                auto const& [f_name, field] = *iter;
                field->stringToValue(v);
            }

        }

        m_mapped_entities[record_id][table_name].push_back(std::move(entity_object));
    }

    void RelationMapper::fetch_fk_fields(BaseEntity* entity)
    {
        for (auto const& [field_name, field] : entity->fields()){
            if (field->field_type() == "ForeignKeyField"){
                auto fk_field = dynamic_cast<ForeignKeyField*>(field.get());
                auto fk_field_entity = std::make_tuple(fk_field->fk_entity(), field->nullable());
                append_foreign_key_fields(field_name, fk_field_entity);
            }
        }
    }

    void RelationMapper::print_mapped_entities()
    {
        for(auto const& [record_id, record] : m_mapped_entities){
            qDebug() << record_id << ":" ;
            for(auto const& [table_name, entities] : record){
                qDebug() << "  " << stoq(table_name);
                for(auto const& entity: entities){
                    if (entity->tableName() == table_name){
                        for(auto const& field_obj : entity->fields()){
                            auto const& [field_name, field] = field_obj;
                            qDebug() << "    "<< stoq(field_name) <<":"<< stoq(field->valueToString());
                        }
                    }
                }
            }

            // record
        }

    }

    MappedEntity const& RelationMapper::mapped_entities() const
    {
        return m_mapped_entities;
    }

    void RelationMapper::set_model(EntityDataModel* edm)
    {
        m_model = edm;
    }

    void RelationMapper::append_query_results(int record_id, const std::string table_name, const std::string field_name, const std::string field_value)
    {
        auto field_name_value = std::make_tuple(field_name, field_value);
        m_query_results[record_id][table_name].push_back(field_name_value);

    }

    void RelationMapper::print_query_results()
    {
        for(auto const&[record_id, record] : m_query_results){
            qDebug() << record_id << ":";
            for(auto const&[table_name, results] : record){
                qDebug() << "  "<< stoq(table_name) << ":";
                for (auto const& field_value : results){
                    auto [field, value] = field_value;
                    qDebug() <<"        "<< stoq(field) << "=" << stoq(value);
                }

            }
        }
    }

    void RelationMapper::print_join_statements_chain()
    {
      for (auto stmt : m_chain_joins){
          qDebug() << stoq(stmt);
      }

    }

    void RelationMapper::map_data()
    {
        gather_data();
        map_data_object();
    }

    void RelationMapper::print_data_objects(const std::string table_name)
    {
        /*
        auto artist = dynamic_cast<AUDIO::Artist*>(m_mapped_entities[table_name].get());
        qDebug() << "** Print Data Objects **";
        qDebug() << stoq(artist->fullName()->value());
        */
    }
}
