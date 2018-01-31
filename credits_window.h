#ifndef CREDITS_WINDOW_H
#define CREDITS_WINDOW_H

#include <QDialog>

namespace Ui {
class credits_window;
}

class credits_window : public QDialog
{
    Q_OBJECT

public:
    explicit credits_window(QWidget *parent = 0);
    ~credits_window();

private:
    Ui::credits_window *ui;
};

#endif // CREDITS_WINDOW_H
