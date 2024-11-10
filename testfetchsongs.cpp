#include "testfetchsongs.h"
#include "songitemwidget.h"
#include "mainwindow.h"
#include <QMediaPlayer>

void TestRealBackendIntegration::testFetchSongsFromBackend()
{
    BackendCommunicator backendCommunicator;

    QSignalSpy loginSpy(&backendCommunicator, &BackendCommunicator::loginFinished);
    backendCommunicator.login("testuser", "testpassword");
    QVERIFY(loginSpy.wait(5000));

    auto loginArguments = loginSpy.takeFirst();
    bool loginSuccess = loginArguments.at(0).toBool();
    QVERIFY(loginSuccess);

    QSignalSpy songListUpdatedSpy(&backendCommunicator, &BackendCommunicator::songsFetched);
    backendCommunicator.fetchSongs();
    QVERIFY(songListUpdatedSpy.wait(5000));

    auto songsArguments = songListUpdatedSpy.takeFirst();
    QJsonArray songsList = songsArguments.at(0).toJsonArray();
    QVERIFY(!songsList.isEmpty());
}

void TestRealBackendIntegration::testPlaybackStart()
{
    MainWindow mainWindow;
    BackendCommunicator backendCommunicator;

    QSignalSpy songListUpdatedSpy(&backendCommunicator, &BackendCommunicator::songsFetched);
    QSignalSpy playbackStartedSpy(mainWindow.getMediaPlayer(), &QMediaPlayer::playbackStateChanged);

    connect(&backendCommunicator, &BackendCommunicator::songsFetched, &mainWindow, &MainWindow::onSongsFetched);
    connect(&backendCommunicator, &BackendCommunicator::songFetched, &mainWindow, &MainWindow::onSongUrlFetched);

    QSignalSpy loginSpy(&backendCommunicator, &BackendCommunicator::loginFinished);
    backendCommunicator.login("testuser", "testpassword");
    QVERIFY(loginSpy.wait(5000));
    QVERIFY(loginSpy.takeFirst().at(0).toBool());

    backendCommunicator.fetchSongs();
    QVERIFY(songListUpdatedSpy.wait(5000));
    QVERIFY(!songListUpdatedSpy.isEmpty());

    QSignalSpy songFetchedSpy(&backendCommunicator, &BackendCommunicator::songFetched);
    backendCommunicator.requestSongUrl(1);
    QVERIFY(songFetchedSpy.wait(5000));

    mainWindow.onPlayButtonClicked();

    QVERIFY(playbackStartedSpy.wait(5000));
    QCOMPARE(playbackStartedSpy.takeLast().at(0).toInt(), QMediaPlayer::PlayingState);
}
