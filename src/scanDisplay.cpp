/*
* Copyright (C) 2020, KylinSoft Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
*
*/
#include "scanDisplay.h"

QString outText;

ScanDisplay::ScanDisplay(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(600,567);
    labInit = new QLabel();
    labConnectError = new QLabel();
    labConnectErrorText = new QLabel();
    labNormalLeft = new QLabel();
    labNormalRight = new QLabel();
    labEditLayout = new QLabel();

    timerScan = new QTimer(this);

    btnNormal = new QPushButton();
    btnEditLayout = new QPushButton();

    widgetConnectError = new QWidget();
    widgetNormal = new QWidget();
    widgetEditLayout = new QWidget();

    imgConnectError = new QImage();
    imgEditLayout = new QImage();
    imgStack = new QImage();
    imgNormal = new QImage();
    imgBeautify = new QImage();
    imgRectify = new QImage();
    vBoxConnectError = new QVBoxLayout();
    vBoxScanSet = new QVBoxLayout(this);
    hBoxNormal = new QHBoxLayout();
    hBoxEditLayout = new QHBoxLayout();

    vStackedLayout = new QStackedLayout();

    labConnectError->setParent(widgetConnectError);
    labConnectErrorText->setParent(widgetConnectError);
    labInit->setMinimumSize(600,567);
    labConnectError->setMinimumSize(600,320);
    labConnectErrorText->setMinimumSize(600,231);
    labNormalLeft->setMinimumSize(360,490);
    labNormalRight->setFixedWidth(40);
    labEditLayout->setMinimumSize(360,490);

    labInit->setText(" ");

    imgConnectError->load(":/icon/icon/connect_device.png");
    labConnectError->setPixmap(QPixmap::fromImage(*imgConnectError));
    labConnectError->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);

    QFont ft;
    ft.setPointSize(24);
    labConnectErrorText->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(232,232,232));
    labConnectErrorText->setPalette(pa);
    labConnectErrorText->setText(tr("Please connect to a scan device firstly !"));
    labConnectErrorText->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    vBoxConnectError->setSpacing(0);
    vBoxConnectError->addStretch();
    vBoxConnectError->addWidget(labConnectError);
    vBoxConnectError->addSpacing(16);
    vBoxConnectError->addWidget(labConnectErrorText);
    vBoxConnectError->addStretch();
    vBoxConnectError->setContentsMargins(0,0,0,0);
    widgetConnectError->setLayout(vBoxConnectError);

    labNormalLeft->setParent(widgetNormal);
    labNormalRight->setParent(widgetNormal);
    btnNormal->setParent(widgetNormal);
    btnNormal->setFixedSize(12,30);
    btnNormal->setStyleSheet("QPushButton{ "
                                 "border-image: url(:/icon/icon/editBar/shrink-normal.svg);"
                                 "border:none;"
                                 "background-color:#0f0801;"
                                 "border-radius:0px;"
                                 "}");

    labNormalLeft->setAlignment(Qt::AlignCenter);
    labNormalRight->setStyleSheet("QLabel{background-color:#0f0801;}");

    hBoxNormal->setSpacing(0);
    hBoxNormal->addSpacing(93);
    hBoxNormal->addStretch();
    hBoxNormal->addWidget(labNormalLeft);
    hBoxNormal->addStretch();
    hBoxNormal->addSpacing(93);
    hBoxNormal->addWidget(labNormalRight);
    hBoxNormal->addSpacing(2);
    hBoxNormal->addWidget(btnNormal);
    hBoxNormal->setContentsMargins(0,45,0,32);
    widgetNormal->setLayout(hBoxNormal);


    editLayout = new EditBar();
    labEditLayout->setParent(widgetEditLayout);
    btnEditLayout->setParent(widgetEditLayout);
    editLayout->setParent(widgetEditLayout);


    labEditLayout->setAlignment(Qt::AlignCenter);


    btnEditLayout->setFixedSize(12,30);
    btnEditLayout->setStyleSheet("QPushButton{border-image: url(:/icon/icon/editBar/shrink-editLayout.svg);border:none;background-color:#0f0801;border-radius:0px;}"
                                 );

    hBoxEditLayout->setSpacing(0);
    hBoxEditLayout->addSpacing(93);
    hBoxEditLayout->addStretch();
    hBoxEditLayout->addWidget(labEditLayout);
    hBoxEditLayout->addStretch();
    hBoxEditLayout->addSpacing(93);
    hBoxEditLayout->addWidget(editLayout);
    hBoxEditLayout->addSpacing(2);
    hBoxEditLayout->addWidget(btnEditLayout);
    hBoxEditLayout->setContentsMargins(0,45,0,32);
    widgetEditLayout->setLayout(hBoxEditLayout);

    vStackedLayout->addWidget(widgetNormal);
    vStackedLayout->addWidget(widgetEditLayout);
    vStackedLayout->addWidget(widgetConnectError);
    vStackedLayout->addWidget(labInit);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(15, 8, 1));
    setAutoFillBackground(true);
    setPalette(pal);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    vBoxScanSet->setSpacing(0);
    vBoxScanSet->addLayout(vStackedLayout);
    vBoxScanSet->setContentsMargins(0,0,0,0);
    setLayout(vBoxScanSet);
    vStackedLayout->setCurrentWidget(labInit);
    vStackedLayout->setContentsMargins(0,0,0,0);

    // For switch page
    connect(btnNormal,SIGNAL(clicked()),this,SLOT(switchPage()));
    connect(btnEditLayout,SIGNAL(clicked()),this,SLOT(switchPage()));

    //For rotate
    connect(editLayout->btnRotate,SIGNAL(clicked()),this,SLOT(rotating()));

    // For tailor
    connect(editLayout->btnTailor,SIGNAL(clicked()),this,SLOT(tailor()));

    // For symmetry
    connect(editLayout->btnSymmetry,SIGNAL(clicked()),this,SLOT(symmetry()));

    // For watermark
    connect(editLayout->btnWatermark,SIGNAL(clicked()),this,SLOT(addWatermark()));

    // For ORC
    connect(&thread,SIGNAL(orcFinished()),this,SLOT(orcText()));

    // For timerScan
    connect(timerScan, SIGNAL(timeout()), this, SLOT(timerScanUpdate()));
}

void ScanDisplay::keyPressEvent(QKeyEvent *e)
{
    if (((e->key() == Qt::Key_Return) || (e->key() == 0x20)) && (vStackedLayout->currentWidget() == widgetTailor))
    {
        QImage newimage;
        int x1,y1,x2,y2;
        if(labTailor->getX1() <= labTailor->getX2())
        {
             x1 = labTailor->getX1() - ((labTailor->width() - imgEditLayout->width()*scaledNum) / 2);
             x2 = labTailor->getX2() - ((labTailor->width() - imgEditLayout->width()*scaledNum) / 2);
        }
        else
        {
            x1 = labTailor->getX2() - ((labTailor->width() - imgEditLayout->width()*scaledNum) / 2);
            x2 = labTailor->getX1() - ((labTailor->width() - imgEditLayout->width()*scaledNum) / 2);
        }

        if(labTailor->getY1() <= labTailor->getY2())
        {
            y1 = labTailor->getY1() - ((labTailor->height() - imgEditLayout->height()*scaledNum) / 2);
            y2 = labTailor->getY2() - ((labTailor->height() - imgEditLayout->height()*scaledNum) / 2);
        }
        else
        {
            y1 = labTailor->getY2() - ((labTailor->height() - imgEditLayout->height()*scaledNum) / 2);
            y2 = labTailor->getY1() - ((labTailor->height() - imgEditLayout->height()*scaledNum) / 2);
        }

        newimage = imgEditLayout->copy(x1/scaledNum,y1/scaledNum,(x2-x1)/scaledNum,(y2-y1)/scaledNum);
        *imgEditLayout = newimage;
        setPixmapScaled(*imgEditLayout,labEditLayout);
        *imgNormal = imgEditLayout->copy();
        setPixmapScaled(*imgNormal,labNormalLeft);
        vStackedLayout->setCurrentIndex(index);
        vStackedLayout->removeWidget(widgetTailor);

    }
    if(e->key() == Qt::Key_Z && e->modifiers() ==  Qt::ControlModifier)
    {
        if(!stack.isEmpty())
        {
            *imgEditLayout = stack.pop();
            setPixmapScaled(*imgEditLayout,labEditLayout);
            *imgNormal = imgEditLayout->copy();
            setPixmapScaled(*imgNormal,labNormalLeft);
            vStackedLayout->setCurrentIndex(index);
        }
    }
}

QImage *ScanDisplay::imageSave(QString fileName)
{
    if(flagOrc == 0)
    {
        *imgEditLayout = imgNormal->copy();
        if(fileName.endsWith(".pdf"))
            return imgEditLayout;
        if(fileName.endsWith(".png") || fileName.endsWith(".jpg") || fileName.endsWith(".bmp"))
            imgEditLayout->save(fileName);
    }
    else {
        if(!fileName.endsWith(".txt"))
            fileName += ".txt";
        QFile file(fileName);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QByteArray str = outText.toUtf8();
        file.write(str);
        file.close();
    }
    return NULL;
}

/**
 * @brief ScanDisplay::setNoDevice
 * 设置无设备时的页面widget
 * 对设计新的等待页面有作用
 */
void ScanDisplay::setNoDevice()
{
    vStackedLayout->setCurrentWidget(widgetConnectError);
}

/**
 * @brief ScanDisplay::setInitDevice
 * 设置初始化页面widget
 */
void ScanDisplay::setInitDevice()
{
    vStackedLayout->setCurrentWidget(labInit);
}

void ScanDisplay::setPixmap(QImage img, QLabel *lab)
{
    int width = lab->width();
    int height = lab->height();
    QPixmap pixmap = QPixmap::fromImage(img);
    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);  // 按比例缩放
    lab->setPixmap(fitpixmap);
    lab->setAlignment(Qt::AlignCenter);
}

float ScanDisplay::setPixmapScaled(QImage img, QLabel *lab)
{
//    float labWidth = lab->width();
//    float labHeight = lab->height();
    float labWidth = this->width() - 240;
    float labHeight = this->height() - 77;
    float imgWidth = img.width();
    float imgHeight = img.height();
    float num = 1;
    if((labWidth/imgWidth) <= (labHeight/imgHeight))
        num = labWidth/imgWidth;
    else
        num = labHeight/imgHeight;
    int width,height;
    width = imgWidth * num;
    height = imgHeight * num;
    QPixmap pixmap = QPixmap::fromImage(img);
    QPixmap fitpixmap = pixmap.scaled(width, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);  // 按比例缩放
    lab->setPixmap(fitpixmap);
    lab->setAlignment(Qt::AlignCenter);
    return num;
}

void ScanDisplay::updateWindowSize()
{
    setPixmapScaled(*imgNormal,labNormalLeft);
    setPixmapScaled(*imgEditLayout,labEditLayout);
}


void ScanDisplay::timerEvent(QTimerEvent *e)
{
    int id = e->timerId();

    if (id == m_timeScanId)
    {
        MYLOG << "timeScanId";
//            labNormalLeft->height = 490;

//            labNormalLeft->data = NULL; //图像信息
//            labNormalLeft->update();
    }
}

void ScanDisplay::rotating()
{

    QMatrix matrix;
    *imgStack = imgEditLayout->copy();
    stack.push(*imgStack);
    matrix.rotate(270);
    *imgEditLayout = imgEditLayout->transformed(matrix);
    setPixmapScaled(*imgEditLayout,labEditLayout);
    *imgNormal = imgEditLayout->copy();
    setPixmapScaled(*imgNormal,labNormalLeft);

}

void ScanDisplay::symmetry()
{
    *imgStack = imgEditLayout->copy();
    stack.push(*imgStack);
    *imgEditLayout=imgEditLayout->mirrored(true,false);
    setPixmapScaled(*imgEditLayout,labEditLayout);
    *imgNormal = imgEditLayout->copy();
    setPixmapScaled(*imgNormal,labNormalLeft);
}

void ScanDisplay::addWatermark()
{

    if(n == 0)
    {
        n = 1;
        imgBackup = new QImage();
        *imgBackup = imgEditLayout->copy();
    }
    QString text;
    WaterMarkDialog *dialog = new WaterMarkDialog(this);
    int ret=dialog->exec();// 以模态方式显示对话框，用户关闭对话框时返回 DialogCode值
    MYLOG << "ret = " << ret;

    if (ret==QDialog::Accepted) //OK键被按下,对话框关闭，若设置了setAttribute(Qt::WA_DeleteOnClose)，对话框被释放，无法获得返回值
    { //OK键被按下，获取对话框上的输入，设置行数和列数
        text = dialog->getLineEdit();
        MYLOG << text;

        *imgStack = imgEditLayout->copy();
        stack.push(*imgStack);
        *imgEditLayout = imgBackup->copy();
        QPainter painter(imgEditLayout);
        int fontSize = 70, spacing = 20;
        QFont font("华文黑体", fontSize, QFont::Thin);
        QColor colorFont(1, 1, 1);
        //colorFont.setAlphaF(0.4); // 设置水印字体颜色透明度,这个会导致字体不显示等一系列错误
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        painter.setFont(font);
        painter.setPen(colorFont);
        //painter.translate(img2->width() / 2, -img2->width() / 2);//调整位置
        painter.rotate(15);

        int squareEdgeSize = imgEditLayout->width() * sin(45) + imgEditLayout->height() * sin(45);//对角线长度
        int hCount = squareEdgeSize / ((fontSize + spacing) * (text.size() + 1)) + 1;
        int x = squareEdgeSize / hCount + (fontSize + spacing) * 3;
        int y = x / 2;

        for (int i = 0; i < hCount; i++)
        {
            for (int j = 0; j < hCount * 2; j++)
            {
               painter.drawText(x * i, y * j,text);
            }
        }
        setPixmapScaled(*imgEditLayout,labEditLayout);
        *imgNormal = imgEditLayout->copy();
        setPixmapScaled(*imgNormal,labNormalLeft);
    }
    delete dialog; //删除对话框

}

void ScanDisplay::orcText()
{
    labOrcRight->setText(outText);
    MYLOG <<outText;
}

void ScanDisplay::onOrc()
{
    if(flagOrc == 0)
    {
        flagOrc = 1;
        widgetindex = vStackedLayout->currentIndex();
        labOrcLeft = new QLabel();
        labOrcRight = new QLabel();
        imgBackup = new QImage();
        vBoxOrc = new QVBoxLayout();
        hBoxOrc = new QHBoxLayout();
        widgetOrc = new QWidget();
        scrollArea = new QScrollArea();

        labOrcLeft->setFixedWidth(120);
        *imgEditLayout = imgNormal->copy();
        imgEditLayout->save("/tmp/scanner/scan1.png");
        *imgBackup = imgEditLayout->copy();
        thread.start();
        *imgBackup = imgBackup->scaled(120,166);
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(192,253,123,100));
        labOrcLeft->setPalette(palette);
        labOrcLeft->setPixmap(QPixmap::fromImage(*imgBackup));
        labOrcLeft->setAlignment(Qt::AlignTop);

        outText = tr("Try to ocr ...");
        QFont ft1;
        ft1.setPointSize(14);
        labOrcRight->setFont(ft1);
        QPalette pa1;
        pa1.setColor(QPalette::WindowText,QColor(255,255,255));
        labOrcRight->setPalette(pa1);
        labOrcRight->setText(outText);
        labOrcRight->setStyleSheet("background-color:#2f2c2b;border:1px solid #717171;");
        labOrcRight->setMargin(20);
        labOrcRight->setAlignment(Qt::AlignTop);
        labOrcRight->setWordWrap(true);
        labOrcLeft->setParent(widgetOrc);
        labOrcRight->setParent(widgetOrc);
        vBoxOrc->setSpacing(0);
        vBoxOrc->addWidget(labOrcRight);
        vBoxOrc->setContentsMargins(0,0,0,0);
        scrollArea->setParent(widgetOrc);
        QWidget *widget = new QWidget();

        widget->setMinimumHeight(labOrcRight->height());
        widget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint); // 去掉标题栏,去掉任务栏显示，窗口置顶
        widget->setLayout(vBoxOrc);
        widget->setContentsMargins(0,0,0,0);
        scrollArea->setFixedWidth(392);
        scrollArea->setMinimumHeight(503);
        scrollArea->setWidget(widget);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setStyleSheet("QScrollBar:vertical"
                                  "{"
                                  "width:6px;"
                                  "background:#2f2c2b;"
                                  "margin:0px,0px,0px,0px;"
                                  "padding-top:0px;"
                                  "padding-bottom:0px;"
                                  "padding-right:3px"
                                  "}"
                                  "QScrollBar::handle:vertical"
                                  "{"
                                  "width:6px;"
                                  "background:rgba(255,255,255,50%);"
                                  " border-radius:3px;"
                                  "min-height:20;"
                                  "}"
                                  "QScrollBar::handle:vertical:hover"
                                  "{"
                                  "width:6px;"
                                  "background:rgba(255,255,255,80%);"
                                  " border-radius:3px;"
                                  "min-height:20;"
                                  "}"
                                  "QScrollBar::add-line:vertical"
                                  "{"
                                  "height:0px;width:0px;"
                                  "}"
                                  "QScrollBar::sub-line:vertical"
                                  "{"
                                  "height:0px;width:0px;"
                                  "subcontrol-position:top;"
                                  "}"
                                  );
        hBoxOrc->setSpacing(0);
        hBoxOrc->addStretch();
        hBoxOrc->addWidget(labOrcLeft);
        hBoxOrc->addSpacing(24);
        hBoxOrc->addWidget(scrollArea);
        hBoxOrc->addStretch();
        hBoxOrc->setContentsMargins(32,32,32,32);
        widgetOrc->setLayout(hBoxOrc);
        vStackedLayout->addWidget(widgetOrc);
        vStackedLayout->setCurrentWidget(widgetOrc);
    }
    else
    {
        flagOrc = 0;
        vStackedLayout->setCurrentIndex(widgetindex);
    }
}

/**
 * @brief scan_display::scan 扫描功能结束后显示图片
 */
void ScanDisplay::onScan()
{
    vStackedLayout->setCurrentIndex(0);
    vStackedLayout->setCurrentIndex(1);
    vStackedLayout->setCurrentIndex(2);
    vStackedLayout->setCurrentIndex(3);

    // 重新扫描时，应该都初始化，避免撤销到之前的图片情况
    stack.clear();
    list.clear();
    flagOrc = 0;
    flagRectify = 0;
    flagBeautify = 0;

    timerScan->start(100);

    imgNormal->load("/tmp/scanner/scan.pnm");
    setPixmapScaled(*imgNormal,labNormalLeft);
    vStackedLayout->setCurrentWidget(widgetNormal);
    *imgEditLayout = imgNormal->copy();
    setPixmapScaled(*imgEditLayout,labEditLayout);
}

/**
 * @brief scan_display::rectify 智能纠偏
 */
void ScanDisplay::onRectify()
{
    MYLOG <<"rectify\n";
    if(flagRectify == 0)
    {
        // 此时代表用户点击了智能纠偏按钮
        flagRectify = 1;
        if(vStackedLayout->currentWidget() == widgetNormal)
        {
            imgNormal->save("/tmp/scanner/scan1.png");
            // 为了撤销
            *imgRectify = imgNormal->copy();
            list.append("Rectify");
            ImageRectify("/tmp/scanner/scan1.png");
            imgNormal->load("/tmp/scanner/scan1.png");
            setPixmapScaled(*imgNormal,labNormalLeft);
            *imgEditLayout = imgNormal->copy();
            setPixmapScaled(*imgEditLayout,labEditLayout);
        }
        else
        {
            imgEditLayout->save("/tmp/scanner/scan1.png");
            *imgRectify = imgEditLayout->copy();
            list.append("Rectify");
            ImageRectify("/tmp/scanner/scan1.png");
            imgEditLayout->load("/tmp/scanner/scan1.png");
            setPixmapScaled(*imgEditLayout,labEditLayout);
            *imgNormal = imgEditLayout->copy();
            setPixmapScaled(*imgNormal,labNormalLeft);
        }
    }
    else
    {
        // 此时代表用户重复点击智能纠偏按钮，应该进行撤销操作
        flagRectify = 0;
        if(vStackedLayout->currentWidget() == widgetNormal)
        {
            *imgNormal = imgRectify->copy();
            /**
             * 2代表点击了先智能纠偏和后一键美化：
             * 有2种情况： 1）先撤销一键美化 2）先撤销智能纠偏
             */
            if(list.count() == 2)
            {
                // 撤销的是智能纠偏：先全部清空，后把一键美化的加上
                if(list[0] == "Rectify")
                {
                    list.clear();
                    imgNormal->save("/tmp/scanner/scan1.png");
                    *imgBeautify = imgNormal->copy();
                    list.append("Beautify");
                    oneClickEmbelish("/tmp/scanner/scan1.png");
                    imgNormal->load("/tmp/scanner/scan1.png");
                }
                else
                {
                    // 撤销的是一键美化：撤销到之前一个就行
                    list.removeLast();
                }
            }
            else
                list.clear();
            setPixmapScaled(*imgNormal,labNormalLeft);
            *imgEditLayout = imgNormal->copy();
            setPixmapScaled(*imgEditLayout,labEditLayout);
        }
        else
        {
            *imgEditLayout = imgRectify->copy();
            if(list.count() == 2)
            {
                if(list[0] == "Rectify")
                {
                    list.clear();
                    imgEditLayout->save("/tmp/scanner/scan1.png");
                    *imgBeautify = imgEditLayout->copy();
                    list.append("Beautify");
                    oneClickEmbelish("/tmp/scanner/scan1.png");
                    imgEditLayout->load("/tmp/scanner/scan1.png");
                }
                else
                    list.removeLast();
            }
            else
                list.clear();
            setPixmapScaled(*imgEditLayout,labEditLayout);
            *imgNormal = imgEditLayout->copy();
            setPixmapScaled(*imgNormal,labNormalLeft);
        }
    }
}

/**
 * @brief scan_display::beautify 一键美化
 */
void ScanDisplay::onBeautify()
{
    MYLOG << "beauty()";
    if(flagBeautify == 0)
    {
        flagBeautify = 1;
        if(vStackedLayout->currentWidget() == widgetNormal)
        {
            imgNormal->save("/tmp/scanner/scan1.png");
            *imgBeautify = imgNormal->copy();
            list.append("Beautify");
            oneClickEmbelish("/tmp/scanner/scan1.png");
            imgNormal->load("/tmp/scanner/scan1.png");
            setPixmapScaled(*imgNormal,labNormalLeft);
            *imgEditLayout = imgNormal->copy();
            setPixmapScaled(*imgEditLayout,labEditLayout);
        }
        else
        {
            imgEditLayout->save("/tmp/scanner/scan1.png");
            *imgBeautify = imgEditLayout->copy();
            list.append("Beautify");
            oneClickEmbelish("/tmp/scanner/scan1.png");
            imgEditLayout->load("/tmp/scanner/scan1.png");
            setPixmapScaled(*imgEditLayout,labEditLayout);
            *imgNormal = imgEditLayout->copy();
            setPixmapScaled(*imgNormal,labNormalLeft);
        }
    }
    else
    {
        flagBeautify = 0;
        if(vStackedLayout->currentWidget() == widgetNormal)
        {
            *imgNormal = imgBeautify->copy();
            if(list.count() == 2)
            {
                if(list[0] == "Beautify")
                {
                    list.clear();
                    imgNormal->save("/tmp/scanner/scan1.png");
                    *imgRectify = imgNormal->copy();
                    list.append("Rectify");
                    ImageRectify("/tmp/scanner/scan1.png");
                    imgNormal->load("/tmp/scanner/scan1.png");
                }
                else
                    list.removeLast();
            }
            else
                list.clear();
            setPixmapScaled(*imgNormal,labNormalLeft);
            *imgEditLayout = imgNormal->copy();
            setPixmapScaled(*imgEditLayout,labEditLayout);
        }
        else
        {
            *imgEditLayout = imgBeautify->copy();
            if(list.count() == 2)
            {
                if(list[0] == "Beautify")
                {
                    list.clear();
                    imgEditLayout->save("/tmp/scanner/scan1.png");
                    *imgRectify = imgEditLayout->copy();
                    list.append("Rectify");
                    ImageRectify("/tmp/scanner/scan1.png");
                    imgEditLayout->load("/tmp/scanner/scan1.png");
                }
                else
                    list.removeLast();
            }
            else
                list.clear();
            setPixmapScaled(*imgEditLayout,labEditLayout);
            *imgNormal = imgEditLayout->copy();
            setPixmapScaled(*imgNormal,labNormalLeft);
        }
    }
}

/**
 * @brief scan_display::switchPage 用于右侧工具栏切换页面
 */
void ScanDisplay::switchPage()
{
    index++;
    if(index > 1)
    {
        MYLOG << "1 switchPage index = " << index;
        index = 0;
        *imgNormal = imgEditLayout->copy();
        setPixmapScaled(*imgNormal,labNormalLeft);
    }
    else
    {
        MYLOG << "2 switchPage index = " << index;
        *imgEditLayout = imgNormal->copy();
        setPixmapScaled(*imgEditLayout,labEditLayout);
    }
    vStackedLayout->setCurrentIndex(index);
}

/**
 * @brief ScanDisplay::timerScanUpdate 定时器事件
 */
void ScanDisplay::timerScanUpdate()
{
    m_timerNum++;
    MYLOG << m_timerNum << "timer";
    if (m_timerNum == 50)
    {
        timerScan->stop();
        MYLOG << "timer stop";
        emit scanTimerFinished();
    }
}

/**
 * @brief scan_display::tailor 工具栏裁切、裁剪
 */
void ScanDisplay::tailor()
{
    btnTailor = new QPushButton();
    labTailor = new KylinLbl();
    editLayoutTailor = new EditBar();
    widgetTailor = new QWidget();
    hBoxTailor = new QHBoxLayout();
    imgTailor = new QImage();
    labTailor->setMinimumSize(360,490);
    labTailor->setParent(widgetTailor);
    btnTailor->setParent(widgetTailor);
    editLayoutTailor->setParent(widgetTailor);
    labTailor->setAlignment(Qt::AlignCenter);

    btnTailor->setFixedSize(12,30);
    btnTailor->setStyleSheet("QPushButton{border-image: url(:/icon/icon/editBar/shrink-editLayout.svg);border:none;background-color:#0f0801;border-radius:0px;}");

    hBoxTailor->setSpacing(0);
    hBoxTailor->addSpacing(93);
    hBoxTailor->addStretch();
    hBoxTailor->addWidget(labTailor);
    hBoxTailor->addStretch();
    hBoxTailor->addSpacing(93);
    hBoxTailor->addWidget(editLayoutTailor);
    hBoxTailor->addSpacing(2);
    hBoxTailor->addWidget(btnTailor);
    hBoxTailor->setContentsMargins(0,45,0,32);
    widgetTailor->setLayout(hBoxTailor);
    vStackedLayout->addWidget(widgetTailor);
    vStackedLayout->setCurrentWidget(widgetTailor);
    *imgTailor = imgEditLayout->copy();
    *imgStack = imgEditLayout->copy();
    stack.push(*imgStack);
    scaledNum = setPixmapScaled(*imgTailor,labTailor);
}

/**
 * @brief edit_bar::edit_bar 工具栏
 * @param parent
 */
EditBar::EditBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(40,220);
    vBoxEditBar = new QVBoxLayout(this);
    btnRotate = new QPushButton();
    btnTailor = new QPushButton();
    btnSymmetry = new QPushButton();
    btnWatermark = new QPushButton();

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(232, 232, 232));
    setAutoFillBackground(true);
    setPalette(pal);

    setEditBarWindowBorderRadius();

    btnRotate->setFixedSize(30,30);
    btnTailor->setFixedSize(30,30);
    btnSymmetry->setFixedSize(30,30);
    btnWatermark->setFixedSize(30,30);
    btnRotate->setToolTip(tr("rotate")); // 旋转
    btnTailor->setToolTip(tr("tailor")); // 裁切
    btnWatermark->setToolTip(tr("watermark")); // 水印
    btnSymmetry->setToolTip(tr("symmetry")); // 对称翻转

    btnRotate->setStyleSheet("QPushButton{border-image: url(:/icon/icon/editBar/rotate.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                              "QPushButton:hover{border-image: url(:/icon/icon/editBar/rotate-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                                "QPushButton:checked{border-image: url(:/icon/icon/editBar/rotate-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}");
    btnTailor->setStyleSheet("QPushButton{border-image: url(:/icon/icon/editBar/tailor.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                              "QPushButton:hover{border-image: url(:/icon/icon/editBar/tailor-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                                "QPushButton:checked{border-image: url(:/icon/icon/editBar/tailor-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}");
    btnSymmetry->setStyleSheet("QPushButton{border-image: url(:/icon/icon/editBar/symmetry.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                              "QPushButton:hover{border-image: url(:/icon/icon/editBar/symmetry-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                                "QPushButton:checked{border-image: url(:/icon/icon/editBar/symmetry-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}");
    btnWatermark->setStyleSheet("QPushButton{border-image: url(:/icon/icon/editBar/watermark.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                              "QPushButton:hover{border-image: url(:/icon/icon/editBar/watermark-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}"
                                "QPushButton:checked{border-image: url(:/icon/icon/editBarwatermark-click.svg);border:none;background-color:rgb(232,232,232);border-radius:0px;}");
    vBoxEditBar->setSpacing(0);
    vBoxEditBar->addSpacing(17);
    vBoxEditBar->addWidget(btnTailor);
    vBoxEditBar->addSpacing(22);
    vBoxEditBar->addWidget(btnRotate);
    vBoxEditBar->addSpacing(22);
    vBoxEditBar->addWidget(btnSymmetry);
    vBoxEditBar->addSpacing(22);
    vBoxEditBar->addWidget(btnWatermark);
    vBoxEditBar->addSpacing(17);
    vBoxEditBar->setContentsMargins(5,0,5,0);
    setLayout(vBoxEditBar);

}

/**
 * @brief EditBar::setEditBarWindowBorderRadius 设置窗口圆角
 */
void EditBar::setEditBarWindowBorderRadius()
{
    QBitmap bitMap(width(),height()); // A bit map has the same size with current widget
    bitMap.fill();
    QPainter painter(&bitMap);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen); // Any color that is not QRgb(0,0,0) is right
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(bitMap.rect(),4,4); //设置圆角弧度4px
    setMask(bitMap);
}

/**
 * @brief myThread::run 文字识别线程
 */
void myThread::run()
{
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    MYLOG <<"orc run!\n";
    //使用中文初始化tesseract-ocr，而不指定tessdata路径。正在识别中
    if (api->Init(NULL, "chi_sim")) {
        MYLOG <<"Could not initialize tesseract.\n";
        outText = "Unable to read text";
        exit(1);
    }
    // 使用leptonica库打开输入图像。
    Pix* image = pixRead("/tmp/scanner/scan1.png");
    if(!image)
    {
        MYLOG <<"pixRead error!";
        outText = "Unable to read text";
        emit orcFinished();
        // 销毁使用过的对象并释放内存。
        api->End();
       // pixDestroy(&image);
        quit();
    }
    api->SetImage(image);
    // 得到光学字符识别结果
    outText = api->GetUTF8Text();
    emit orcFinished();
    // 销毁使用过的对象并释放内存。
    api->End();
    pixDestroy(&image);
    quit();
}
