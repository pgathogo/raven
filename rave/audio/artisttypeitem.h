#pragma once

#include "../audio/artist.h"

class QStandardItemModel;
class QStandardItem;

using Columns = QList<QStandardItem*>;

namespace AUDIO
{
    class ArtistTypeItem
    {
    public:
        ArtistTypeItem();
        ArtistTypeItem(AUDIO::Artist*);
        ArtistTypeItem(QStandardItemModel*);
        ~ArtistTypeItem();

        virtual QString first_name();
        virtual QString surname();
        virtual QString fullname();
        virtual QString artist_type();
        virtual QString notes();

        AUDIO::Artist* artist() const;

        template<typename T>
        void create_row_item(AUDIO::Artist* artist)
        {
            static_assert(std::is_base_of<AUDIO::ArtistTypeItem, T>::value, "`T` must be derived from ArtistTypeItem");

            Columns columns;

            auto item = std::make_unique<T>(artist);
            auto first_name = new T(item->first_name());
            first_name->setData(artist->id(), Qt::UserRole);

            auto surname = new T(item->surname());
            auto fullname = new T(item->fullname());
            auto artist_type = new T(item->artist_type());
            //auto note = new T(item->notes());

            columns.append(first_name);
            columns.append(surname);
            columns.append(fullname);
            columns.append(artist_type);
            //columns.append(note);

            m_model->appendRow(columns);
            m_artist_type_items.push_back(std::move(item));

        }


    private:
        AUDIO::Artist* m_artist;
        QStandardItemModel* m_model;
        std::vector<std::unique_ptr<AUDIO::ArtistTypeItem>> m_artist_type_items;
    };

    class FemaleArtistTypeItem: public ArtistTypeItem, public QStandardItem
    {
     public:
        FemaleArtistTypeItem(AUDIO::Artist*);
        FemaleArtistTypeItem(QString);
        ~FemaleArtistTypeItem();
    };

    class MaleArtistTypeItem: public ArtistTypeItem, public QStandardItem
    {
     public:
        MaleArtistTypeItem(AUDIO::Artist*);
        MaleArtistTypeItem(QString);
        ~MaleArtistTypeItem();
    };

    class GroupArtistTypeItem : public ArtistTypeItem, public QStandardItem
    {
     public:
        GroupArtistTypeItem(AUDIO::Artist*);
        GroupArtistTypeItem(QString);
        ~GroupArtistTypeItem();
    };

}
