#ifndef CROPRECT_H
#define CROPRECT_H
#include <QPoint>
#include <QSize>

class CropRect
{
public:
    CropRect();
    ~CropRect();
    void setStart(QPoint s);
    void setEnd(QPoint e);
    QPoint startPoint();
    QPoint endPoint();
    void reset();
    QSize size();
    int height();
    int width();

private:
    QPoint start;
    QPoint end;
};

#endif // CROPRECT_H
