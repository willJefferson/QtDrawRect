#include "savepic.h"

Savepic::Savepic(QWidget *parent)
    : QWidget(parent)
{
    loadPicBtn = new QPushButton(this);
    loadPicBtn->setText(tr("载入图片"));
    cropBeginBtn = new QPushButton(this);
    cropBeginBtn->setText(tr("剪裁图片"));
    undoCropBtn = new QPushButton(this);
    undoCropBtn->setText(tr("撤销剪裁"));
    cropEndBtn = new QPushButton(this);
    cropEndBtn->setText(tr("剪裁完成"));
    saveBtn = new QPushButton(this);
    saveBtn->setText(tr("保存图片"));
    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(loadPicBtn);
    btnLayout->addWidget(cropBeginBtn);
    btnLayout->addWidget(undoCropBtn);
    btnLayout->addWidget(cropEndBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->setSpacing(20);

    imageviewer = new ImageViewer(this);
    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(imageviewer);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    mainLayout->setStretchFactor(btnLayout,1);
    mainLayout->setStretchFactor(imageviewer,9);

    connect(loadPicBtn,SIGNAL(clicked()),this,SLOT(OpenPic()));
    connect(cropBeginBtn,SIGNAL(clicked()),imageviewer,SLOT(CropBegin()));
    connect(undoCropBtn,SIGNAL(clicked()),imageviewer,SLOT(undoCrop()));
    connect(cropEndBtn,SIGNAL(clicked()),imageviewer,SLOT(cropFinished()));
    connect(saveBtn,SIGNAL(clicked()),this,SLOT(SaveCrop()));
    connect(this,SIGNAL(Sig_loadPixmap(QString&)),imageviewer,SLOT(LoadPixmap(QString&)));
    resize(1000,800);
}
void Savepic::OpenPic()
{
    QString s = QFileDialog::getOpenFileName(this,"select outputfile","D:/video/","jpeg files(*.jpeg)::bmp files(*.bmp)::PNG files(*.PNG)::All files(*.*)");
    if(s.isEmpty())
        return;
    this->setWindowTitle(s);
    emit Sig_loadPixmap(s);

}

void Savepic::SaveCrop()
{
    QString s = QFileDialog::getSaveFileName(this,"select outputfile","D:/video/","jpeg files(*.jpeg)::bmp files(*.bmp)::PNG files(*.PNG)::All files(*.*)");
    if(s.isEmpty())
        return;
    imageviewer->SaveCrops(s);
}
void Savepic::changeEvent(QEvent * event)
{
    if(event->type()!=QEvent::WindowStateChange)
        return;
    if(this->windowState()==Qt::WindowMinimized)
    {
        this->resize(1000,800);
    }
}

Savepic::~Savepic()
{

}

