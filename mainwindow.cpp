#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csv.h"

#include <QtDebug>
#include <QDir>
#include <QApplication>
#include <QKeySequence>
#include <QTextStream>

enum UserRoles {
    ExecCommand = Qt::UserRole + 1
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->hide();

    settingsDialog = new SettingsDialog(this);

    connect(settingsDialog, SIGNAL(accepted()), this, SLOT(acceptNewSettings()));

    QString settingsFilePath = QDir(QApplication::applicationDirPath()).relativeFilePath("ruff.ini");
    settings = new QSettings(settingsFilePath, QSettings::IniFormat, this);

    loadSettings();
    restoreGeometry(settings->value("windowGeometry").toByteArray());
    restoreState(settings->value("windowState").toByteArray());
    refreshDataModel();
    ui->tableWidget->horizontalHeader()->restoreState(settings->value("tableState").toByteArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshDataModel()
{
    QString dataSourceFile = settingsDialog->getDataSource();
    if (dataSourceFile.isNull() || dataSourceFile.isEmpty()) {
        showError("Select your data source in the settings dialog.");
        return;
    }

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
            if (!item) continue;
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

void MainWindow::loadSettings()
{
    QString dataSource = settings->value("dataSource", "").toString();
    bool closeBehavior = settings->value("closeBehavior", true).toBool();
    QKeySequence showHotKey = QKeySequence::fromString(settings->value("showHotKey", "Ctrl+Alt+R").toString());
    QString enterCommand = settings->value("enterCommand", "").toString();

    settingsDialog->setDataSource(dataSource);
    settingsDialog->setCloseBehavior(closeBehavior);
    settingsDialog->setShowHotKey(showHotKey);
    settingsDialog->setEnterCommand(enterCommand);

}

static void parseCommand(QString sourceCommand, QString &exe, QStringList &args)
{
    /*
    QTextStream bufStream;
    enum State {Normal, Quote} state = Normal;
    bool isProgram = true;
    QChar quoteType;
    for (int i = 0; i < sourceCommand.size(); i += 1) {
        QChar c = sourceCommand.at(i);
        switch (state) {
        case Normal:
            break;
        case Quote:
            break;
        }
    }*/
    args = sourceCommand.split(" ", QString::SkipEmptyParts);
    exe = args.takeFirst();
}

void MainWindow::openSelectedItem()
{
    QString sourceCommand = settingsDialog->getEnterCommand();

    QString exe;
    QStringList args;
    parseCommand(sourceCommand, exe, args);

    QProcess *process = new QProcess(this);
    process->setProperty("cmd", sourceCommand);
    process->startDetached(exe, args);
    connect(process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(displayProcessError(QProcess::ProcessError)));
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

void MainWindow::on_searchBox_textChanged(const QString &)
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

void MainWindow::acceptNewSettings()
{

    settings->setValue("dataSource", settingsDialog->getDataSource());
    settings->setValue("closeBehavior", settingsDialog->getCloseBehavior());
    settings->setValue("showHotKey", settingsDialog->getShowHotKey());
    settings->setValue("enterCommand", settingsDialog->getEnterCommand());

    settings->sync();

    refreshDataModel();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings->setValue("windowGeometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->setValue("tableState", ui->tableWidget->horizontalHeader()->saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::on_openButton_clicked()
{
    openSelectedItem();
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *)
{
    openSelectedItem();
}

void MainWindow::displayProcessError(QProcess::ProcessError error)
{
    QString errDesc;
    switch (error) {
    case QProcess::FailedToStart:
        errDesc = "file not found or permissions problem";
        break;
    case QProcess::Crashed:
        errDesc = "crashed";
        break;
    case QProcess::Timedout:
        errDesc = "timed out";
        break;
    case QProcess::WriteError:
        errDesc = "write problem";
        break;
    case QProcess::ReadError:
        errDesc = "read problem";
        break;
    default:
        errDesc = "unknown";
        break;
    }
    QProcess *process = dynamic_cast<QProcess*>(QObject::sender());
    QString cmd = process->property("cmd").toString();
    showError(QString("Error executing %1: %2").arg(cmd, errDesc));
}
