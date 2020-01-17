#include <QtCore>
#include <QtPositioning/qgeopositioninfosource.h>
#include <QtAndroid>
#ifndef CLIENTAPPLICATION_H
#define CLIENTAPPLICATION_H

#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
class QGeoPositionInfo;
class QTextEdit;
QT_END_NAMESPACE

class ClientApplication : public QMainWindow
{
    Q_OBJECT
public:
    ClientApplication(QWidget *parent = 0);
    ~ClientApplication();

private slots:
    void positionUpdated(const QGeoPositionInfo &info);

private:
    QTextEdit* textEdit;
    QFile * logFile;
    QTextStream out;
};

bool check_permission();

#endif
