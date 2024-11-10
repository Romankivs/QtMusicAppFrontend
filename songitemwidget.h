#ifndef SONGITEMWIDGET_H
#define SONGITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class SongItemWidget : public QWidget
{
    Q_OBJECT

public:
    SongItemWidget(const QString &title, const QString &author, int duration, int songId, QWidget *parent = nullptr);

    int getSongId() const;

private:
    QLabel *imageLabel;
    QLabel *titleLabel;
    QLabel *authorLabel;
    QLabel *durationLabel;
    int songId;
};

#endif // SONGITEMWIDGET_H
