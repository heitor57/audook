#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCursor>


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool fileselected = false;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void run();

    int sync_progress_bar(QTextCursor txt_cursor);

    double get_progress_double(QTextCursor txt_cursor);

    void on_actionAbout_triggered();

    void on_actionCredits_triggered();

    void on_actionOptions_triggered();

signals:
    void text_progress_changed(int progress);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
