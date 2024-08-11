#include "downloader.h"

#include <QString>
#include <QDebug>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

void downloadFile(const QString& url, const QString& fileName) {
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QUrl myUrl(url);
    request.setUrl(myUrl);

    // Send the request and wait for the reply
    QNetworkReply* reply = manager.get(request);

    // Create an event loop to wait for the reply
    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    // Check for errors
    if (reply->error()) {
        qDebug() << "Error downloading file:" << reply->errorString();
    } else {
        // Save the file
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.flush();
            file.close();
            qDebug() << "File saved:" << fileName;
        } else {
            qDebug() << "Error saving file:" << file.errorString();
        }
    }

    // Clean up
    reply->deleteLater();
}


