#include "mainwindow.h"
#include "songitemwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QJsonObject>
#include <QAudioOutput>
#include <QSlider>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
    songsListWidget(new QListWidget(this)),
    mediaPlayer(new QMediaPlayer(this)),
    volumeSlider(new QSlider(Qt::Horizontal, this))
{
    setWindowTitle("Music Library");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(songsListWidget);

    songsListWidget->setSpacing(5);
    songsListWidget->setStyleSheet(
        "QListWidget::item { border-bottom: 1px solid lightgray; }"
        "QListWidget::item:selected { background-color: #ADD8E6; color: white; }"
    );
    songsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(songsListWidget, &QListWidget::itemClicked, this, &MainWindow::onSongSelected);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    playButton = new QPushButton("Play", this);
    pauseButton = new QPushButton("Pause", this);
    stopButton = new QPushButton("Stop", this);

    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(stopButton);

    layout->addLayout(controlsLayout);

    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    layout->addWidget(new QLabel("Volume:"));
    layout->addWidget(volumeSlider);

    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayButtonClicked);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseButtonClicked);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::onStopButtonClicked);

    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);

    mediaPlayer->setAudioOutput(new QAudioOutput(this));

    setLayout(layout);

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
    connect(mediaPlayer, &QMediaPlayer::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &MainWindow::onStateChanged);

    resize(500, 350);
}

void MainWindow::onSongsFetched(const QJsonArray &songs)
{
    songsListWidget->clear();

    for (const QJsonValue &value : songs) {
        if (value.isObject()) {
            QJsonObject songObject = value.toObject();

            QString title = songObject.value("title").toString();
            QString author = songObject.value("author").toString();
            int duration = songObject.value("duration").toInt();
            int songId = songObject.value("id").toInt();  // Extract the song ID

            SongItemWidget *itemWidget = new SongItemWidget(title, author, duration, songId, this);

            QListWidgetItem *item = new QListWidgetItem(songsListWidget);
            item->setSizeHint(itemWidget->sizeHint());
            songsListWidget->setItemWidget(item, itemWidget);
        }
    }
}

void MainWindow::onSongSelected(QListWidgetItem *item)
{
    SongItemWidget *widget = qobject_cast<SongItemWidget*>(songsListWidget->itemWidget(item));

    if (widget) {
        int songId = widget->getSongId();

        qInfo() << "Song selected: " << songId;

        emit songSelectedForPlayback(songId);
    }
}

void MainWindow::onSongUrlFetched(const QString &url)
{
    if (!url.isEmpty()) {
        qInfo() << "Received URL for song: " << url;

        mediaPlayer->setSource(QUrl(url));

        qInfo() << "Now playing: " << url;
    } else {
        qWarning() << "Failed to fetch song URL.";
    }
}

void MainWindow::onPlayButtonClicked()
{
    qInfo() << "Play button clicked.";
    mediaPlayer->play();
}

void MainWindow::onPauseButtonClicked()
{
    qInfo() << "Pause button clicked.";
    mediaPlayer->pause();
}

void MainWindow::onStopButtonClicked()
{
    qInfo() << "Stop button clicked.";
    mediaPlayer->stop();
}

void MainWindow::onVolumeChanged(int value)
{
    QAudioOutput *audioOutput = mediaPlayer->audioOutput();
    if (audioOutput) {
        qreal volume = value / 100.0; // Convert to a float between 0.0 and 1.0
        audioOutput->setVolume(volume);
        qInfo() << "Volume set to:" << value << "%";
    }
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::NoMedia:
        qInfo() << "Media status changed: No Media.";
        break;
    case QMediaPlayer::LoadingMedia:
        qInfo() << "Media status changed: Loading Media.";
        break;
    case QMediaPlayer::BufferedMedia:
        qInfo() << "Media status changed: Buffered Media.";
        break;
    case QMediaPlayer::LoadedMedia:
        qInfo() << "Media status changed: Playing Media.";
        break;
    case QMediaPlayer::BufferingMedia:
        qInfo() << "Media status changed: Paused Media.";
        break;
    case QMediaPlayer::StalledMedia:
        qInfo() << "Media status changed: Stalled Media.";
        break;
    case QMediaPlayer::EndOfMedia:
        qInfo() << "Media status changed: End of Media.";
        break;
    case QMediaPlayer::InvalidMedia:
        qInfo() << "Media status changed: Invalid Media.";
        break;
    }
}

void MainWindow::onErrorOccurred(QMediaPlayer::Error error)
{
    switch (error) {
    case QMediaPlayer::NoError:
        qInfo() << "No error.";
        break;
    case QMediaPlayer::ResourceError:
        qWarning() << "Resource Error occurred.";
        break;
    case QMediaPlayer::FormatError:
        qWarning() << "Format Error occurred.";
        break;
    case QMediaPlayer::NetworkError:
        qWarning() << "Network Error occurred.";
        break;
    case QMediaPlayer::AccessDeniedError:
        qWarning() << "Access Denied Error occurred.";
        break;
    }
}

void MainWindow::onStateChanged(QMediaPlayer::PlaybackState state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        qInfo() << "Media player state: Stopped.";
        break;
    case QMediaPlayer::PlayingState:
        qInfo() << "Media player state: Playing.";
        break;
    case QMediaPlayer::PausedState:
        qInfo() << "Media player state: Paused.";
        break;
    }
}
