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
    QList<int> dayNums;
    QList<QString> dayDescriptions;
    QList<int> dayWind;
    QList<float> dayPercipitation;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void parseTitles(QString str);
    void parseTemperature(QString str);
    void parseDescriptions(QString str);
    void parseWind(QString str);
    void parsePercipitation(QString str);
    void printJson(QString str);

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    void drawDateTitle(QPainter* p, int xoffset, int yoffset);
    void drawDateTitleUnit(QPainter *p, int x, int y, int num);
    void drawTemperatures(QPainter* p, int xoffset, int yoffset);
    void drawTempUnit(QPainter *p, int x, int y, int num);
    void drawWind(QPainter* p, int xoffset, int yoffset);
    void drawWindUnit(QPainter *p, int x, int y, int num);
    void drawPercipitation(QPainter* p, int xoffset, int yoffset);
    void drawPercipitationUnit(QPainter *p, int x, int y, int num);

signals:

public slots:
    void finished(QNetworkReply* r);
    void sslErrors(QNetworkReply* r, QList<QSslError> errors);

};

#endif // MAINWINDOW_H
