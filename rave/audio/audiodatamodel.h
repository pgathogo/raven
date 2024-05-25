#ifndef AUDIODATAMODEL_H
#define AUDIODATAMODEL_H

#include <memory>

#include <QObject>
#include "audio.h"
#include "artist.h"
#include "audiolibitem.h"
#include "audiotrackviewer.h"
#include "../framework/relationmapper.h"
#include "../framework/entitydatamodel.h"

//class EntityDataModel;

namespace AUDIO {

//class AudioLibItem;
//class AudioTrackViewer;

    class AudioDataModel : public QObject
    {
        Q_OBJECT
    public:
        AudioDataModel(AudioTrackViewer* viewer);
        void fetch_audio_data(FRAMEWORK::RelationMapper*);
        void show_data();
        std::shared_ptr<Audio> find_audio_by_id(int);

        template<typename T>
        void fetch_filtered_audio(T arg)
        {
           Artist artist;
           Audio audio;

           auto [field_name, op, value] = arg;
           auto base_filter = std::make_tuple(field_name, op, value);
           auto active_audio = std::make_tuple(audio.is_deleted()->qualified_column_name<Audio>(),
                                               "=", false);
           FRAMEWORK::RelationMapper* r_mapper = new FRAMEWORK::RelationMapper();
           r_mapper = m_audio_edm->select_related(artist)->filter(base_filter, active_audio);

           fetch_audio_data(r_mapper);
        }
    signals:
        void selected_audio(std::shared_ptr<Audio>);

    private slots:
        void track_selected(int);

    private:
       AudioTrackViewer* m_viewer;
       std::unique_ptr<AudioLibItem> m_lib_item;
       std::unique_ptr<EntityDataModel> m_audio_edm;
    };





}


#endif
