
#include "clientapplication.h"

bool check_permission()
{
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied)
    {
        QtAndroid::requestPermissionsSync( QStringList() <<  "android.permission.WRITE_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied)
        {
            qDebug() << "Permission denied";
            return false;
        }
    }

    qDebug() << "Permissions granted!";
    return true;
}

ClientApplication::ClientApplication(QWidget* parent):
    QMainWindow(parent)
{
    textEdit = new QTextEdit;
    setCentralWidget(textEdit);
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    logFile = new QFile(filePath+"/log.kml");
    if (!logFile->open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << logFile->errorString();
    auto line1 = QString("<?xml version=\"1.0\" standalone=\"yes\"?><kml xmlns=\"http://earth.google.com/kml/2.2 \"><Document><Folder id=\"Waypoints\">");
    QTextStream out1(logFile);
    out1 << line1;

    QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);

    connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
            this, SLOT(positionUpdated(QGeoPositionInfo)));

    source->setUpdateInterval(10000);
    source->startUpdates();
}

ClientApplication::~ClientApplication()
{
    QTextStream out2(logFile);
    auto line2 = QString("<name>Waypoints</name><visibility>1</visibility></Folder><name><![CDATA[GPS data]]></name><open>1</open><visibility>1</visibility></Document></kml>");
    out2 << line2;
}

void ClientApplication::positionUpdated(const QGeoPositionInfo &info)
{
    auto coord = info.coordinate();
    auto longit = coord.longitude();
    auto lat = coord.latitude();
    //auto alt = coord.altitude();
    auto line = QString("Position updated: Date/time = %1, Coordinate = %2 \n").arg(info.timestamp().toString()).arg(coord.toString());
    textEdit->append(line);
    //logFile->write(line.toLatin1());
    static auto i = 0;
    QTextStream out(logFile);
    auto line2 = QString("<Placemark><Point><altitudeMode>clampToGround</altitudeMode><coordinates>%2, %3</coordinates></Point><name>%1</name></Placemark>").arg(i).arg(longit).arg(lat);
    out << line2;
    i++;
}
