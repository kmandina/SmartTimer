#ifndef ALERTWIDGET_H
#define ALERTWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QSound>
#include <QMediaPlayer>
#include <QUrl>
#include <QMediaPlaylist>

namespace Ui {
class alertwidget;
}

class alertwidget : public QWidget
{
    Q_OBJECT

public:
    explicit alertwidget(int msecs, const QString& name, bool turnedOn = false, QWidget *parent = nullptr);
    ~alertwidget();

    inline bool getState() { return state; }
    inline QString getName() {return alertName; }
    int getAlertTime();

public slots:
    void statusChanged(bool);
    void onTickCheck();
    void blink();
    void stopBlinking();
    void closeAlarm();

    void changeAlarm();

    void ShowContextMenu(const QPoint &);

    void setAlarm(int,const QString&);

signals:
    void alarmFinished();
    void del(const alertwidget*);

    void customContextMenuRequested(const QPoint &);

private:
    Ui::alertwidget *ui;

    QTime alertTime;
    QTimer alertTick;
    QTimer blinkTimer;
    QString alertName;

    bool state;
    bool blinking;
    bool blinky;

    QMediaPlaylist *playlist;
    QMediaPlayer *player;

    void mousePressEvent(QMouseEvent *e);

};


int getMsecs(const QTime& t);

int calculateDuration(const QTime& t);

#endif // ALERTWIDGET_H
