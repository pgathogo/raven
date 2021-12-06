#pragma once

#include <memory>
#include <QDialog>

#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../framework/relationmapper.h"
#include "../framework/entitydatamodel.h"

namespace Ui {
class TrashCanForm;
}

namespace FRAMEWORK{
  class RelationMapper;
}

namespace AUDIO{
  class AudioLibItem;
}

class QStandardItemModel;

class BaseEntity;
class EntityDataModel;

class TrashCanForm : public QDialog
{
    Q_OBJECT

public:
    explicit TrashCanForm(QWidget *parent = nullptr);
    ~TrashCanForm();

    void create_track_view_headers();
    void set_track_view_column_width();

    void fetch_deleted_audio(FRAMEWORK::RelationMapper*);
    void show_audio_data();
    void set_tracks_view();
    void un_delete_audio(const std::string);

    template<typename... T>
    void fetch_del_audio(T... args)
    {
        AUDIO::Artist artist;
        AUDIO::Folder folder;
        AUDIO::Genre genre;
        AUDIO::Audio audio;

        if constexpr(sizeof...(args) > 0)
            auto [field_name, op, value] = ((args),...);

        auto deleted_audio_only = std::make_tuple(audio.is_deleted()->qualified_column_name<AUDIO::Audio>(), "=", true);
        FRAMEWORK::RelationMapper* r_mapper = m_audio_edm->select_related(folder, artist, genre)->filter(deleted_audio_only);
        fetch_deleted_audio(r_mapper);
    }

private slots:
    void restore();


private:
    Ui::TrashCanForm *ui;

    QStandardItemModel* m_tracks_model;
    std::unique_ptr<EntityDataModel> m_audio_edm;
    std::unique_ptr<AUDIO::AudioLibItem> m_audio_lib_item;

};

