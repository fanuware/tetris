#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "options.h"
#include "about.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowIcon(QPixmap(":/res/tetris_icon.png"));

    ui->frame->addLabels(ui->lblPreview, ui->lblLevel, ui->lblScore);
    QPalette pal (ui->lblPreview->palette());
    pal.setColor(QPalette::WindowText,Qt::lightGray);
    ui->lblPreview->setPalette(pal);
    ui->lblPreview->setFrameStyle(QLabel::Box);

    ui->btnStartGame->setFocusPolicy(Qt::NoFocus);
    ui->btnStopGame->setFocusPolicy(Qt::NoFocus);

    // connections
    connect(ui->btnStartGame, SIGNAL(clicked()), ui->frame, SLOT(startGame()));
    connect(ui->btnStopGame, SIGNAL(clicked()), ui->frame, SLOT(stopGame()));
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::openDialogOptions);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::openDialogAbout);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openDialogOptions() {
    Options *options = new Options(ui->frame);
    options->setWindowTitle(tr("Options"));
    options->exec();
    delete options;
}

void MainWindow::openDialogAbout() {
    About *options = new About();
    options->setWindowTitle(tr("About"));
    options->exec();
    delete options;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (event->spontaneous()) {
        QRect geometry = ui->playFieldContainer->geometry();
        ui->frame->containerResized(geometry.width(), geometry.height());
    }
}
