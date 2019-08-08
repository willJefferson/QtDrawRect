#ifndef SAVEPIC_H
#define SAVEPIC_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QEvent>
#include "imageviewer.h"

class Savepic : public QWidget
{
    Q_OBJECT

public:
    Savepic(QWidget *parent = 0);
    void changeEvent(QEvent * event);
    ~Savepic();
signals:
    void Sig_loadPixmap(QString &path);
private:
    QPushButton *loadPicBtn,*cropBeginBtn,*undoCropBtn,*cropEndBtn,*saveBtn;
    QHBoxLayout *btnLayout;
    QVBoxLayout *mainLayout;
    ImageViewer *imageviewer;

public slots:
    void OpenPic();
    void SaveCrop();
};

#endif // SAVEPIC_H
