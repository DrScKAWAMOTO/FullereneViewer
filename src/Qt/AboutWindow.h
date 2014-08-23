#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QMainWindow>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = 0);
    ~AboutWindow();
    static void showAbout();

protected:
  virtual void closeEvent(QCloseEvent *);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
