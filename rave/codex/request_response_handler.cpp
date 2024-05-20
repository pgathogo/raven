#include <QJsonObject>
#include <QStorageInfo>
#include "request_response_handler.h"

#include <Windows.h>
#include <cmath>

// RequestResponseManager

DiskListRequestHandler::DiskListRequestHandler()
{

}

QString DiskListRequestHandler::handler_type()
{
    return "DISK-LIST";
}

Response DiskListRequestHandler::handle_request(Request request)
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

        // shift right to the next letter
        drive_mask >>= 1;
    }

    QJsonDocument json_doc(all_volumes);
    return json_doc;

}

DiskInfo DiskListRequestHandler::disk_info(const QString& drive)
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





