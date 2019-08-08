#include "croprect.h"

CropRect::CropRect()
{

}
CropRect::~CropRect()
{

}
void CropRect::setStart(QPoint s)
{
    start = s;
}
void CropRect::setEnd(QPoint e)
{
    end = e;
}
QPoint CropRect::startPoint()
{
    return start;
}
QPoint CropRect::endPoint()
{
    return end;
}
void CropRect::reset()
{
    QPoint P(0,0);
    start = P;
    end = P;
}
QSize CropRect::size()
{
    return QSize(width(),height());
}
int CropRect::height()
{
    return qAbs(start.y()-end.y());
}
int CropRect::width()
{
    return qAbs(start.x()-end.x());
}
