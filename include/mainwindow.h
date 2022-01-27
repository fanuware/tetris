#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "playfield.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void openDialogOptions();
    void openDialogAbout();

private:
    Ui::MainWindow *ui;
    void resizeEvent(QResizeEvent *event) override;
};

#endif
