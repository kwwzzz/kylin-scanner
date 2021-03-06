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
#ifndef SCAN_DISPLAY_H
#define SCAN_DISPLAY_H

#include <qmath.h>
#include <QStackedLayout>
#include <QInputDialog>
#include <QScrollArea>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QToolTip>
#include <QTextEdit>
#include "kylinLbl.h"
#include "waterMarkDlg.h"
#include "funcBar.h"
#include "rectify.h"
#include "embelish.h"
#include "kylinSane.h"
#include "realTimelbl.h"
#include "interruptDlg.h"
#include "kylinLog.h"

class myThread : public QThread
{
    Q_OBJECT
public:
    void run() Q_DECL_OVERRIDE;

signals:
    void orcFinished();
};

class EditBar  :   public QWidget
{
    Q_OBJECT

public:
    EditBar(QWidget *parent = nullptr);

    void setEditBarWindowBorderRadius();
//private:
    QPushButton *btnTailor;
    QPushButton *btnRotate;
    QPushButton *btnSymmetry;
    QPushButton *btnWatermark;
    QVBoxLayout *vBoxEditBar;
};

class ScanDisplay  : public QWidget
{
    Q_OBJECT

public:
    int flagBeautify = 0; //一键美化标志
    int flagRectify = 0; //智能纠偏标志
    int flagOrc = 0; //文字识别标志
    float scaledNum = 1; //缩放倍数
    int index = 0;
    int n = 0;

    ScanDisplay(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *e);
    QImage *imageSave(QString fileName);
    void setNoDevice();
    void setInitDevice();
    void setPixmap(QImage img, QLabel *lab);
    float setPixmapScaled(QImage img, QLabel *lab);
    void updateWindowSize();

    int m_timerNum = 0; // 计时器执行次数
    int m_timeScanId; // 正常扫描结束后显示扫描结果定时器id
//    void showEvent(QShowEvent *event);
    void timerEvent(QTimerEvent *e);

private:
    QTimer *timerScan; // 实时显示扫描结果定时器
    QLabel *labInit; // 初始化界面，即空白界面
    QLabel *labConnectError; // 连接或者打开扫描仪出错界面
    QLabel *labConnectErrorText; //连接或者打开扫描仪出错界面
    QLabel *labNormalLeft;       //正常显示界面左部分
    QLabel *labNormalRight;      //正常显示界面右部分
    QLabel *labEditLayout;       //编辑栏展开界面的显示部分
    KylinLbl *labTailor;         //编辑栏
    QLabel *labOrcLeft;          //文字识别图片显示部分
    QLabel *labOrcRight;         //文字识别文字显示部分
    QPushButton *btnNormal;      //正常显示界面按钮
    QPushButton *btnEditLayout;  //编辑栏展开界面按钮
    QPushButton *btnTailor;      //裁剪界面按钮

    QImage *imgConnectError;     // 连接或者打开扫描仪出错界面显示图片
    QImage *imgEditLayout;       //编辑栏展开界面图片
    QImage *imgTailor;           //裁剪界面图片
    QImage *imgStack;            //栈中保存图片
    QImage *imgNormal;           //正常显示界面图片
    QImage *imgBackup;           //添加水印时图片
    QImage *imgBeautify;         //一键美化图片
    QImage *imgRectify;          //智能纠偏图片
    QStack<QImage> stack;        //用于保存图片
    QVBoxLayout *vBoxConnectError;
    QVBoxLayout *vBoxScanSet;
    QVBoxLayout *vBoxOrc;
    QHBoxLayout *hBoxNormal;
    QHBoxLayout *hBoxEditLayout;
    QHBoxLayout *hBoxTailor;
    QHBoxLayout *hBoxOrc;
    QWidget *widgetConnectError;
    QWidget *widgetNormal;
    QWidget *widgetEditLayout;
    QWidget *widgetTailor;
    QWidget *widgetOrc;
    QStackedLayout *vStackedLayout;
    EditBar *editLayout;
    EditBar *editLayoutTailor;
    myThread thread;
    QScrollArea *scrollArea;
    int widgetindex;
    QList<QString> list;

public slots:
    void onOrc();
    void onScan();
    void onRectify();
    void onBeautify();
    void switchPage();
    void timerScanUpdate();

private slots:
    void tailor();
    void rotating();
    void symmetry();
    void addWatermark();
    void orcText();

signals:
    void scanTimerFinished();
};

#endif // SCAN_DISPLAY_H
