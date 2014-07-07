#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    updateEnableDisable();
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

void SettingsDialog::on_closeBehaviorBox_currentIndexChanged(int)
{
    updateEnableDisable();
}

void SettingsDialog::updateEnableDisable()
{
    bool hotKeyEnabled = ui->closeBehaviorBox->currentIndex() == 1;
    ui->hotKeyLabel->setEnabled(hotKeyEnabled);
    ui->hotKeyBox->setEnabled(hotKeyEnabled);
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
    updateEnableDisable();
}

bool SettingsDialog::getCloseBehavior()
{
    return ui->closeBehaviorBox->currentIndex() == 0;
}

void SettingsDialog::setShowHotKey(QKeySequence seq)
{
    ui->hotKeyBox->setKeySequence(seq);
}

QKeySequence SettingsDialog::getShowHotKey()
{
    return ui->hotKeyBox->keySequence();
}

void SettingsDialog::setEnterCommand(QString value)
{
    ui->commandText->setText(value);
}

QString SettingsDialog::getEnterCommand()
{
    return ui->commandText->text();
}
