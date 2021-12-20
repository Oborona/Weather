#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    unitsNum = 14;
    unitSize = 40;

    this->resize(unitsNum*50+20, 800);
    QNetworkAccessManager* nam = new QNetworkAccessManager();
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    connect(nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    QString url = "https://www.gismeteo.ru/weather-moscow-4368/2-weeks/";
    QNetworkRequest request;
    request.setUrl(url);
    nam->get(request);

//    edit = new QTextEdit(this);
//    layout = new QGridLayout();
//    layout->addWidget(edit, 0, 0);
//    this->setLayout(layout);
}

void MainWindow::printJson(QString str)
{
    QString str2 = str.remove("\"");
    str2 = str2.replace(",", "\n");
    str2 = str2.replace("{", "\n{\n");
    str2 = str2.replace("}", "\n}\n");
    QStringList strl = str2.split("\n");
    strl.removeAll("");
    int tabLevel = -1;
    for (int i = 0; i < strl.size(); i++)
    {
        QString single = strl.at(i);
        if (single.contains("{"))
            tabLevel++;

        for (int j = 0; j < tabLevel; j++)
            single.prepend("    ");

        edit->append(single);
        if (single.contains("}"))
            tabLevel--;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    int xoffset = 10;
    int yoffset = 10;
    int x, y;

    QPen pen(Qt::black, 2);
    QPainter painter(this);
    painter.setPen(pen);
    for (int i = 0; i < unitsNum; i++)
        painter.drawRect(xoffset+i*unitSize, yoffset, unitSize, unitSize);

    drawTemperatures(&painter, xoffset, unitSize+2*yoffset);
//    for (int i = 0; i < unitsNum; i++)
//        painter.drawRect(xoffset+i*unitSize, yoffset, unitSize, unitSize);

//    for (int i = 0; i < unitsNum; i++)
//        painter.drawRect(xoffset+i*unitSize, yoffset, unitSize, unitSize);

}

void MainWindow::drawTemperatures(QPainter *p, int xoffset, int yoffset)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(xoffset, yoffset, unitSize*unitsNum, unitSize*0.4);
    QFont font;
    font.setPixelSize(unitSize*0.36);
    p->setFont(font);
    p->setPen(Qt::green);
    p->drawText(xoffset+unitSize*0.10, yoffset+unitSize*0.35, "Температура:");
    for (int i = 0; i < unitsNum; i++)
        drawTempUnit(p, xoffset+(i*unitSize), yoffset+unitSize*0.4, i);
}

void MainWindow::drawTempUnit(QPainter *p, int x, int y, int num)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(x, y, unitSize, unitSize);

    QString high, low;
    if(dayTemps.size() >= unitsNum*2)
    {
        if (dayTemps[num*2] > 0)
            high = QString("+%1").arg(dayTemps[num*2]);
        else
            high = QString("%1").arg(dayTemps[num*2]);
        if (dayTemps[num*2+1] > 0)
            low = QString("+%1").arg(dayTemps[num*2+1]);
        else
            low = QString("%1").arg(dayTemps[num*2+1]);

        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::green);
        p->drawText(x+unitSize*0.12, y+unitSize*0.48, high);
        p->setPen(Qt::darkGreen);
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, low);
    }
    else
    {
        high = "—";
        low = "—";
    }
}

void MainWindow::parseTemperature(QString str)
{
    int start = 1;
    int end = 0;
    // Parsing temp
    while (start != -1)
    {
        start = str.indexOf("<span class=\"unit unit_temperature_c\">");
        str.remove(0, start+38);
        end = str.indexOf("</span>");
        dayTemps.append(str.mid(0, end).replace("&minus;", "-").toInt());//end - start);
    }
}

void MainWindow::parseTitles(QString str)
{
    int start = 1;
    int end = 0;
    while (start != -1)
    {
        start = str.indexOf("weather-icon tooltip");
        str.remove(0, start+32);
        end = str.indexOf(">");
        dayTitles.append(str.mid(0, end).remove("\""));//end - start);
    }
    if (dayTitles.size() > 14)
        for (int i = 0; i < dayTitles.size()-14; i++)
            dayTitles.pop_back();
}

// тут проблема в том, что иногда бывают прочерки вместо значений и надо понять, как это ловить
// (у них по-другому называется юнит, в котором они находятся)
void MainWindow::parseWind(QString str)
{
    int start = 1;
    int end = 0;
    while (start != -1)
    {
        start = str.indexOf("wind-unit unit unit_wind_m_s");
        str.remove(0, start+30);
        end = str.indexOf("</span>");
        dayWind.append(str.mid(0, end).toInt());//end - start);
    }
}

void MainWindow::parsePercipitation(QString str)
{
    int start = 1;
    int end = 0;
    while (start != -1)
    {
        start = str.indexOf("item-unit unit-blue");
        str.remove(0, start+21);
        end = str.indexOf("</div>");
        qDebug() << str.mid(0, end);
        dayPercipitation.append(str.mid(0, end).replace(",", ".").toFloat());//end - start);
    }
}

void MainWindow::finished(QNetworkReply* r)
{
    if (!r->error())
    {
        QByteArray replyString = r->readAll();
        QString tempStr = replyString;

        parseTemperature(tempStr);
        parseTitles(tempStr);
        parseWind(tempStr);
        parsePercipitation(tempStr);

        qDebug() << dayTemps;
        qDebug() << dayTitles;
        qDebug() << dayWind;
        qDebug() << dayPercipitation;
        this->update();
    }
    else
        qDebug() << "Reply contains errors!";
}

void MainWindow::sslErrors(QNetworkReply* r, QList<QSslError> errors)
{
    Q_UNUSED(r);
    qDebug() << errors;
}
