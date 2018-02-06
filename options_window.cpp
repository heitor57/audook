#include "options_window.h"
#include "ui_options_window.h"
#include "mainwindow.h"
extern char *langNativeString;

options_window::options_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::options_window)
{
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(langNativeString));
}

options_window::~options_window()
{
    delete ui;
}

void options_window::on_buttonBox_accepted()
{
    string fn = ui->comboBox->currentText().toStdString();
    langNativeString = new char [fn.size()+1];
    strcpy( langNativeString, fn.c_str());
    this->close();
}

void options_window::on_buttonBox_rejected()
{
    this->close();
}
