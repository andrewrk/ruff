#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QKeySequence>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();


    void setDataSource(QString value);
    QString getDataSource();

    void setCloseBehavior(bool value);
    bool getCloseBehavior();

    void setShowHotKey(QKeySequence seq);
    QKeySequence getShowHotKey();

    void setEnterCommand(QString value);
    QString getEnterCommand();

private slots:
    void on_sourceFileButton_clicked();

private:

    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
