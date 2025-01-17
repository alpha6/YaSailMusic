#include "downloader.h"

#include <QNetworkRequest>

Downloader::Downloader(QObject* parent)
    : QObject(parent)
{
    m_manager = new QNetworkAccessManager();
}

void Downloader::setUrl(QString urlString)
{
    QUrl url(urlString);
    if (url.isValid() && m_url != url) {
        m_url = url;
    } else {
        emit urlNotValid();
    }
}

void Downloader::setUrl(QUrl url)
{
    if (url.isValid() && m_url != url) {
        m_url = url;
    } else {
        emit urlNotValid();
    }
}

void Downloader::abort()
{
    m_response->abort();
}

void Downloader::loadData()
{
    if (m_url.isEmpty()) {
        return;
    }

    QNetworkRequest request(m_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", "Glacier music player");
    m_response = m_manager->get(request);

    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(dataReady(QNetworkReply*)));
    connect(m_response, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
}

void Downloader::dataReady(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
    }

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 307 || reply->rawHeaderList().contains("Location")) {
        QNetworkRequest req(reply->header(QNetworkRequest::LocationHeader).toString());
        m_manager->get(req);
        return;
    }

    emit stringReady(reply->readAll());
}

void Downloader::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        float progress;
        progress = (float)bytesRead / (float)bytesTotal;
        emit downloadProgress(progress);
    }
}
