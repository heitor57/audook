#include "credits_window.h"
#include "ui_credits_window.h"

credits_window::credits_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::credits_window)
{
    ui->setupUi(this);
}

credits_window::~credits_window()
{
    delete ui;
}
