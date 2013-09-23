/*
 * QtGLAppBase - Simple Qt Application to get started with OpenGL stuff.
 *
 * Copyright (c) 2011-2012 Martin Felis <martin.felis@iwr.uni-heidelberg.de>
 *
 * Licensed under the MIT license. See LICENSE for more details.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QTimeLine>
#include "ui_MainWindow.h"

class QtGLBaseApp : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
 
public:
    QtGLBaseApp(QWidget *parent = 0);

protected:
		QTimer *draw_timer;

public slots:
		void camera_changed ();
		void update_camera ();
		void action_quit();
};
 
#endif
