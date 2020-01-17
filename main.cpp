#include <QApplication>

#include "clientapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!check_permission())
        return -1;

    ClientApplication client;

    client.show();

    return app.exec();
}
