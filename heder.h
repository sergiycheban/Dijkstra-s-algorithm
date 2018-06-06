#ifndef HEDER_H
#define HEDER_H

#include <QWidget>

namespace Ui {
class Heder;
}

class Heder : public QWidget
{
    Q_OBJECT

public:
    explicit Heder(QWidget *parent = 0);
    ~Heder();

private:
    Ui::Heder *ui;
};

#endif // HEDER_H
