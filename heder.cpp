#include "heder.h"
#include "ui_heder.h"

Heder::Heder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Heder)
{
    ui->setupUi(this);
}

Heder::~Heder()
{
    delete ui;
}
