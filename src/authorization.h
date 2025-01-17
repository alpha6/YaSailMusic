#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Authorization : public QObject {
    Q_OBJECT

public:
    explicit Authorization(QObject* parent = 0);
    ~Authorization();

    static void setupRequest(QNetworkRequest* r);
    Q_INVOKABLE void doAuth(QString username, QString password);
    Q_INVOKABLE void storeToken(QString url);
    Q_INVOKABLE bool checkToken();
    Q_INVOKABLE void removeAccessToken();

public slots:

private slots:
    void doAuthFinished();

signals:
    void error(QString errorMessage);
    void authorized(QString accessToken);

private:
    const QString m_oauthURL = "https://oauth.yandex.ru/authorize";
    const QString m_clientID = "23cabbbdc6cd418abb4b39c32c41195d";
    const QString m_clientSecret = "53bc75238f0c4d08a118e51fe9203300";

    QString m_token;
    QDateTime m_ttl;
};

#endif // AUTHORIZATION_H
