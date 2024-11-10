#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QJsonArray>
#include <QPushButton>
#include <QMediaPlayer>
#include <QListWidgetItem>
#include <QSlider>
#include <QLabel>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void songSelectedForPlayback(int songId);

public slots:
    void onSongsFetched(const QJsonArray &songs);
    void onSongUrlFetched(const QString &url);
    void onSongSelected(QListWidgetItem *item);
    void onPlayButtonClicked();
    void onPauseButtonClicked();
    void onStopButtonClicked();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onErrorOccurred(QMediaPlayer::Error error);
    void onStateChanged(QMediaPlayer::PlaybackState state);
    void onVolumeChanged(int value);

    QListWidget* getSongsListWidget() { return songsListWidget; }
    QMediaPlayer* getMediaPlayer() { return mediaPlayer; }

private:
    QListWidget *songsListWidget;
    QMediaPlayer *mediaPlayer;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QString currentSongUrl;
    QSlider *volumeSlider;
};

#endif // MAINWINDOW_H
