#ifndef ALERTWIDGET_H
#define ALERTWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>

namespace Ui {
class alertwidget;
}

class alertwidget : public QWidget
{
    Q_OBJECT

public:
    explicit alertwidget(int msecs, const QString& name, QWidget *parent = nullptr);
    ~alertwidget();

    inline bool getState() { return state; }
    inline QString getName() {return alertName; }
    int getAlertTime();

public slots:
    void statusChanged(bool);
    void onTickCheck();
    void blink();

signals:


private:
    Ui::alertwidget *ui;

    QTime alertTime;
    QTimer alertTick;
    QTimer blinkTimer;
    QString alertName;

    bool state;
    bool blinking;
    bool blinky;

};


int getMsecs(const QTime& t)
{
    return (t.hour()*3600+t.minute()*60+t.second())*1000;
}

#endif // ALERTWIDGET_H