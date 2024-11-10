#include <QtTest/QtTest>
#include "backendcommunicator.h"
#include <QSignalSpy>
#include <QJsonArray>
#include <QJsonObject>

class TestRealBackendIntegration : public QObject
{
    Q_OBJECT

private slots:
    void testFetchSongsFromBackend();
    void testPlaybackStart();
};
