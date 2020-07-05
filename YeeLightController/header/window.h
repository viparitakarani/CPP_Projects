#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <iostream>

#include <QLabel>
#include <QDebug>
#include <QSlider>
#include <QMainWindow>
#include <QPushButton>
#include <QCoreApplication>
#include <QCloseEvent>
 
 namespace Ui {
    class MainWindow;
    //enum Color;
 }
 
 //enum Color { red, green, blue };

 class MainWindow : public QMainWindow
 {
    Q_OBJECT
 public:
    explicit MainWindow(QWidget *parent = 0);

 private slots:
    void handleButtonR();
    void handleButtonG();
    void handleButtonB();
    void handleButtonRefresh();
    void handleButtonFlowStart();
    void handleButtonFlowStop();
    void handleSliderBrtReleased();
    void handleSliderBrtValueChange(int k);
    void handleSliderTempValueChange(int k);





 private:
 	QLabel *m_label;
 	QLabel *m_labelBrt;
 	QLabel *m_labelTemp;
    QPushButton *m_buttonG;
    QPushButton *m_buttonR;
    QPushButton *m_buttonB;
    QPushButton *m_buttonFlowStart;
    QPushButton *m_buttonFlowStop;
    QPushButton *m_buttonRefresh;
    QSlider *m_sliderBrt;
    QSlider *m_sliderTemp;


  private:
  	void _processBtnClrClick(QPushButton *m_button,QString label,int r,int g, int b);
  	virtual void closeEvent ( QCloseEvent * event );

  private:
  	struct Bright
	{
  		int Low= 25;
  		int Medium =50;
  		int High =100;
	};

	const Bright Brightness;
	const char* TAG ="[YeeLightController]:[MainWindow]";

    
 };
 
 #endif // MAINWINDOW_H