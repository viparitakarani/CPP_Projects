#include "../header/lampcontroller.h"
#include "../header/window.h"



#define BOOL_STR(b) (b?"true":"false")


 
 MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
 {
    // Create the button, make "this" the parent
    m_label     = new QLabel("Click to Play With The Room Colour!", this);
    m_labelBrt  = new QLabel("Brightness", this);
    m_labelTemp = new QLabel("Temperature", this);

    m_buttonR    = new QPushButton("RED", this);
    m_buttonG    = new QPushButton("GREEN", this);
    m_buttonB    = new QPushButton("BLUE", this);
    m_buttonRefresh     = new QPushButton("REFRESH", this);
    m_buttonFlowStart   = new QPushButton("START", this);
    m_buttonFlowStop    = new QPushButton("STOP", this);
    m_sliderBrt            = new QSlider(Qt::Horizontal,this);
    m_sliderTemp           = new QSlider(Qt::Horizontal,this);



   

    // set size and location of the button
    this->setGeometry(QRect(QPoint(100, 100),QSize(500, 500)));
    m_label->setGeometry(QRect(QPoint(25, 2),QSize(300, 50)));

    m_buttonR->setGeometry(QRect(QPoint(20, 45),QSize(70, 50)));
    m_buttonG->setGeometry(QRect(QPoint(120, 45),QSize(70, 50)));
    m_buttonB->setGeometry(QRect(QPoint(220, 45),QSize(70, 50)));

    m_buttonFlowStart->setGeometry(QRect(QPoint(320, 45),QSize(70, 50)));
    m_buttonFlowStop->setGeometry(QRect(QPoint(420, 45),QSize(70, 50)));

    m_labelBrt->setGeometry(QRect(QPoint(25, 85),QSize(300, 50)));
    m_sliderBrt->setGeometry(QRect(QPoint(25, 110),QSize(200, 50)));

    m_labelTemp->setGeometry(QRect(QPoint(25, 140),QSize(300, 50)));
    m_sliderTemp->setGeometry(QRect(QPoint(25, 160),QSize(200, 50)));
    
    m_buttonRefresh->setGeometry(QRect(QPoint(20, 230),QSize(100, 50)));


    //m_slider->setFocusPolicy(Qt::StrongFocus);
    //m_slider->setTickPosition(QSlider::TicksBothSides);
    m_sliderBrt->setTickInterval(100);
    m_sliderBrt->setSingleStep(1);

    m_sliderTemp->setTickInterval(100);
    m_sliderTemp->setSingleStep(1);

 
    // Connect button signal to appropriate slot
    connect(m_buttonR, SIGNAL (released()), this, SLOT (handleButtonR()));
    connect(m_buttonG, SIGNAL (released()), this, SLOT (handleButtonG()));
    connect(m_buttonB, SIGNAL (released()), this, SLOT (handleButtonB()));
    connect(m_buttonRefresh, SIGNAL (released()), this, SLOT (handleButtonRefresh()));

    connect(m_buttonFlowStart, SIGNAL (released()), this, SLOT (handleButtonFlowStart()));
    connect(m_buttonFlowStop, SIGNAL (released()), this, SLOT (handleButtonFlowStop()));
    connect(m_sliderBrt,SIGNAL (valueChanged(int)),this,SLOT (handleSliderBrtValueChange(int)));
    connect(m_sliderBrt,SIGNAL (sliderReleased()),this,SLOT (handleSliderBrtReleased()));

    connect(m_sliderTemp,SIGNAL (valueChanged(int)),this,SLOT (handleSliderTempValueChange(int)));





 }

 void MainWindow::_processBtnClrClick(QPushButton *m_button,QString label,int r,int g, int b)
 {

    m_label->setText(QString("Changing colour to %1....").arg(label));

    m_button->setText("Connecting....");

    LampController lampController("192.168.1.33", "55443"); 
    bool isSuccess = lampController.SetLampRGBColour(r,g,b,"smooth",1500);


    m_button->setText(QString("%1").arg(label));
    if(isSuccess)
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Yeaa!! It successfully changing colour to %1..!!").arg(label));
    }
    else
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Fail! Click to try again!"));
    }
    
    // resize button
    m_button->resize(70,50);

 }

 void MainWindow::handleButtonFlowStart()
 {
    m_label->setText(QString("Setting Lamp FLow Mode...."));

    m_buttonFlowStart->setText("Connecting....");

    LampController lampController("192.168.1.33", "55443"); 

    //Deprecated
    //bool isSuccess = lampController.SetLampMusic(1,"192.168.1.24",54321);

    bool isSuccess = lampController.StartLampColourFlow(0,0,QString("100, 2, 2700, 100, 100, 1, 255, 10, 100, 7, 0,0, 100, 2, 5000, 1"));


    m_buttonFlowStart->setText(QString("START"));
    if(isSuccess)
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Yeaa!! It successfully set  flow mode on"));
    }
    else
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Fail! Click to try again!"));
    }
    
    // resize button
    m_buttonFlowStart->resize(70,50);
 }

void MainWindow::handleButtonFlowStop()
 {
    m_label->setText(QString("Stop Lamp FLow Mode...."));

    m_buttonFlowStop->setText("Connecting....");

    LampController lampController("192.168.1.33", "55443"); 

    //Deprecated
    //bool isSuccess = lampController.SetLampMusic(1,"192.168.1.24",54321);

    bool isSuccess = lampController.StopLampColourFlow();


    m_buttonFlowStop->setText(QString("STOP"));
    if(isSuccess)
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Yeaa!! It successfully set  flow mode off"));
    }
    else
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Fail! Click to try again!"));
    }
    
    // resize button
    m_buttonFlowStop->resize(70,50);
 }


 void MainWindow::handleButtonR()
 {

    this->_processBtnClrClick(this->m_buttonR,QString("RED"),240,17,79);
 }


 void MainWindow::handleButtonG()
 {
    this->_processBtnClrClick(this->m_buttonG,QString("GREEN"),17,157,157);
 }


 void MainWindow::handleButtonB()
 {
    this->_processBtnClrClick(this->m_buttonB,QString("BLUE"),10,78,199);
 }


 void MainWindow::handleButtonRefresh()
 {
    m_label->setText(QString("Refresh Lamp...."));

    LampController lampController("192.168.1.33", "55443"); 

    bool isSuccess = lampController.SetLampPower("off","sudden",100);
         isSuccess = lampController.SetLampPower("on","sudden",100);

    if(isSuccess)
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Yeaa!! It successfully refresh the lamp"));
    }
    else
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Fail! Click to try again!"));
    }
    
 }


  void MainWindow::handleSliderTempValueChange(int k)
 {
    //m_label->setText(QString("Changing Brightness of the Lamp...."));
    if(k<=1)
        k=1;
    if (k>=100)
        k=99;

    k=1700+(k*48);
    LampController lampController("192.168.1.33", "55443"); 
    bool isSuccess = lampController.SetLampColourTemperature(k,"sudden",100);


    //printf("%s : %d\n", TAG,k);

    if(isSuccess)
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Temperature= %1 ").arg(k));
    }
    else
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Fail! Slide to try again!"));
    }
   
 }

 void MainWindow::handleSliderBrtValueChange(int k)
 {
    //m_label->setText(QString("Changing Brightness of the Lamp...."));
    if(k<=1)
        k=1;
    if (k>=100)
        k=99;

    LampController lampController("192.168.1.33", "55443"); 
    bool isSuccess = lampController.SetLampBrightness(k,"sudden",100);


    //printf("%s : %d\n", TAG,k);

    if(isSuccess)
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Brightness= %1 %").arg(k));
    }
    else
    {
        //m_label->resize(200,50);
        m_label->setText(QString("Fail! Slide to try again!"));
    }
   
 }


 void MainWindow::handleSliderBrtReleased()
 {
    
    //LampController lampController("192.168.1.33", "55443"); 
    //bool isSuccess = lampController.SetLampBrightness(k,"sudden",100);


    printf("%s : %d\n", TAG,m_sliderBrt->value());
   
 }



//@Overide 
void MainWindow::closeEvent(QCloseEvent *event )
{
    //! Ignore the event by default.. otherwise the window will be closed always.
    event->ignore();
    LampController lampController("192.168.1.33", "55443"); 
    bool isSuccess = lampController.SetLampRGBColour(245,245,167,"smooth",1000);
    isSuccess = lampController.SetLampBrightness(Brightness.High,"smooth",1000);
    isSuccess = lampController.SetLampHSV(359,70,"smooth",1000);
    isSuccess = lampController.SetLampColourTemperature(6000,"smooth",1000);
    

    //printf("%s : %d\n", TAG,Brightness.High);
    qDebug() << QString("%1:[closeEvent]:%2").arg(TAG).arg(Brightness.High); 



    event->accept();
}