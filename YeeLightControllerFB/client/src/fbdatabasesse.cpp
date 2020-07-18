
#include "../header/fbdatabasesse.h"


FBDataBaseSSE::FBDataBaseSSE(const QString action,const QString host,const QString port,const QString email,const QString pass,const bool isLocal=false){

	this->netManager = new QNetworkAccessManager(this);
	connect(this->netManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(streamConnFinished(QNetworkReply *)));


	this->action=action;
    this->host=host;
    if (port !="")
        this->port=":"+port;
    this->host=this->host+this->port;
    this->isLocal=isLocal;

	this->isResponseCompleted=true;
	this->retries = 0;

    this->email=email;
    this->pass=pass;

    this->_setDebugMsg(this->host,"[FBDataBaseSSE]");

}

void FBDataBaseSSE::_setDebugMsg(QString msg,QString method)
{
    QDateTime now = QDateTime::currentDateTime();
    qDebug() << QString("%1:[%2]:%3:%4").arg(TAG).arg(now.toString()).arg(method).arg(msg); 
}

void FBDataBaseSSE::post(const QUrl &url,const QByteArray &body,const QByteArray token=nullptr,const QByteArray accept=QByteArray(ACCEPT_HEADER_JSON))
{

   	this->isResponseCompleted=false;

    QNetworkRequest request(url);

   	request.setHeader(QNetworkRequest::ContentTypeHeader, accept);
    if(token!=nullptr)
    	request.setRawHeader(QByteArray("Authorization"), token);

    if(accept!=nullptr)
    	request.setRawHeader(QByteArray("Accept"), accept);

    request.setHeader(QNetworkRequest::UserAgentHeader, QByteArray(USERAGENT));
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork); 
    /*
    if (AppSettings::isUsingHttps()) {
        request.setUrl(QUrl("https://httpbin.org/post"));

        QSslConfiguration config = request.sslConfiguration();
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1);
        request.setSslConfiguration(config);
    }*/
    
   
    this->reply = this->netManager->post(request, body);
    ReplyTimeout::set(this->reply, 10000);

   

    connect(this->reply, SIGNAL(readyRead()), this, SLOT(onResponseReceived()));
    //connect(this->reply, SIGNAL(finished()), this, SLOT(onResponseFinished()));

}


void FBDataBaseSSE::get(const QUrl &url,const QByteArray token=nullptr,const QByteArray accept=QByteArray(ACCEPT_HEADER_PLAIN),const bool keepalive=false)
{

   	this->isResponseCompleted=false;
    QNetworkRequest request(url);

   	request.setHeader(QNetworkRequest::ContentTypeHeader, accept);
    if(token!=nullptr)
    	request.setRawHeader(QByteArray("Authorization"), token);

   
    if(accept!=nullptr)
    	request.setRawHeader(QByteArray("Accept"), accept);

    if(keepalive)
    {
    	request.setRawHeader(QByteArray("Connection"), QByteArray("Keep-Alive"));
    	//request.setRawHeader(QByteArray("Keep-Alive"), QByteArray("timeout=10, max=1000"));
    }
    else
    {
        ReplyTimeout::set(this->reply, 10000);

    }


    request.setHeader(QNetworkRequest::UserAgentHeader, QByteArray(USERAGENT));
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork); 
    /*
    if (AppSettings::isUsingHttps()) {
        request.setUrl(QUrl("https://httpbin.org/post"));

        QSslConfiguration config = request.sslConfiguration();
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1);
        request.setSslConfiguration(config);
    }*/
    
    this->reply = this->netManager->get(request);

    connect(this->reply, SIGNAL(readyRead()), this, SLOT(onResponseReceived()));
    //connect(this->reply, SIGNAL(finished()), this, SLOT(onResponseFinished()));

}

void FBDataBaseSSE::setYeeLampIP(QString ip)
{
    this->yeelampip=ip;
}


void FBDataBaseSSE::onResponseReceived()
{
	this->response=nullptr;
	
    if (reply) 
    {
	    if (this->reply->error() == QNetworkReply::NoError) {

            const int available = this->reply->bytesAvailable();
            if (available > 0) 
            {
                this->response = QString(this->reply->readAll()).simplified().replace("event: put data: ", "");
              
                //qDebug()  <<  this->response;

                QString urlPath = reply->url().path();
                if(((urlPath==URLLocal.requeststreams)||(urlPath==URL.requeststreams))&&(this->action==LISTENING))
            		this->streamEventHandler();
                if(((urlPath==URLLocal.listencmd)||(urlPath==URL.listencmd))&&(this->action==LISTENING))
                    this->streamDBHandler();
            }
            this->retries = 0;
        } 
        else {

            this->response = tr("Error: %1 status: %2").arg(this->reply->errorString(), this->reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
            this->_setDebugMsg(this->response,"[onResponseReceived]");
 
        }
        //this->reply->deleteLater();
    }

    if (response.trimmed().isEmpty()) {
        this->response = tr("Unable to retrieve post response");
    }

    this->isResponseCompleted=true;
    
}



void FBDataBaseSSE::streamDBHandler()
{

   
    QJsonDocument jsonResponse = QJsonDocument::fromJson(this->response.toUtf8());

    //QString json_string = jsonResponse.toJson(QJsonDocument::Indented);
    //qDebug() << "COMPLETE JESON" << json_string;

    if(!jsonResponse.isEmpty())
    {
        QString json_string = jsonResponse.toJson(QJsonDocument::Compact);
       
        QJsonObject jsonObject = jsonResponse.object();

        QString    path  =  jsonObject["path"].toString();
        QJsonObject data =  jsonObject["data"].toObject();
        QString cmdstr =  data["cmd"].toString();
        QJsonObject params = data["params"].toObject();


        LampController lampController(this->yeelampip, YEELAMPPORT); 
        bool isSuccess=false;

        if(cmdstr==QString(SETRGBCOLOUR))
        {
            int r = params["r"].toInt();
            int g = params["g"].toInt();
            int b = params["b"].toInt();
            QString effect = params["effect"].toString();
            int duration = params["duration"].toInt();

            isSuccess = lampController.SetLampRGBColour(r,g,b,effect,duration);
          
            if(isSuccess)
            {
                 this->_setDebugMsg("Yeaa!! It successfully changing colour..!!","[streamDBHandler]");
            }
            else
            {
                this->_setDebugMsg("Fail! Please again!","[streamDBHandler]");
            }

            return;
        }

        if(cmdstr==QString(SETTEMPERATURE))
        {
            int temp = params["temp"].toInt();
            QString effect = params["effect"].toString();
            int duration = params["duration"].toInt();

            if(temp<=1)
                temp=1;
            if (temp>=100)
                temp=99;

            temp=1700+(temp*48);
            isSuccess = lampController.SetLampColourTemperature(temp,effect,duration);


            //printf("%s : %d\n", TAG,k);

            if(isSuccess){

                this->_setDebugMsg("Temperature set to "+QString::number(temp),"[streamDBHandler]");
            
            }
            else{
                
                this->_setDebugMsg("Fail! Please  try again!","[streamDBHandler]");
            
            }
           
            return;
        }

        if(cmdstr==QString(SETBRIGHTNESS))
        {
            int brightness = params["brightness"].toInt();
            QString effect = params["effect"].toString();
            int duration = params["duration"].toInt();

            if(brightness<=1)
                brightness=1;
            if (brightness>=100)
                brightness=99;

            //temp=1700+(temp*48);
            isSuccess = lampController.SetLampBrightness(brightness,effect,duration);


            //printf("%s : %d\n", TAG,k);

            if(isSuccess){

                this->_setDebugMsg("Brightness set to "+QString::number(brightness),"[streamDBHandler]");
            
            }
            else{
                
                this->_setDebugMsg("Fail! Please try again!","[streamDBHandler]");
            
            }
           
            return;
        }

        if(cmdstr==QString(SETLAMPPOWER))
        {
            QString power = params["power"].toString();
            QString effect = params["effect"].toString();
            int duration = params["duration"].toInt();

            
            isSuccess = lampController.SetLampPower(power,effect,duration);


            if(isSuccess){

                this->_setDebugMsg("Power is switched "+power,"[streamDBHandler]");
            
            }
            else{
                
                this->_setDebugMsg("Fail! Please try again!","[streamDBHandler]");
            
            }
           
            return;
        }

        if(cmdstr==QString(STARTFLOW))
        {
            int count = params["count"].toInt();
            int action = params["action"].toInt();
            QString flow = params["flow"].toString();


            
            isSuccess = lampController.StartLampColourFlow(count,action,flow);


            if(isSuccess){

                this->_setDebugMsg("Flow is started","[streamDBHandler]");
            
            }
            else{
                
                this->_setDebugMsg("Fail! Please try again!","[streamDBHandler]");
            
            }
           
            return;
        }


        if(cmdstr==QString(STOPFLOW))
        {
            QString flow = params["flow"].toString();


            isSuccess = lampController.StopLampColourFlow();


            if(isSuccess){

                this->_setDebugMsg("Flow is stoped","[streamDBHandler]");
            
            }
            else{
                
                this->_setDebugMsg("Fail! Please try again!","[streamDBHandler]");
            
            }
           
            return;
        }

    }
    else{
        this->_setDebugMsg("NO UPDATE FOUND","[streamDBHandler]");
    }
}




void FBDataBaseSSE::waitResponse()
{
	while(this->isResponseCompleted==false)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100 );
	}
}

QString FBDataBaseSSE::formatRGBColour(int r=200,int g=17,int b=79,QString effect="smooth",int duration=1500)
{
    QString cmd= QString("{\"id\":" +QString::number(QDateTime::currentMSecsSinceEpoch())+ ",\"user\":\"" +this->email+ "\",\"cmd\":\"" +QString(SETRGBCOLOUR)+ "\",\"params\":{\"r\":"+QString::number(r)+",\"g\":"+QString::number(g)+",\"b\":"+QString::number(b)+",\"effect\":\""+effect+"\",\"duration\":"+QString::number(duration)+"}}\n\n");
    return cmd;
}

QString FBDataBaseSSE::formatTemperature(int temp=80,QString effect="smooth",int duration=1500)
{
    QString cmd= QString("{\"id\":" +QString::number(QDateTime::currentMSecsSinceEpoch())+ ",\"user\":\"" +this->email+ "\",\"cmd\":\"" +QString(SETTEMPERATURE)+ "\",\"params\":{\"temp\":"+QString::number(temp)+",\"effect\":\""+effect+"\",\"duration\":"+QString::number(duration)+"}}\n\n");
    return cmd;
}

QString FBDataBaseSSE::setLampBrightness(int brightness=80, QString effect="smooth",int duration=1500)
{
    QString cmd= QString("{\"id\":" +QString::number(QDateTime::currentMSecsSinceEpoch())+ ",\"user\":\"" +this->email+ "\",\"cmd\":\"" +QString(SETBRIGHTNESS)+ "\",\"params\":{\"brightness\":"+QString::number(brightness)+",\"effect\":\""+effect+"\",\"duration\":"+QString::number(duration)+"}}\n\n");
    return cmd;
}

QString FBDataBaseSSE::setLampPower(QString power="off", QString effect="smooth",int duration=1500)
{
    QString cmd= QString("{\"id\":" +QString::number(QDateTime::currentMSecsSinceEpoch())+ ",\"user\":\"" +this->email+ "\",\"cmd\":\"" +QString(SETLAMPPOWER)+ "\",\"params\":{\"power\":\""+power+"\",\"effect\":\""+effect+"\",\"duration\":"+QString::number(duration)+"}}\n\n");
    return cmd;
}

QString FBDataBaseSSE::startLampColourFlow(int count=0, int action=0, QString flow=QString("100, 2, 2700, 100, 100, 1, 255, 10, 100, 7, 0,0, 100, 2, 5000, 1"))
{
    QString cmd= QString("{\"id\":" +QString::number(QDateTime::currentMSecsSinceEpoch())+ ",\"user\":\"" +this->email+ "\",\"cmd\":\"" +QString(STARTFLOW)+ "\",\"params\":{\"count\":\""+QString::number(count)+"\",\"action\":"+QString::number(action)+",\"flow\":\""+flow+"\"}}\n\n");
    return cmd;
}

QString FBDataBaseSSE::stopLampColourFlow()
{
    QString cmd= QString("{\"id\":" +QString::number(QDateTime::currentMSecsSinceEpoch())+ ",\"user\":\"" +this->email+ "\",\"cmd\":\"" +QString(STOPFLOW)+ "\",\"params\":{\"flow\":\"stop\"} }\n\n");
    return cmd;
}



QString FBDataBaseSSE::jsonObjToString(QJsonObject json)
{
    QJsonDocument Doc(json);
    QByteArray ba = Doc.toJson();
    return QString(ba);
}




void FBDataBaseSSE::Login()
{
    QString url=URL.login;

    if (this->isLocal){
        url = URLLocal.login;
    }

	this->_setDebugMsg("Login...","[Login]");
    QString data= QString("{\"email\":\""+this->email+"\",\"pass\":\""+pass+"\"}\n\n");
	this->token=nullptr;
	this->post(QUrl(this->host+url),data.toUtf8(),QByteArray(ACCEPT_HEADER_JSON));
	this->waitResponse();
	this->token=this->response;
}




void FBDataBaseSSE::SendCommand(int cmdid)
{
   
    this->_setDebugMsg("Sending DB Command...","[SendCommand]");

    QString data;
    switch(cmdid)
    {
        case 0: data=this->formatRGBColour(); break;
        case 1: data=this->formatTemperature(); break;
        case 2: data=this->setLampBrightness();break;
        case 3: data=this->setLampPower();break;
        case 4: data=this->startLampColourFlow();break;
        case 5: data=this->stopLampColourFlow();break;
    }

    QString url=URL.postcmd;
    if (this->isLocal)
        url = URLLocal.postcmd;

    qDebug() << data;
    QUrl qurl(QString(DBURL)+url);
    QUrlQuery query;
    query.addQueryItem("auth",this->token);
    qurl.setQuery(query);

    this->post( qurl , data.toUtf8() ,"", QByteArray(ACCEPT_HEADER_JSON) );
    this->waitResponse();
}

void FBDataBaseSSE::ListeningCmdData()
{

    QString url=URL.listencmd;
    if (this->isLocal)
        url = URLLocal.listencmd;

    this->_setDebugMsg("Listenning Database...","[ListeningCmdData]");


    QUrl qurl(QString(DBURL)+url);
    QUrlQuery query;
    query.addQueryItem("auth",this->token);
    qurl.setQuery(query);

    this->get(qurl,"",QByteArray(ACCEPT_HEADER_STREAM),true);
    
}





/***
* DEPRECATED METHODS
* NO LONGER USE SINCE THE SSE DIRECT TO FIREBASE DB
*
*/

void FBDataBaseSSE::ListeningEvent()
{
    QString url=URL.requeststreams;
    if (this->isLocal){
        url = URLLocal.requeststreams;
    }

	this->_setDebugMsg("Listenning Event...","[ListeningEvent]");

	this->get(QUrl(this->host+url),QString(" Bearer "+this->token).toUtf8(),QByteArray(ACCEPT_HEADER_STREAM),true);
	
}


//Try to connect when connestion lost
void FBDataBaseSSE::streamConnFinished(QNetworkReply *reply)
{
	//qDebug() << "Connection finished:" << reply->url().path();
    QString rplUrl=reply->url().path();
    this->_setDebugMsg(rplUrl,"[streamConnFinished]");

	if(((rplUrl==URLLocal.requeststreams)||(rplUrl==URL.requeststreams)||(rplUrl==URLLocal.listencmd)||(rplUrl==URL.listencmd))&&(this->action==LISTENING))
	{
		QDateTime now = QDateTime::currentDateTime();

    	this->_setDebugMsg("[" + now.toString() + "]: Listener Reconnecting...","[streamConnFinished]");

    	if(this->retries < MAXRETRY) 
    	{
        	this->retries++;
        	this->Login();
        	QThread::sleep(1);
        	
    	}
   	 	else 
   	 	{
            this->_setDebugMsg("Unable to reconnect, max retries reached","[streamConnFinished]");

    	}
    }

    if((rplUrl==URLLocal.login)||(rplUrl==URL.login)||(rplUrl=="/ServiceLogin"))
	{
		this->token=this->response;

        this->_setDebugMsg("Login is Commpleted...","[streamConnFinished]");


        if(this->action==LISTENING)
		{
			if(this->retries < MAXRETRY) 
	    	{
	        	this->retries++;
	        	//this->ListeningEvent();
                this->ListeningCmdData();
	        	QThread::sleep(1);	        	
	    	}
	   	 	else 
	   	 	{
                this->_setDebugMsg("Unable to reconnect, max retries reached","[streamConnFinished]");

	    	}
		}
		

	}

    if(((rplUrl==URLLocal.postcmd)||(rplUrl==URL.postcmd))&&(this->action==SENDEVENT))
    {
        this->_setDebugMsg(this->response,"[streamConnFinished]");
        this->_setDebugMsg("Send Commands Commpleted...","[streamConnFinished]");

    }

	if(((rplUrl==URLLocal.sendevent)||(rplUrl==URL.sendevent))&&(this->action==SENDEVENT))
	{
        this->_setDebugMsg(this->response,"[streamConnFinished]");
        this->_setDebugMsg("Send Event Commpleted...","[streamConnFinished]");
	}

    
}



//NOT USED FOR NOW
void FBDataBaseSSE::onResponseFinished()
{
	this->_setDebugMsg(this->reply->url().path(),"[onResponseFinished]");
	this->isResponseCompleted=true;
	this->retries = 0;
}

void FBDataBaseSSE::streamEventHandler()
{
    this->_setDebugMsg("Response :" + this->response,"[streamEventHandler]");

    LampController lampController(this->yeelampip, YEELAMPPORT); 
    bool isSuccess=false;

    QJsonDocument jsonResponse = QJsonDocument::fromJson(this->response.toUtf8());

    QJsonObject jsonObject = jsonResponse.object();

    //QJsonObject cmd = jsonObject.toObject();
    QString cmdstr =  jsonObject["cmd"].toString();
    //qDebug() << cmdstr;

    QJsonObject params = jsonObject["params"].toObject();
    if(cmdstr==QString(SETRGBCOLOUR))
    {
        int r = params["r"].toInt();
        int g = params["g"].toInt();
        int b = params["b"].toInt();
        QString effect = params["effect"].toString();
        int duration = params["duration"].toInt();

        isSuccess = lampController.SetLampRGBColour(r,g,b,effect,duration);

        if(isSuccess)
        {
            this->_setDebugMsg("Yeaa!! It successfully changing colour..!!","[streamEventHandler]");

        }
        else
        {
            this->_setDebugMsg("Fail! Click to try again!","[streamEventHandler]");
        }

        return;
    }

    if(cmdstr==QString(SETTEMPERATURE))
    {
        int temp = params["temp"].toInt();
        QString effect = params["effect"].toString();
        int duration = params["duration"].toInt();

        if(temp<=1)
            temp=1;
        if (temp>=100)
            temp=99;

        temp=1700+(temp*48);
        isSuccess = lampController.SetLampColourTemperature(temp,effect,duration);


        //printf("%s : %d\n", TAG,k);

        if(isSuccess)
        {
            

            this->_setDebugMsg("Temperature set to "+QString::number(temp),"[streamEventHandler]");

        }
        else
        {
            this->_setDebugMsg("Fail! Slide to try again!","[streamEventHandler]");
        }
       
        return;
    }

    this->_setDebugMsg("Invalid Yee Lamp Command !","[streamEventHandler]");


}

void FBDataBaseSSE::SendEvent(int cmdid)
{
    this->_setDebugMsg("Sending Event...","[SendEvent]");

    QString data;
    switch(cmdid)
    {
        case 0: data=this->formatRGBColour(); break;
        case 1: data=this->formatTemperature(); break;
    }

    QString url=URL.sendevent;
    if (this->isLocal)
        url = URLLocal.sendevent;

    this->post(QUrl(this->host+url),data.toUtf8(),QString(" Bearer "+this->token).toUtf8(),QByteArray(ACCEPT_HEADER_JSON));
    this->waitResponse();
}




/*
RestAPI *RestAPI::instance = nullptr;
RestAPI *RestAPI::getInstance()
{
    if (instance == nullptr) {
        //qDebug() << "Creating new Network::NetManager";
        instance = new RestAPI();
    }
    return instance;
}*/

