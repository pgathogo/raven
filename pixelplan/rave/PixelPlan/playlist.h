#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <memory>
#include <tuple>
#include <vector>

#include <QDomDocument>
#include <QDate>



namespace PIXELPLAN {

    class PlaylistItem : public QDomElement {
        PlaylistItem(const QString name);
    private:
        QString m_item_name;
    };

#define __TAG
#define FOR_EACH_ELEMENT_TYPE \
        __TAG(Invalid) \
        __TAG(ID) \
        __TAG(CHANNEL) \
        __TAG(ORD) \
        __TAG(PARENTID) \
        __TAG(ITEM_TYPE) \
        __TAG(CLIP_PATH) \
        __TAG(RESERVED_1) \
        __TAG(RESERVED_2) \
        __TAG(RESERVED_3) \
        __TAG(RESERVED_4) \
        __TAG(LAST_RESULT_PLAYED) \
        __TAG(EVENT_END) \
        __TAG(ENTRY_TIME_ESTIMATED) \
        __TAG(FILE_MODIFICATION_TIME) \
        __TAG(FILE_SIZE) \
        __TAG(MARK_IN) \
        __TAG(MARK_OUT) \
        __TAG(TOTAL_DURATION) \
        __TAG(VIDEO_WIDTH) \
        __TAG(VIDEO_HEIGHT) \
        __TAG(TFF) \
        __TAG(SWAP_FIELD) \
        __TAG(INTERLACED) \
        __TAG(INTERLACED_FORCED) \
        __TAG(ASPECT_RATIO) \
        __TAG(ASPECT_RATIO_FORCED) \
        __TAG(FRAME_RATE) \
        __TAG(ASSIGNED_LOGO_ID) \
        __TAG(METADATA_1) \
        __TAG(METADATA_2) \
        __TAG(METADATA_3) \
        __TAG(METADATA_4) \
        __TAG(METADATA_5) \
        __TAG(METADATA_6) \
        __TAG(METADATA_7) \
        __TAG(METADATA_8) \
        __TAG(MATERIAL_AVAILABILITY) \
        __TAG(LOCATION) \
        __TAG(ANALYZED) \
        __TAG(PROGRAM_TS_NUM_SELECTED) \
        __TAG(COLOR_ADJUSTEMENT) \
        __TAG(COLOR_SPACE) \
        __TAG(OBSERVATIONS) \
        __TAG(DATE_TEMPLATE_SET) \
        __TAG(SOUND_LOUDNESS) \
        __TAG(SOUND_GAIN) \
        __TAG(FILE_ID) \
        __TAG(CLIP_ALTERNATIVE_TITLE) \
        __TAG(REQUEST_ANALYZE_INSTRUCTION) \
        __TAG(AUTOMATION_IN) \
        __TAG(AUTOMATION_OUT) \
        __TAG(GUID) \
        __TAG(SubtitleEnabled) \
        __TAG(Count)

    struct PlaylistElement {

        enum class TagType {
            #define __TAG(x) x,
            FOR_EACH_ELEMENT_TYPE
            #undef __TAG
        };

        const char* to_string() const
        {
            switch (m_tag_type) {
        #define __TAG(x) \
            case TagType::x: \
                return #x;
            FOR_EACH_ELEMENT_TYPE
        #undef __TAG
            }
        }

        TagType m_tag_type{ TagType::Invalid };


    };

    using PlaylistItemAttributes  = std::map<std::string, std::string>;
    using ErrorOr = std::tuple<bool, QString>;

    class Playlist {

    public:
        Playlist(const QString playlist_filepath, const QString output_filepath);
        ~Playlist();

        ErrorOr set_playlist_content();

        void create_playlist_item(PlaylistItemAttributes pia);
        QDomNode find_node(QString node_name);

        PlaylistItemAttributes attributes();
        void make_attribute_elements(PlaylistItemAttributes&, QDomElement&);
        ErrorOr save_playlist();


    private:
        QString m_playlist_file;
        QString m_output_filepath;

        std::unique_ptr<QDomDocument> m_dom_document;
        std::vector<QDomElement> m_playlist_items;

        void update_playlist(std::vector<QDomElement>&);
        bool attribute_has_zero_value(std::string);
        bool attribute_has_negative_one(std::string);

    };


}





#endif
