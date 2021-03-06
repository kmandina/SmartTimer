#ifndef CHANGEALARMDIALOG_H
#define CHANGEALARMDIALOG_H

#include "alertwidget.h"
#include "widgetsettings.h"

#include <QDialog>

namespace Ui {
class ChangeAlarmDialog;
}

class ChangeAlarmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeAlarmDialog(alertwidget *parent = nullptr);
    ~ChangeAlarmDialog();

    void updateWidget(GlobalSettings _globalSettings);
public slots:
    void changeAlarm();
    void updateTimeTo(QTime time);
signals:
    void changeAlarmSignal(WidgetSettings settings);

private:
    Ui::ChangeAlarmDialog *ui;
    GlobalSettings globalSettings;
};

#endif // CHANGEALARMDIALOG_H
