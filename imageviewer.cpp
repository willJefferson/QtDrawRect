#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{

}
void ImageViewer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    if(m_pixmap.isNull())
        return;
    QPainter painter(this);
    if(isLoaded)
    {
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        QSize pixSize = m_pixmap.size();

        QSize initialSize = event->rect().size();
        //printf("initSize: x: %d, y: %d\n",initialSize.width(),initialSize.height());
        scalling = 1.0*initialSize.height()/pixSize.height();
        pixSize.scale(scalling*pixSize,Qt::KeepAspectRatio);

        //qDebug()<<"pic width: "<<pixSize.width()<<" pic height: "<<pixSize.height()<<endl;
        //this->setMinimumSize(pixSize);
        QPoint topleft;
        topleft.setX((this->width()-pixSize.width())/2);
        topleft.setY((this->height()-pixSize.height())/2);
        //qDebug()<<"pic begin x: "<<topleft.x()<<" pic begin y: "<<topleft.y()<<endl;
        painter.drawPixmap(topleft,m_pixmap.scaled(pixSize,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        if(isCropping)
        {
            QPen pen;
            pen.setBrush(Qt::red);
            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);
            painter.setPen(pen);
            for(CropRect cropRect:mycrops)
            {
                if (cropRect.startPoint().x()< cropRect.endPoint().x())
                {
                    if (cropRect.startPoint().y() < cropRect.endPoint().y())
                    {
                        painter.drawRect(cropRect.startPoint().x(), cropRect.startPoint().y(), cropRect.width(), cropRect.height());
                    }
                    else
                    {
                        painter.drawRect(cropRect.startPoint().x(), cropRect.endPoint().y(), cropRect.width(), cropRect.height() );
                    }
                }
                else
                {
                    if (cropRect.startPoint().y() > cropRect.endPoint().y())
                    {
                        painter.drawRect( cropRect.endPoint().x(), cropRect.endPoint().y(), cropRect.width(), cropRect.height());
                    }
                    else
                    {
                        painter.drawRect(cropRect.endPoint().x(), cropRect.startPoint().y(), cropRect.width(), cropRect.height());
                    }
                }
            }
            //当前框
            if (currentRect.startPoint().x()< currentRect.endPoint().x())
            {
                if (currentRect.startPoint().y() < currentRect.endPoint().y())
                {
                    painter.drawRect(currentRect.startPoint().x(), currentRect.startPoint().y(), currentRect.width(), currentRect.height());
                }
                else
                {
                    painter.drawRect(currentRect.startPoint().x(),currentRect.endPoint().y(), currentRect.width(), currentRect.height());
                }
            }
            else
            {
                if (currentRect.startPoint().y() > currentRect.endPoint().y())
                {
                    painter.drawRect(currentRect.endPoint().x(), currentRect.endPoint().y(), currentRect.width(), currentRect.height());
                }
                else
                {
                    painter.drawRect(currentRect.endPoint().x(), currentRect.startPoint().y(), currentRect.width(), currentRect.height());
                }
            }
        }

    }

}

bool ImageViewer::isContainPoint(QPoint p)
{
    QSize s = m_pixmap.size();
    s.scale(scalling*s,Qt::KeepAspectRatio);
    if((s.height()>this->rect().height())&&s.width() > this->rect().width())
        return true;
    QPoint topleft;
    topleft.setX((this->width()-s.width())/2);
    topleft.setY((this->height()-s.height())/2);
    QRect rect(topleft,s);
    return rect.contains(p);
}

QPoint ImageViewer::mapToPixmap(QPoint screenPoint)
{
    QSize pixmapSize = m_pixmap.size();
    pixmapSize.scale(scalling*pixmapSize,Qt::KeepAspectRatio);
    QPoint tmpPos;
    if(pixmapSize.width()>this->width()&&pixmapSize.height()>this->height())
    {
        tmpPos.setX(pixmapSize.width()-(this->width()-screenPoint.x()));
        tmpPos.setY(pixmapSize.height()-(this->height()-screenPoint.y()));
    }
    else if(pixmapSize.width()<this->width()&&pixmapSize.height()>this->height())
    {
        tmpPos.setX(screenPoint.x() - (this->width() - pixmapSize.width()) / 2);
        tmpPos.setY(pixmapSize.height() - (this->height() - screenPoint.y()));
    }
    else if (pixmapSize.width() > this->width() && pixmapSize.height() < this->height())
    {
        tmpPos.setX(pixmapSize.width() - (this->width() - screenPoint.x()));
        tmpPos.setY(screenPoint.y() - (this->height() - pixmapSize.height()) / 2);
    }
    else
    {
        tmpPos.setX(screenPoint.x() - (this->width() - pixmapSize.width()) / 2);
        tmpPos.setY(screenPoint.y() - (this->height() - pixmapSize.height()) / 2);
    }
    return QPoint(tmpPos.x() / scalling, tmpPos.y() / scalling);
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if((event->buttons()==Qt::LeftButton)&&isContainPoint(event->pos())&&isCropping)
    {
        realRect.setStart(mapToPixmap(event->pos()));
        realRect.setEnd(mapToPixmap(event->pos()));
        currentRect.setStart(event->pos());
        currentRect.setEnd(event->pos());
        isStartingCrop = true;
    }
}
void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons()==Qt::LeftButton)&&isStartingCrop)
    {
        if(isContainPoint(event->pos()))
        {
            realRect.setEnd(mapToPixmap(event->pos()));
            currentRect.setEnd(event->pos());
            update();


        }
    }
}
void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint a = currentRect.startPoint();
    QPoint b = currentRect.endPoint();
    //qDebug()<<"x: "<<b.x()<<" y: "<<b.y()<<endl;
    if(a.x()!=b.x()&&a.y()!=b.y())
    {
        CropRect croprect;
        croprect.setStart(currentRect.startPoint());
        croprect.setEnd(currentRect.endPoint());
        mycrops.push_back(croprect);

        croprect.setStart(realRect.startPoint());
        croprect.setEnd(realRect.endPoint());
        realcrops.push_back(croprect);
    }
    else
    {
        currentRect.reset();
        realRect.reset();
    }
    isStartingCrop = false;
}

void ImageViewer::LoadPixmap(QString &path)
{
    m_pixmap.load(path);
    isLoaded = true;
    mycrops.clear();
    mypixmap.clear();
    realcrops.clear();
    update();
}

void ImageViewer::cropFinished()
{
    for(CropRect cropRect:realcrops)
    {
        QRect crop(cropRect.startPoint(), QSize(cropRect.width(), cropRect.height()));
        QPixmap cropped = m_pixmap.copy(crop);
        mypixmap.push_back(cropped);
    }
    isCropping = false;
    currentRect.reset();
    realRect.reset();
    //this->update();
}
void ImageViewer::undoCrop()
{
    if(!isCropping)
        return;
    if(mycrops.size()==0)
    {
        currentRect.reset();
        realRect.reset();
        return;
    }
    else if(mycrops.size()==1)
    {
        mycrops.pop_back();
        realcrops.pop_back();
        currentRect.reset();
        realRect.reset();
    }
    else
    {
        mycrops.pop_back();
        realcrops.pop_back();
        currentRect = mycrops.value(mycrops.size()-1);
        realRect= realcrops.value(realcrops.size()-1);
    }
    update();
}
void ImageViewer::SaveCrops(QString &path)
{
    int size = mypixmap.size();
    if(size<1)
        return;
    QFileInfo fileinfo = QFileInfo(path);
    QString filename = fileinfo.fileName();
    QString filepath = fileinfo.absolutePath();
    QString format;
    if(filename.endsWith(".jpeg"))
    {
        format = ".jpeg";
        filename.remove(format);
    }
    else if(filename.endsWith(".bmp"))
    {
        format  = ".bmp";
        filename.remove(format);
    }
    else if(filename.endsWith(".PNG"))
    {
        format = ".PNG";
        filename.remove(format);
    }
    else
        format = tr(".jpeg");

    for(int i = 0;i<size;++i)
    {
        QPixmap mymap = mypixmap.at(i);
        QString num;
        num.setNum(i+1);
        QString CommandLine;
        CommandLine.append(filepath).append("/").append(filename).append(num).append(format);
        mymap.save(CommandLine);
    }
    mycrops.clear();
    mypixmap.clear();
    realcrops.clear();
    update();
}

