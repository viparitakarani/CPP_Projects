#ifndef REPLAYTIMEOUT_H
#define REPLAYTIMEOUT_H

#include <QBasicTimer>
#include <QNetworkReply>
#include <QTimerEvent>

class ReplyTimeout : public QObject {
  Q_OBJECT

private: 
  QBasicTimer m_timer;

public:
  ReplyTimeout(QNetworkReply* reply, const int timeout);
  static void set(QNetworkReply* reply, const int timeout);

protected:
  void timerEvent(QTimerEvent * ev);

};

#endif //REPLAYTIMEOUT_H
