#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "extension.h"
#include "TextExtractor.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <fstream>
#include <string>
#include <iostream>
#include <QFileInfo>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <QScrollBar>
#include <QTextCursor>
#ifdef _HAVE_CONFIG
#include <config.h>
#endif // _HAVE_CONFIG

extern string txt_content;
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
    ui->plainTextEdit->setReadOnly(false);
    //connect(ui->plainTextEdit->verticalScrollBar(), SIGNAL(valueChanged(int)),
               // ui->progressBar, SLOT(set_progress(int)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->plainTextEdit->document()->setPlainText("");
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),"/home",tr("(*.txt *.pdf)"));
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(nullptr,"info",file.errorString());
    }

    if(ext_name(path.toStdString().c_str()) == "pdf"){
        txt_extract(path.toStdString().c_str());
        ui->plainTextEdit->document()->setPlainText(QString::fromStdString(txt_content));
        txt_content.clear();
    }else{
        QTextStream in(&file);
        ui->plainTextEdit->document()->setPlainText(in.readAll());
    }



}


void MainWindow::on_pushButton_2_clicked()
{
    QTextCursor txt_cursor = ui->plainTextEdit->textCursor();
    cerr << txt_cursor.position();
    ui->plainTextEdit->setFocus();
    txt_cursor.setPosition(txt_cursor.position()+1 ,QTextCursor::KeepAnchor);
    ui->plainTextEdit->setTextCursor(txt_cursor);

}

void MainWindow::on_pushButton_2_toggled(bool checked)
{

}

