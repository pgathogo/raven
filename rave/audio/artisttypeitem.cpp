#include <QStandardItemModel>
#include <QStandardItem>

#include "artisttypeitem.h"
#include "../framework/choicefield.h"

namespace AUDIO
{
    ArtistTypeItem::ArtistTypeItem()
    {
    }

    ArtistTypeItem::ArtistTypeItem(AUDIO::Artist* artist)
        :m_artist{artist}
    {
    }

    ArtistTypeItem::ArtistTypeItem(QStandardItemModel* model)
        :m_model{model}
    {
    }

    ArtistTypeItem::~ArtistTypeItem()
    {
    }

    AUDIO::Artist* ArtistTypeItem::artist() const
    {
        return m_artist;
    }

    QString ArtistTypeItem::first_name()
    {
        return QString::fromStdString(artist()->first_name()->value());
    }

    QString ArtistTypeItem::surname()
    {
        return QString::fromStdString(artist()->surname()->value());
    }

    QString ArtistTypeItem::fullname()
    {
        return QString::fromStdString(artist()->fullName()->value());
    }

    QString ArtistTypeItem::artist_type()
    {
        return QString::fromStdString(artist()->artist_type()->displayName());
    }

    QString ArtistTypeItem::notes()
    {
        return QString::fromStdString(artist()->notes()->value());
    }

    /* ----- FemaleArtistTypeItem --------- */

    FemaleArtistTypeItem::FemaleArtistTypeItem(AUDIO::Artist* artist)
        :ArtistTypeItem(artist)
    {
    }

    FemaleArtistTypeItem::FemaleArtistTypeItem(QString text)
        :ArtistTypeItem()
    {
        setText(text);
    }

    FemaleArtistTypeItem::~FemaleArtistTypeItem()
    {
    }

    /* ------ MaleArtistTypeItem --------- */

    MaleArtistTypeItem::MaleArtistTypeItem(AUDIO::Artist* artist)
        :ArtistTypeItem(artist)
    {
    }

    MaleArtistTypeItem::MaleArtistTypeItem(QString text)
    {
        setText(text);
    }

    MaleArtistTypeItem::~MaleArtistTypeItem()
    {
    }

    /* ----- GroupArtistTypeItem ------- */

    GroupArtistTypeItem::GroupArtistTypeItem(AUDIO::Artist* artist)
        :ArtistTypeItem(artist)
    {
    }

    GroupArtistTypeItem::GroupArtistTypeItem(QString text)
    {
        setText(text);
    }

    GroupArtistTypeItem::~GroupArtistTypeItem()
    {
    }

}
