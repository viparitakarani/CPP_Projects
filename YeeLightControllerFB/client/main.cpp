
#include "header/fbdatabasesse.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	
	if((argc >= 4)&&(argc <= 7))
    {

    	if((QString(argv[1])!=SENDEVENT)&&(QString(argv[1])!=LISTENING) )
    	{
    		qInfo() << "USAGE: ./ClientSSE [operator:sendevent/listening] [ipaddress/ipaddress] [port/port] [commandid/lampipaddress] [email] [password]]";
	        app.exit();
			return 0;
    	}
    	else
    	{
    		FBDataBaseSSE *rest = new FBDataBaseSSE(QString(argv[1]),QString(argv[2]),QString(argv[3]),QString(argv[5]),QString(argv[6]),false);
	        if(QString(argv[1])==SENDEVENT)
	        {
	        	rest->Login();
	        	rest->SendCommand(atoi(argv[4]));
				app.exit();
				return 0;
	        }
	        else
	        {
		        if(QString(argv[1])==LISTENING)
		        {
		        	rest->setYeeLampIP(argv[4]);
		        	rest->Login();
		    		//rest->ListeningCmdData();
		        }
		    }
    	}
    	
	       
    }
    else
    {
    	qInfo() << "USAGE: ./ClientSSE [operator:sendevent/listening] [ipaddress/ipaddress] [port/port] [commandid/lampipaddress] [email] [password]]";
        app.exit();
		return 0;
    }
	
	//untuk hold applicaton running
	return app.exec();
}