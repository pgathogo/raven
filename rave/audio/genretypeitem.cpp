#include <QStandardItemModel>
#include <QStandardItem>

#include "genretypeitem.h"

namespace AUDIO{

    GenreTypeItem::GenreTypeItem()
    {
    }

    GenreTypeItem::GenreTypeItem(AUDIO::Genre* genre)
        :m_genre{genre}
    {
    }

    GenreTypeItem::GenreTypeItem(QStandardItemModel* model)
        :m_model{model}
    {
    }

    GenreTypeItem::~GenreTypeItem()
    {
    }

    QString GenreTypeItem::genre_name()
    {
        return QString::fromStdString(genre()->genre_name()->value());
    }

    AUDIO::Genre* GenreTypeItem::genre() const
    {
        return m_genre;
    }
  /* ----- GeneralGenreTypeItem ---- */

    GeneralGenreTypeItem::GeneralGenreTypeItem(AUDIO::Genre* genre)
        :GenreTypeItem(genre)
    {
    }

    GeneralGenreTypeItem::GeneralGenreTypeItem(QString text)
        :GenreTypeItem()
    {
        setText(text);
    }

    GeneralGenreTypeItem::~GeneralGenreTypeItem()
    {
    }


  /* ----- RockGenreTypeItem ------ */

    RockGenreTypeItem::RockGenreTypeItem(AUDIO::Genre* genre)
        :GenreTypeItem(genre)
    {
    }

    RockGenreTypeItem::RockGenreTypeItem(QString text)
        :GenreTypeItem()
    {
        setText(text);
    }

    RockGenreTypeItem::~RockGenreTypeItem()
    {
    }
}
