#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csv.h"

#include <QtDebug>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    refreshDataModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshDataModel()
{
    QString dataSourceFile = "/home/andy/Downloads/jobs-example.csv";
    QList<QStringList> csv = CSV::parseFromFile(dataSourceFile);
    if (csv.length() < 2) {
        showError(QString("Invalid CSV file: %1").arg(dataSourceFile));
        return;
    }

    // first row is headers
    QStringList headers = csv.at(0);
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setRowCount(csv.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < csv.size(); row += 1) {
        QStringList rowStringList = csv.at(row);
        for (int col = 0; col < rowStringList.size() && col < headers.size(); col += 1) {
            QString value = rowStringList.at(col);
            QTableWidgetItem *item = new QTableWidgetItem(value);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    showError("");
}

void MainWindow::showError(QString err)
{
    if (err.isEmpty() || err.isNull()) {
        ui->errorLabel->hide();
    } else {
        ui->errorLabel->setText(err);
        ui->errorLabel->show();
    }
}
