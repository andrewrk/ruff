#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"

#include <QMainWindow>
#include <QStringList>
#include <QList>
#include <QSettings>
#include <QCloseEvent>
#include <QProcess>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionFocusFind_triggered();

    void on_actionClose_triggered();

    void on_closeButton_clicked();

    void on_searchBox_textChanged(const QString &arg1);

    void on_settingsButton_clicked();

    void on_actionSettings_triggered();

    void acceptNewSettings();

    void on_openButton_clicked();

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *);

    void displayProcessError(QProcess::ProcessError error);

    void handleProcessFinished(int, QProcess::ExitStatus exitStatus);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    void refreshDataModel();
    void filterSearch();
    void showSettings();
    void showError(QString err);
    void loadSettings();
    void openSelectedItem();

    SettingsDialog *settingsDialog;

    QSettings *settings;

    QList<QStringList> csv;
};

#endif // MAINWINDOW_H
