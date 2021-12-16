#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QTextEdit* edit;
    QGridLayout* layout;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void printJson(QString str);

signals:

public slots:
    void finished(QNetworkReply* r);
    void sslErrors(QNetworkReply* r, QList<QSslError> errors);

};

#endif // MAINWINDOW_H
