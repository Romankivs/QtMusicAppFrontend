#include "SongItemWidget.h"
#include <QPixmap>
#include <QFont>

SongItemWidget::SongItemWidget(const QString &title, const QString &author, int duration, int songId, QWidget *parent)
    : QWidget(parent), songId(songId)
{
    titleLabel = new QLabel(title, this);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    authorLabel = new QLabel(author, this);

    durationLabel = new QLabel(QString("🕒 %1 sec").arg(duration), this);
    durationLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(authorLabel);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(textLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(durationLabel);

    mainLayout->setContentsMargins(5, 5, 5, 5);
    setLayout(mainLayout);
}

int SongItemWidget::getSongId() const
{
    return songId;
}
