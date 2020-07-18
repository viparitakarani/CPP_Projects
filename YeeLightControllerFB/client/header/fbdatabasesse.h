#ifndef FBDATABASESSE_H
#define FBDATABASESSE_H

#include <iostream>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSslConfiguration>
#include <QCoreApplication>
#include <QNetworkAccessManager>


#include "lampcontroller.h"
#include "replytimeout.h"



#define ACCEPT_HEADER_STREAM "text/event-stream"
#define ACCEPT_HEADER_PLAIN "text/plain"
#define ACCEPT_HEADER_JSON "application/json"
#define USERAGENT "CLIENT SSE USER"
#define LISTENING "listening"
#define SENDEVENT "sendevent"
#define YEELAMPPORT "55443"
#define MAXRETRY 100

#define SETRGBCOLOUR "setRGBColour"
#define SETTEMPERATURE "setTemperature"
#define SETBRIGHTNESS "setBrightness"
#define SETLAMPPOWER "setLampPower"
#define STARTFLOW "startLampFlow"
#define STOPFLOW "stopLampFlow"



#define DBURL "https://[project name].firebaseio.com"


namespace YeeLightCore {
    class LampController;
    class FBDataBaseSSE;
 }




typedef struct URLType {
    QString postcmd;
    QString listencmd;
    QString login;
    QString sendevent;
    QString requeststreams;

} URLType;

static const URLType URL = 
{ 
	"/data/commands.json", 
	"/data/commands.json", 
	"/app/login",
	"/app/sendevent",
	"/app/requeststreams"
};


static const URLType URLLocal = { 
	"/data/commands.json", 
	"/data/commands.json", 
	"/login",
	"/sendevent",
	"/requeststreams"
};


class FBDataBaseSSE : public QObject
{

	Q_OBJECT

	//PROPERTIES
	private: 
		int id;
		bool isResponseCompleted;
		bool isLocal;
		const char* TAG ="[YeeLightController]:[FBDataBaseSSE]";

		qint16 retries;

		QString response;
		QString	token;
		QString email;
		QString pass;

		QString action;
		QString host;
		QString port;
		QString yeelampip;


		QNetworkReply *reply;
		QNetworkAccessManager *netManager;


	//METHOD   
	private slots:
		void streamConnFinished(QNetworkReply *reply);
		void onResponseReceived();
		void onResponseFinished();

	private:
		QString setLampBrightness(int brightness, QString effect, int duration);
		QString formatRGBColour(int r,int g,int b,QString effect,int duration);
		QString formatTemperature(int temp,QString effect, int duration);
		QString setLampPower(QString power, QString effect,int duration);
		QString startLampColourFlow(int count, int action, QString flow);
		QString stopLampColourFlow();





		QString jsonObjToString(QJsonObject json);

		void waitResponse();
		void streamEventHandler();
		void streamDBHandler();
		void _setDebugMsg(QString msg,QString method);



	public:
	    FBDataBaseSSE(const QString action,const QString host,const QString port,const QString email,const QString pass,const bool isLocal);
	    void Login();
		void SendEvent(int cmdid);
		void SendCommand(int cmdid);
		void ListeningEvent();
		void ListeningCmdData();
		void setYeeLampIP(QString ip);
		void post(const QUrl &url,const QByteArray &body,const QByteArray token, const QByteArray accept);
		void get(const QUrl &url,const QByteArray token, const QByteArray accept,const bool keepalive);

};

#endif // FBDATABASESSE_H