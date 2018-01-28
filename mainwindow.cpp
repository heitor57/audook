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

#include <future>
#include <thread>
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

// SPEAK
#include <malloc.h>
#include <espeak/speak_lib.h>

espeak_POSITION_TYPE position_type;
espeak_AUDIO_OUTPUT output;
char *path=NULL;
int Buflength = 500, Options=0;
void* user_data;
t_espeak_callback *SynthCallback;
espeak_PARAMETER Parm;
unsigned int Size,position=0, end_position=0, flags=espeakCHARS_AUTO, *unique_identifier;

void speak(const char spoken_text[]){
    output = AUDIO_OUTPUT_PLAYBACK;
        int I, Run = 1, L;
        espeak_Initialize(output, Buflength, path, Options );
        const char *langNativeString = "pt";
        espeak_VOICE voice;
        memset(&voice, 0, sizeof(espeak_VOICE)); // Zero out the voice first
        voice.languages = langNativeString;
        voice.variant = 6;
        voice.gender = 1;
        espeak_SetVoiceByProperties(&voice);
        Size = strlen(spoken_text)+1;
        espeak_Synth( spoken_text, Size, position, position_type, end_position, flags,
        unique_identifier, user_data );
        espeak_Synchronize( );
        espeak_Terminate();
}

// SPEAK
void MainWindow::run(){
    QTextCursor txt_cursor = ui->plainTextEdit->textCursor();
    if(ui->pushButton_2->isChecked()){
        txt_cursor.select(QTextCursor::WordUnderCursor);
        ui->plainTextEdit->setFocus();
        ui->plainTextEdit->setTextCursor(txt_cursor);
        speak(txt_cursor.selectedText().toStdString().c_str());
        while(txt_cursor.movePosition(QTextCursor::NextWord,QTextCursor::MoveAnchor,1) && ui->pushButton_2->isChecked()){
            txt_cursor.select(QTextCursor::WordUnderCursor);
            ui->plainTextEdit->setFocus();
            ui->plainTextEdit->setTextCursor(txt_cursor);
            speak(txt_cursor.selectedText().toStdString().c_str());
        }
    }
    if(!txt_cursor.movePosition(QTextCursor::NextWord,QTextCursor::MoveAnchor,1))
        ui->pushButton_2->setChecked(false);
}
void MainWindow::on_pushButton_2_clicked()
{
    std::thread t1(&MainWindow::run,this);
    t1.detach();
}

void MainWindow::on_pushButton_2_toggled(bool checked)
{

}

