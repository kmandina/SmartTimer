#include "timerwidget.h"
#include "ui_timerwidget.h"

#include "changetimerdialog.h"

#include <iostream>
#include <string>

#include <QFile>
#include <QTextFormat>

int TimerWidget::MAXID = 0;

TimerWidget::TimerWidget(WidgetSettings settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimerWidget)
{
    ui->setupUi(this);

    Settings = settings;

    QFile file(":/stylesheet.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        this->setStyleSheet(file.readAll());
        file.close();
    }

    timer = new QTimer(this);
    tickTimer = new QTimer(this);
    blinkTimer = new QTimer(this);

    timeLeft = 0;

    blinky = false;

    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl(Settings.signalPath));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player = new QMediaPlayer();
    player->setPlaylist(playlist);


    ui->intervalTime->setText(QString::fromStdString(secondsToTimeString(Settings.msecs/1000)));
    ui->timeLeft->setText(QString::fromStdString(secondsToTimeString(Settings.msecs/1000)));
    ui->TimerName->setText(Settings.name);

    connect(timer,SIGNAL(timeout()),this, SLOT(timerExecuted()));
    connect(tickTimer,SIGNAL(timeout()),this,SLOT(updateLeftTime()));
    connect(blinkTimer,SIGNAL(timeout()),this, SLOT(blink()));

    connect(ui->startButton, SIGNAL(clicked()),this, SLOT(startTimer()));
    connect(ui->restartButton, SIGNAL(clicked()),this, SLOT(resetTimer()));
    connect(ui->deleteTimerButton,SIGNAL(released()),this, SLOT(closeTimer()));

    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(changeTimer()));




    ui->restartButton->setDisabled(true);
    ui->editButton->setEnabled(true);
    ui->startButton->setEnabled(true);

    id = MAXID+1;
    TimerWidget::MAXID++;

}

TimerWidget::~TimerWidget()
{
    delete ui;
}

QString TimerWidget::getTimerName()
{
    return Settings.name;
}

int TimerWidget::getTimerDuration()
{
    return Settings.msecs;
}

void TimerWidget::setTimerName(const QString &_name)
{
    Settings.name = _name;
    ui->TimerName->setText(_name);
}

void TimerWidget::setTimerDuration(int _duration)
{
    Settings.msecs = _duration;

    timer->stop();
    tickTimer->stop();

    ui->startButton->setEnabled(true);
    ui->editButton->setEnabled(true);
    ui->restartButton->setDisabled(true);

    ui->intervalTime->setText(QString::fromStdString(secondsToTimeString(Settings.msecs/1000)));
    ui->timeLeft->setText(QString::fromStdString(secondsToTimeString(Settings.msecs/1000)));
}

void TimerWidget::updateWidget(GlobalSettings _globalSettings)
{
    globalSettings = _globalSettings;
    ui->intervalTime->setText((QTime::fromMSecsSinceStartOfDay(Settings.msecs)).toString(globalSettings.timerTimeFormat));

    //ui->timeLeft->setText((QTime::fromMSecsSinceStartOfDay(Settings.msecs)).toString(globalSettings.timerTimeFormat));
}

void TimerWidget::resetTimer()
{
    timer->stop();
    tickTimer->stop();
    blinkTimer->stop();


    if (player->state())
        player->stop();

    timeLeft = 0;
    ui->timeLeft->setText(QString::fromStdString(secondsToTimeString(Settings.msecs/1000)));

    ui->restartButton->setDisabled(true);
    ui->startButton->setEnabled(true);
    ui->editButton->setEnabled(true);

    ui->restartButton->setStyleSheet("");

    emit blinkInfo("Timers",false);
}

void TimerWidget::timerExecuted()
{
    //std::cerr << getID() << " " << "timer executed!";
    player->play();
    blinkTimer->start(350);

    timer->stop();
    tickTimer->stop();

    ui->timeLeft->setText(QString::fromStdString(secondsToTimeString(0)));

    emit timerFinished();
    emit blinkInfo("Timers",true);
}

void TimerWidget::updateLeftTime()
{
    if(timeLeft >= 0)
    {
        std::string time = secondsToTimeString(timeLeft);
        ui->timeLeft->setText(QString::fromStdString(time));

        timeLeft--;
    }
}

void TimerWidget::changeTimer()
{
    auto *changeDial = new ChangeTimerDialog(this);

    changeDial->exec();
}

void TimerWidget::blink()
{
    if(blinky)
    {
        ui->restartButton->setStyleSheet("");

        blinky = false;
    }
    else
    {
        ui->restartButton->setStyleSheet("background-color: rgb(245, 121, 0)");
        blinky = true;
    }
}


void TimerWidget::closeTimer()
{
    if (blinkTimer->isActive())
        emit blinkInfo("Timers",false);

    player->stop();
    timer->stop();
    tickTimer->stop();
    blinkTimer->stop();



    emit del(this);



    this->close();
}



void TimerWidget::startTimer()
{
    timeLeft = Settings.msecs/1000-1;
    tickTimer->start(1000);
    timer->start(Settings.msecs);
    ui->startButton->setDisabled(true);
    ui->editButton->setDisabled(true);
    ui->restartButton->setEnabled(true);
}

std::string TimerWidget::secondsToTimeString(int val)
{
    int tmp = val/3600;
    std::string time = (tmp<10)?("0"+std::to_string(tmp)):std::to_string(tmp);
    time+=":";
    tmp = (val%3600)/60;
    time += (tmp<10)?("0"+std::to_string(tmp)):std::to_string(tmp);
    time+=":";
    tmp = val%60;
    time += (tmp<10)?("0"+std::to_string(tmp)):std::to_string(tmp);

    return time;
}
