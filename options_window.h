#ifndef OPTIONS_WINDOW_H
#define OPTIONS_WINDOW_H

#include <QDialog>

namespace Ui {
class options_window;
}

class options_window : public QDialog
{
    Q_OBJECT

public:
    explicit options_window(QWidget *parent = 0);
    ~options_window();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::options_window *ui;
};

#endif // OPTIONS_WINDOW_H
