#pragma once

#include "../audio/audio.h"

class QStandardItemModel;
class QStandardItem;

using Columns = QList<QStandardItem*>;

namespace AUDIO{

    class GenreTypeItem
    {
    public:
        GenreTypeItem();
        ~GenreTypeItem();

        GenreTypeItem(AUDIO::Genre*);
        GenreTypeItem(QStandardItemModel*);

        virtual QString genre_name();
        AUDIO::Genre* genre() const;

        template<typename T>
        void create_row_item(AUDIO::Genre* genre)
        {
            static_assert(std::is_base_of<AUDIO::GenreTypeItem, T>::value, "`T` must be derived from Genre");

            Columns columns;

            auto item = std::make_unique<T>(genre);
            auto genre_name = new T(item->genre_name());
            genre_name->setData(genre->id(), Qt::UserRole);

            columns.append(genre_name);

            m_model->appendRow(columns);
            m_genre_type_items.push_back(std::move(item));
        }

    private:
        AUDIO::Genre* m_genre;
        QStandardItemModel* m_model;
        std::vector<std::unique_ptr<AUDIO::GenreTypeItem>> m_genre_type_items;
    };

    class GeneralGenreTypeItem : public GenreTypeItem, public QStandardItem
    {
    public:
        GeneralGenreTypeItem(AUDIO::Genre*);
        GeneralGenreTypeItem(QString);
        ~GeneralGenreTypeItem();
    };

    class RockGenreTypeItem : public GenreTypeItem, public QStandardItem
    {
     public:
        RockGenreTypeItem(AUDIO::Genre*);
        RockGenreTypeItem(QString);
        ~RockGenreTypeItem();
    };



}

