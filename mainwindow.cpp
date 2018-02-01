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
#include "json_control.h"
#include <future>
#include <thread>
#include "about_window.h"
#include "credits_window.h"
#ifdef _HAVE_CONFIG
#include <config.h>
#endif // _HAVE_CONFIG

extern string txt_content;
QString fpath;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setText("Arquivo");
    ui->pushButton_2->setCheckable(true);
    ui->plainTextEdit->setReadOnly(false);
    connect(this, SIGNAL(text_progress_changed(int)),
               ui->progressBar, SLOT(setValue(int)));
}


MainWindow::~MainWindow()
{
    if(!fpath.isEmpty()){
        edit_save(fpath,get_progress_double(ui->plainTextEdit->textCursor()));
    }
    delete ui;
}

int MainWindow::sync_progress_bar(QTextCursor txt_cursor){
    return 100*(float)txt_cursor.position()/ui->plainTextEdit->toPlainText().size();
}
double MainWindow::get_progress_double(QTextCursor txt_cursor){
    return 100*(float)txt_cursor.position()/ui->plainTextEdit->toPlainText().size();
}
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

void MainWindow::on_pushButton_clicked()
{
    if(!fpath.isEmpty()){
        edit_save(fpath,get_progress_double(ui->plainTextEdit->textCursor()));
    }
    ui->plainTextEdit->document()->setPlainText("");
    fpath = QFileDialog::getOpenFileName(this,tr("Open File"),"/home",tr("(*.txt *.pdf)"));
    QFile file(fpath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(nullptr,"info",file.errorString());
        return;
    }
    if(ext_name(fpath.toStdString().c_str()) == "pdf"){
        txt_extract(fpath.toStdString().c_str());
        ui->plainTextEdit->document()->setPlainText(QString::fromStdString(txt_content));
        txt_content.clear();
    }else{
        QTextStream in(&file);
        ui->plainTextEdit->document()->setPlainText(in.readAll());
    }

    if(exist_save(fpath)){
        QTextCursor txt_cursor = ui->plainTextEdit->textCursor();
        emit text_progress_changed(get_progress(fpath));
        txt_cursor.setPosition((get_progress(fpath)/100)*ui->plainTextEdit->toPlainText().size(),QTextCursor::MoveAnchor);
        ui->plainTextEdit->setFocus();
        ui->plainTextEdit->setTextCursor(txt_cursor);
    }else{
        add_new_save(fpath,0);
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
        emit text_progress_changed(sync_progress_bar(txt_cursor));
        speak(txt_cursor.selectedText().toStdString().c_str());

        while(txt_cursor.movePosition(QTextCursor::NextWord,QTextCursor::MoveAnchor,1) && ui->pushButton_2->isChecked()){
            txt_cursor.select(QTextCursor::WordUnderCursor);
            ui->plainTextEdit->setFocus();
            ui->plainTextEdit->setTextCursor(txt_cursor);
            emit text_progress_changed(sync_progress_bar(txt_cursor));
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

void MainWindow::on_actionAbout_triggered()
{
    about_window* w=  new about_window(this);
    w->setModal(true);
    w->show();

}

void MainWindow::on_actionCredits_triggered()
{
    credits_window* w=  new credits_window(this);
    w->setModal(true);
    w->show();
}
