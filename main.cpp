#include <QApplication>
#include "loginwindow.h"
#include "mainwindow.h"
#include "backendcommunicator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BackendCommunicator *backend = new BackendCommunicator();
    LoginWindow loginWindow(backend);
    MainWindow mainWindow;

    QObject::connect(backend, &BackendCommunicator::loginFinished, &mainWindow, [&](bool success) {
        if (success)
            mainWindow.show();
    });

    QObject::connect(&mainWindow, &MainWindow::songSelectedForPlayback, &mainWindow,
                    [&](int songId) { backend->requestSongUrl(songId); });

    QObject::connect(backend, &BackendCommunicator::songsFetched, &mainWindow, &MainWindow::onSongsFetched);
    QObject::connect(backend, &BackendCommunicator::songFetched, &mainWindow, &MainWindow::onSongUrlFetched);

    loginWindow.show();

    return app.exec();
}
