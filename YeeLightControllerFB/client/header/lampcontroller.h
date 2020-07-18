#ifndef LAMCONTROLLER_H
#define LAMCONTROLLER_H


#include <cmath>
#include <iostream>
#include <QString>
#include <QTcpSocket>
#include <QTimer>
#include <QDateTime>



class LampController
{
    private:
        QString ip;
        QString port;
        const char* TAG ="[YeeLightController]:[LampController]";


    private:
        void _setDebugMsg(QString msg);
        void _setDebugMsg(QString msg,QString method);
        bool _SendCommand(QString cmdJson,QString conf,QString method);
        bool _SendCommand(QString cmdJson,QString conf,QString action,QString method);
  

    public:
        LampController(QString ipadd, QString portadd); 

        bool GetLampConnection();

        bool SetLampRGBColour(int R, int G, int B, QString effect, int duration);
        bool SetLampHSV(int hue, int sat, QString effect, int duration);

        bool SetLampColourTemperature(int temperature, QString effect, int duration);
        bool SetLampBrightness(int brightness, QString effect, int duration);

        bool StartLampColourFlow(int count, int action, QString flow);
        bool StopLampColourFlow();

        bool SetLampMusic(int action, QString host, int port);

        bool SetLampScene(QString scane, int val1, int val2);
        bool SetLampScene(QString scane, int val1, int val2, int val3);

        bool ToggleLamp();
        bool SetLampDefaultSetting();

        bool AddLampCronJob(int type, int value);
        bool GetLampCronJob(int type);
        bool DelLampCronJob(int type);

        bool SetLampAdjust(QString action, QString prop);

        bool SetLampPower(QString power, QString effect, int duration);

        
};

#endif // LAMCONTROLLER_H