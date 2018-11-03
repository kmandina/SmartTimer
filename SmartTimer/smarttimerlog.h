#pragma once

#include "timerwidget.h"
#include "alertwidget.h"
#include "widgetsettings.h"


#include <QObject>
#include <QString>
#include <QFile>

class smartTimerLog : public QObject
{
    Q_OBJECT
public:
    explicit smartTimerLog(QObject *parent = nullptr);
    void runLogger();

signals:
    void createTimer(int duration, const QString& timerName);
    void createAlarm(int msecs, const QString &alarmName, bool turnedOn);
    void createSettings(GlobalSettings settings);
public slots:
    void saveLog(QList<TimerWidget*> timers, QList<alertwidget*> alarms, GlobalSettings settings);

private:

    QObject* par;

    QString toLoadFormat(const QString& str);
    QString toSaveFormat(const QString& str);

};


