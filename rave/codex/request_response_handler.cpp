#include <QJsonObject>
#include <QStorageInfo>
#include "request_response_handler.h"

#include <Windows.h>
#include <cmath>

// RequestResponseManager

DiskListRequest::DiskListRequest()
{

}

QString DiskListRequest::handler_type()
{
    return "DISK-LIST";
}

Response DiskListRequest::handle_request(Request request)
{
    QJsonObject all_volumes;

    DWORD drive_mask = GetLogicalDrives();

    for (char drive_letter = 'A'; drive_letter <= 'Z'; ++drive_letter) {

        if (drive_mask & 1) {
            QString drive_path = QString("%1:\\").arg(drive_letter);
            DiskInfo di = disk_info(drive_path);

            QJsonObject volume_data;
            volume_data["total"] = di.total;
            volume_data["free"] = di.free;
            all_volumes[drive_path] = volume_data;
        }

        drive_mask >>= 1;
    }

    QJsonDocument json_doc(all_volumes);
    return json_doc;

}

DiskInfo DiskListRequest::disk_info(const QString& drive)
{
    ULARGE_INTEGER total, free;

    DiskInfo di;

    if (GetDiskFreeSpaceExW(drive.toStdWString().c_str(), &free, &total, NULL)) {
        double totalgb = round(total.QuadPart / (1024.0 * 1024.0 * 1024.0) * 10) /10;
        double freegb = round(free.QuadPart / (1024.0 * 1024.0 * 1024.0) * 10) /10;

        di.free = freegb;
        di.total = totalgb;

        return di;
    }

    return di;
}


    /*
    QList<QStorageInfo> volumes = QStorageInfo().mountedVolumes();
    for(auto volume : volumes) {
        QJsonObject volume_data;
        qint64 total = volume.bytesTotal();
        float total_f = total / (1000 * 1000 * 1000);
        QString total_s = " "+QString::number(total_f,'f', 3);

        volume_data["total"] = total_s; ///volume.bytesTotal()/1000/1000;
        volume_data["free"] = volume.bytesAvailable()/1000/1000;
        all_volumes[volume.displayName()] = volume_data;
    }

    QJsonDocument json_doc(all_volumes);
    return json_doc;
   */



