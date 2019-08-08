#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QFileInfo>
#include <QDebug>
#include "croprect.h"

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool isContainPoint(QPoint p);
    QPoint mapToPixmap(QPoint screenPoint);
    void SaveCrops(QString& path);


signals:

public slots:
    void undoCrop();
    void cropFinished();
    void LoadPixmap(QString &path);
    void CropBegin()
    {
        isCropping = true;
    }

public:
    QPixmap m_pixmap;
    double scalling;
    bool isLoaded;
    bool isInitalised;
    bool isCropping;
    bool isStartingCrop;
    CropRect currentRect,realRect;
    QList<CropRect> mycrops,realcrops;
    QList<QPixmap> mypixmap;

};

#endif // IMAGEVIEWER_H
