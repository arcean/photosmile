#ifndef DECLARATIVEIMAGE_H
#define DECLARATIVEIMAGE_H

#include <QDeclarativeItem>

class DeclarativeImage : public QDeclarativeItem
{
    Q_OBJECT

public:
    explicit DeclarativeImage(QDeclarativeItem *parent = 0);
    ~DeclarativeImage();

protected slots:


private:

};

#endif // DECLARATIVEIMAGE_H
