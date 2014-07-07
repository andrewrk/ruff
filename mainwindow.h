#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void on_searchBox_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;

    void refreshDataModel();
    void filterSearch();
    void showError(QString err);

    QList<QStringList> csv;
};

#endif // MAINWINDOW_H
