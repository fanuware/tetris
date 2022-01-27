#include "about.h"
#include "ui_about.h"
#include "applicationdefs.h"
#include <QtCore>
#include <QtGui>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About) {
    ui->setupUi(this);
    window()->layout()->setSizeConstraint(QLayout::SetFixedSize);
    setWindowIcon(QPixmap(":/res/tetris_icon.png"));

    ui->versionLabel->setText(ApplicationDefs::kVersion);
    ui->legalCopyrightLabel->setText(ApplicationDefs::kLegalCopyright);

    // connections
    connect(ui->okButton, &QPushButton::clicked, this, &About::close);
}

About::~About() {
    delete ui;
}
