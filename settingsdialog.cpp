#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_sourceFileButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose data source", QString(), "CSV Files (*.csv)");
    if (filename.isNull())
        return;
    ui->sourceFileText->setText(filename);
}

void SettingsDialog::setDataSource(QString value)
{
    ui->sourceFileText->setText(value);
}

QString SettingsDialog::getDataSource()
{
    return ui->sourceFileText->text();
}

void SettingsDialog::setCloseBehavior(bool value)
{
    ui->closeBehaviorBox->setCurrentIndex(value ? 0 : 1);
}

bool SettingsDialog::getCloseBehavior()
{
    return ui->closeBehaviorBox->currentIndex() == 0;
}

void SettingsDialog::setShowHotKey(QKeySequence seq)
{

}

QKeySequence SettingsDialog::getShowHotKey()
{
    return QKeySequence();
}

void SettingsDialog::setEnterCommand(QString value)
{
    ui->commandText->setText(value);
}

QString SettingsDialog::getEnterCommand()
{
    return ui->commandText->text();
}
