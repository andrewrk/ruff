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
    ui->menuBar->hide();
    refreshDataModel();

    settingsDialog = new SettingsDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshDataModel()
{
    QString dataSourceFile = "/home/andy/Downloads/jobs-example.csv";
    csv = CSV::parseFromFile(dataSourceFile);
    if (csv.length() < 2) {
        showError(QString("Invalid CSV file (expected >= 2 rows): %1").arg(dataSourceFile));
        return;
    }

    // first row is headers
    QStringList headers = csv.at(0);
    if (headers.size() < 1) {
        showError(QString("Invalid CSV file (expected >= 1 column): %1").arg(dataSourceFile));
        return;
    }

    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setRowCount(csv.size() - 1);
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < csv.size() - 1; row += 1) {
        QStringList rowStringList = csv.at(row + 1);
        for (int col = 0; col < rowStringList.size() && col < headers.size(); col += 1) {
            QString value = rowStringList.at(col);
            QTableWidgetItem *item = new QTableWidgetItem(value);
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->sortItems(0);
    ui->tableWidget->setSortingEnabled(true);
    showError("");
    filterSearch();
}

void MainWindow::filterSearch()
{
    QStringList terms = ui->searchBox->text().split(" ", QString::SkipEmptyParts);

    for (int row = 0; row < ui->tableWidget->rowCount(); row += 1) {
        bool hide = true;
        for (int col = 0; col < ui->tableWidget->columnCount(); col += 1) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QString value = item->text();
            bool match = true;
            foreach (QString term, terms) {
                if (value.indexOf(term, 0, Qt::CaseInsensitive) == -1) {
                    match = false;
                    break;
                }
            }
            if (match) {
                hide = false;
                break;
            }
        }
        ui->tableWidget->setRowHidden(row, hide);
    }
}

void MainWindow::showSettings()
{
    settingsDialog->show();
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

void MainWindow::on_actionFocusFind_triggered()
{
    ui->searchBox->selectAll();
    ui->searchBox->setFocus();
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::on_searchBox_textChanged(const QString &arg1)
{
    filterSearch();
}

void MainWindow::on_settingsButton_clicked()
{
    showSettings();
}

void MainWindow::on_actionSettings_triggered()
{
    showSettings();
}
