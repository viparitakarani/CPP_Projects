#include "../header/lampcontroller.h"



LampController::LampController(QString ipAdd, QString portAdd)
{
    ip = ipAdd;
    port = portAdd;
}

void LampController::_setDebugMsg(QString msg)
{
    this->_setDebugMsg(msg,"[LampController]");
}

void LampController::_setDebugMsg(QString msg,QString method)
{

    qDebug() << QString("%1:%2:%3").arg(TAG).arg(method).arg(msg); 
}

bool LampController::_SendCommand(QString cmdJson,QString conf,QString method)
{
    return this->_SendCommand(cmdJson,conf,QString("set"),method);
}

bool LampController::_SendCommand(QString cmdJson,QString conf,QString action,QString method)
{
    QByteArray byteCmd;
    byteCmd.append(cmdJson);

    this->_setDebugMsg("Connecting...",method);
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(ip, port.toUShort());
   

    if(socket->waitForConnected(3000)) 
    {
        this->_setDebugMsg("The connection was successful.",method);

        socket->write(byteCmd); 

        if(socket->waitForBytesWritten(3000)) 
        {
            this->_setDebugMsg(QString("%1 was %2 successfully.").arg(conf).arg(action),method);
        }
        else 
        {
            this->_setDebugMsg(QString("Fail!! %1 was NOT %2.").arg(conf).arg(action),method);
            return false;
        }

        socket->close(); 

        return true;
    }

    else
    {
        this->_setDebugMsg("The connection was NOT successful",method); 
        return false;
    }
}

bool LampController::GetLampConnection() 
{
    QString method = "[GetLampConnection]";

    //Byte Command which will be sent to the Lamp from JSON Convertion
    QByteArray cmdByte1;
    QByteArray cmdByte2;

    //The JSON Cmmand which will be sent to the lamp
    QString cmdJson1 = "{ \"id\": 1, \"method\": \"set_power\", \"params\":[\"off\", \"smooth\", 1500]}\r\n\r\n\r\n";
    QString cmdJson2 = "{ \"id\": 1, \"method\": \"set_power\", \"params\":[\"on\", \"smooth\", 1500]}\r\n\r\n\r\n";
    

    cmdByte1.append(cmdJson1);
    cmdByte2.append(cmdJson2);


    this->_setDebugMsg("Connecting...",method); 
    //Create a connetion Object to the lamp
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost(ip, port.toUShort());


    //If Connected
    if(socket->waitForConnected(3000)) 
    {
        this->_setDebugMsg("The Connection was successful",method); 

        //Send the off command to socket
        socket->write(cmdByte1); 

        if(socket->waitForBytesWritten(3000)) 
        {
            this->_setDebugMsg("The off command was sent",method); 


        }
        // The off command is not sent
        else 
        {
            this->_setDebugMsg("The off command was NOT sent",method); 
            return false;
        }

        socket->write(cmdByte2);

        if(socket->waitForBytesWritten(3000)) 
        {
            this->_setDebugMsg("The on command was sent",method); 
        }
        // The on command was not sent
        else 
        {
            this->_setDebugMsg("The on command was NOT sent",method); 
            return false;
        }

        // Close Connection
        socket->close(); 

        return true;
    }

    else
    {
        this->_setDebugMsg("The connection was NOT successful",method); 
        return false;
    }
}

bool LampController::SetLampRGBColour(int R, int G, int B, QString effect, int duration) 
{
    QString method = "[SetLampRGBColour]";

    int RGB = (R * 65536) + (G * 256) + B;

    QString cmdJson = "{\"id\":1,\"method\":\"set_rgb\",\"params\":[" + QString::number(RGB) + ",\"" + effect + "\"," + QString::number(duration) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("RGB Colour"),method);
}

bool LampController::SetLampColourTemperature(int temperature, QString effect, int duration) 
{
    QString method = "[SetLampColourTemperature]";
    
    QString cmdJson = "{\"id\":1,\"method\":\"set_ct_abx\",\"params\":[" + QString::number(temperature) + ",\"" + effect + "\"," + QString::number(duration) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("ColourTemperature"),method);
}

bool LampController::SetLampBrightness(int brightness, QString effect, int duration) 
{
    QString method = "[SetLampBrightness]";

    QString cmdJson = "{\"id\":1,\"method\":\"set_bright\",\"params\":[" + QString::number(brightness) + ",\"" + effect + "\"," + QString::number(duration) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Brightness"),method);
}

bool LampController::SetLampHSV(int hue, int sat, QString effect, int duration) 
{
    QString method = "[SetLampHSV]";
    
    QString cmdJson = "{\"id\":1,\"method\":\"set_hsv\",\"params\":[" + QString::number(hue) + "," + QString::number(sat) + ",\"" + effect + "\"," + QString::number(duration) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("HSV"),method);
}

bool LampController::StartLampColourFlow(int count, int action, QString flow) 
{
    QString method = "[StartLampColourFlow]";

    QString cmdJson = "{\"id\":1,\"method\":\"start_cf\",\"params\":[" + QString::number(count) + "," + QString::number(action) + ",\"" + flow + "\"]}\r\n";
   
    return this->_SendCommand(cmdJson,QString("Colour Flow"),QString("start"),method);
}

bool LampController::StopLampColourFlow() 
{
    QString method = "[StopLampColourFlow]";

    QString cmdJson = "{ \"id\": 1, \"method\": \"stop_cf\", \"params\":[]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Colour Flow"),QString("stop"),method);
}

bool LampController::SetLampMusic(int action, QString host, int port)
{
    QString method = "[SetLampMusic]";

    QString cmdJson = "{\"id\":1,\"method\":\"set_music\",\"params\":[" + QString::number(action) + ",\"" + host + "\"," + QString::number(port) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Music"),method);
}

bool LampController::SetLampScene(QString scene, int val1, int val2) 
{
    QString method = "[SetLampScene]";

    QString cmdJson = "{\"id\":1,\"method\":\"set_scene\",\"params\":[\"" + scene + "\"," + QString::number(val1) + "," + QString::number(val2) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Scene"),method);
}

bool LampController::SetLampScene(QString scene, int val1, int val2, int val3) 
{    
    QString method = "[SetLampScene]";

    QString cmdJson = "{\"id\":1,\"method\":\"set_scene\",\"params\":[\"" + scene + "\"," + QString::number(val1) + "," + QString::number(val2) + "," + QString::number(val3) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Scene"),method);
}

bool LampController::ToggleLamp() 
{
    QString method = "[ToggleLamp]";
    
    QString cmdJson = "{ \"id\": 1, \"method\": \"toggle\", \"params\":[]}\r\n";

    return this->_SendCommand(cmdJson,QString("Lamp"),QString("toggled"),method);
}




bool LampController::AddLampCronJob(int type, int value) 
{

    QString method = "[AddLampCronJob]";

    QString cmdJson = "{\"id\":1,\"method\":\"cron_add\",\"params\":[" + QString::number(type) + "," + QString::number(value) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Cron Job"),QString("added"),method);
}

bool LampController::GetLampCronJob(int type)
{
    QString method = "[GetLampCronJob]";

    
    QString cmdJson = "{\"id\":1,\"method\":\"cron_get\",\"params\":[" + QString::number(type) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Cron Job"),QString("retreived"),method);
}

bool LampController::DelLampCronJob(int type) 
{
    QString method = "[DelLampCronJob]";

    QString cmdJson = "{\"id\":1,\"method\":\"cron_del\",\"params\":[" + QString::number(type) + "]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Cron Job"),QString("deleted"),method);
}


bool LampController::SetLampAdjust(QString action, QString prop)
{
    QString method = "[SetLampAdjust]";

    if (action=="color")
        prop="circle";
   
    QString cmdJson = "{ \"id\": 1, \"method\": \"set_power\", \"params\":[\"" + action + "\", \"" + prop + "\",]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Lamp Addjustment"),QString("set"),method);

}


bool LampController::SetLampDefaultSetting() 
{
    QString method = "[SetLampDefault]";

    QString cmdJson = "{ \"id\": 1, \"method\": \"set_default\", \"params\":[]}\r\n";
    
    return this->_SendCommand(cmdJson,QString("Default State"),method);
}


bool LampController::SetLampPower(QString power, QString effect, int duration) 
{
    QString method = "[SetLampPower]";
   
    QString cmdJson = "{ \"id\": 1, \"method\": \"set_power\", \"params\":[\"" + power + "\", \"" + effect + "\"," + QString::number(duration) + "]}r\n\r\n";
   
    return this->_SendCommand(cmdJson,QString("Lamp Power"),QString("set %1").arg(power),method);
}


