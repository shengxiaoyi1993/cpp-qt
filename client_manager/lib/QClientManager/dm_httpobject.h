#ifndef DM_HTTPOBJECT_H
#define DM_HTTPOBJECT_H

#include<QObject>
#include<QNetworkAccessManager>
#include<QUrl>
#include<QByteArray>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QVariant>
#include<QString>
#include<QUrlQuery>
#include<QDebug>
#include<QImage>
#include<QEventLoop>
#include<QFile>
#include<QTcpServer>
#include<QTcpSocket>
#include<QTimer>
#include<QPixmap>
#include<QImage>
#include<QDir>

class OneConnection: public QObject {
    Q_OBJECT
public:
    OneConnection(QTcpSocket *socket, int(*p_fun)(QString &));
    ~OneConnection();
    void StartParse();
private slots:
    void OnMessageReady();
    void OnTimeout();
    void OnCloseByClient();


private:
    QTcpSocket *_socket = nullptr;
    QTimer *_timer = nullptr;
    int(*_p_fun)(QString &) = nullptr;
    bool _message_ready = false;
    bool _closed_by_client = false;

    void CloseAndDelete();
};

class dm_HTTPObject:public QObject
{
  Q_OBJECT
public:
  QNetworkReply *reply;
  QFile file;
  QDataStream file_stream;
  QTcpServer* tcpserver;
  int flag_state=0;
  int (*p_DealWithData)(QString ,QByteArray &);

  dm_HTTPObject();
  ~dm_HTTPObject();

  //
  static bool downloadURL(const QString &url, const QString &fileName);
  void GetInfoFromHttpPort(int (*p_fun)(QString&));
  void GetInfoFromHttpPort(int (*p_fun)(QString ,QByteArray&));
  // timeout : timeout / seconds.
  static int Get(QString httpurl, QByteArray &response, int timeout = 10);
  void StopListen();



  /**
  *   GET request to http and save the info. to a QString.
  *
  *  @param   httpurl        complete request to get get info
  *  @param   response       Result save to it.
  *  @return                 return 0 if normal
  *
  *  @example
  *  ConnectToHTTP("http://root:admin@192.168.1.178/cgi-bin/UVSSapi.cgi?"\
  *          {\"cmd\":\"get\",\"key\":\"device id\"}",str_response);
  */
  static int ConnectToHTTP(QString httpurl,QString &response);



  /**
  *   GET request to http and save the ImageBuffer to certain file.
  *
  *  @param   httpurl        complete request to get ImageBuffer
  *  @param   image_path     File path and name to save ImageBuffer
  *  @return                 return 0 if normal
  *
  *  @example
  *  ConnectToHTTP("http://root:admin@192.168.1.178/cgi-bin/UVSSapi.cgi?"\
  *          {\"cmd\":\"get\",\"key\":\"device id\"}",str_response);
  */
  static int HttpApiToDownloadImage(QString httpurl,QString image_path);



  /**
  *  Listen To Http Signal From Port.
  *
  *  @param   port       the port emit signal
  *  @param   p_fun    excute callback function when receive data.
  *  @return                 return 0 if successful
  *
  *  @example
  *  int operate_flag=dm::BuildPortToWaitInfoFromUVSSCamera(60800,dm::CallBackFunc)
  */
  int ListenToHttpSignalFromPort(int port,int (*p_fun)(QString &));


  /**
  *  Listen To Http Signal From Port for Client.
  *
  *  @param   port       the port emit signal
  *  @param   p_fun    excute callback function when receive data.
  *  @return                 return 0 if successful
  *
  *  @example
  *  int operate_flag=dm::ListenToHttpSignalFromPortForClient(60800,dm::CallBackFunc)
  */
  int ListenToHttpSignalFromPortForClient(int port,int (*p_fun)(QString &));


  /**
  *  Listen To Http Signal From Port for client.
  *
  *  @param   port       the port emit signal
  *  @param   p_fun       excute callback function when receive data.
  *  @param   p_ProveAuthen       to prove the authen.
  *  @return                 return 0 if successful
  *
  *  @example
  *  int operate_flag=dm::BuildPortToWaitInfoFromUVSSCamera(60800,dm::CallBackFunc)
  */
  int ListenToHttpSignalFromPort(int port, int (*p_treatdata)(QString ,QByteArray&));



public slots:
  void ReplyFinished(QNetworkReply *reply);
  void ReadyToReadReply();
  void TestSlot();
};

#endif // DM_HTTPOBJECT_H
