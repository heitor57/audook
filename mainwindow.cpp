#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <fstream>
#include <string>
#include "extension.h"
#include <iostream>
#include <QFileInfo>
#include "TextExtractor.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#ifdef _HAVE_CONFIG
#include <config.h>
#endif // _HAVE_CONFIG

// txt extractor
int txt_extract( const char* argv ){

    TextExtractor extractor;

    try {
        extractor.Init( argv );
    } catch( PdfError & e ) {
        fprintf( stderr, "Error: An error %i ocurred during processing the pdf file.\n", e.GetError() );
        e.PrintErrorMsg();
        return e.GetError();
    }

    return 0;
}
bool fileExists(QString path) {
    QFileInfo check_file(path);
    return check_file.exists();
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setText("Arquivo");
    ui->pushButton_2->setCheckable(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),"/home",tr("(*.txt *.pdf)"));
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(nullptr,"info",file.errorString());
    }

    if(ext_name(path.toStdString().c_str()) == "pdf"){
        if(fileExists(ext_switch(path,"txt"))){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Question", "A file with this name: ("+ext_switch(path,"txt")+") already exists, do you want to overwrite?(Is needed)",
            QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No) {
                return;
            }else{
                ofstream ftemp;
                ftemp.open((ext_switch(path,"txt")).toStdString());
                ftemp<< "";
                ftemp.close();
            }
        }
        txt_extract(path.toStdString().c_str());
        QFile file2in(QString::fromStdString(ext_switch(path.toStdString(),"txt")));
        if(!file2in.open(QIODevice::ReadOnly)){
            QMessageBox::information(nullptr,"info",file.errorString());
        }
        QTextStream in(&file2in);
        ui->textBrowser->setText(in.readAll());
    }else{
        QTextStream in(&file);
        ui->textBrowser->setText(in.readAll());
    }




}



void MainWindow::on_pushButton_2_clicked()
{

}

void MainWindow::on_pushButton_2_toggled(bool checked)
{

}

