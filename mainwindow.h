#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"

#include <QMainWindow>
#include <QStringList>
#include <QList>

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

private:
    Ui::MainWindow *ui;

    void refreshDataModel();
    void filterSearch();
    void showSettings();
    void showError(QString err);

    SettingsDialog *settingsDialog;

    QList<QStringList> csv;
};

#endif // MAINWINDOW_H
