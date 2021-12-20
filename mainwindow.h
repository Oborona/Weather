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

#include <QPaintEvent>
#include <QPainter>

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QTextEdit* edit;
    QGridLayout* layout;

    int unitsNum;
    int unitSize;

    QList<int> dayTemps;
    QList<QString> dayTitles;
    QList<int> dayWind;
    QList<float> dayPercipitation;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void parseTemperature(QString str);
    void parseTitles(QString str);
    void parseWind(QString str);
    void parsePercipitation(QString str);
    void printJson(QString str);

protected:
    void paintEvent(QPaintEvent* event);
    void drawTemperatures(QPainter* p, int xoffset, int yoffset);
    void drawTempUnit(QPainter *p, int x, int y, int num);

signals:

public slots:
    void finished(QNetworkReply* r);
    void sslErrors(QNetworkReply* r, QList<QSslError> errors);

};

#endif // MAINWINDOW_H
