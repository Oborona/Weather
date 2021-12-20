#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    unitsNum = 14;
    unitSize = 50;

    this->resize(unitsNum*unitSize+20, unitSize*6.4);
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
    int xoffset = unitSize/5;
    int yoffset = unitSize/5;

    QPen pen(Qt::black, 2);
    QPainter painter(this);
    painter.setPen(pen);
    for (int i = 0; i < unitsNum; i++)
        painter.drawRect(xoffset+i*unitSize, yoffset, unitSize, unitSize);


    painter.setBrush(Qt::darkGray);
    painter.drawRect(0, 0, xoffset*2 + unitSize*unitsNum, yoffset*2 + 6*unitSize);

    drawDateTitle(&painter, xoffset, yoffset);
    drawTemperatures(&painter, xoffset, yoffset+unitSize*1.5);
    drawWind(&painter, xoffset, yoffset+unitSize*3.0);
    drawPercipitation(&painter, xoffset, yoffset+unitSize*4.5);
    //drawPerc
//    for (int i = 0; i < unitsNum; i++)
//        painter.drawRect(xoffset+i*unitSize, yoffset, unitSize, unitSize);

//    for (int i = 0; i < unitsNum; i++)
//        painter.drawRect(xoffset+i*unitSize, yoffset, unitSize, unitSize);

}

void MainWindow::drawDateTitle(QPainter *p, int xoffset, int yoffset)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(xoffset, yoffset, unitSize*unitsNum, unitSize*0.4);
    QFont font;
    font.setPixelSize(unitSize*0.36);
    p->setFont(font);
    p->setPen(Qt::white);
    p->drawText(xoffset+unitSize*0.10, yoffset+unitSize*0.35, "Число/день недели:");
    for (int i = 0; i < unitsNum; i++)
        drawDateTitleUnit(p, xoffset+(i*unitSize), yoffset+unitSize*0.4, i);
}

void MainWindow::drawDateTitleUnit(QPainter *p, int x, int y, int num)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(x, y, unitSize, unitSize);

    if(dayTemps.size() >= unitsNum)
    {
        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::white);
        p->drawText(x+unitSize*0.12, y+unitSize*0.48, QString("%1").arg(dayNums[num]));
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, dayTitles[num]);
    }
    else
    {
        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::lightGray);
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, "—");
    }
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
        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::green);
        p->drawText(x+unitSize*0.12, y+unitSize*0.48, high);
        p->setPen(Qt::darkGreen);
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, low);
    }
}

void MainWindow::drawWind(QPainter *p, int xoffset, int yoffset)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(xoffset, yoffset, unitSize*unitsNum, unitSize*0.4);
    QFont font;
    font.setPixelSize(unitSize*0.36);
    p->setFont(font);
    p->setPen(Qt::cyan);
    p->drawText(xoffset+unitSize*0.10, yoffset+unitSize*0.35, "Скорость ветра:");
    for (int i = 0; i < unitsNum; i++)
        drawWindUnit(p, xoffset+(i*unitSize), yoffset+unitSize*0.4, i);
}

void MainWindow::drawWindUnit(QPainter *p, int x, int y, int num)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(x, y, unitSize, unitSize);

    QString windStr;
    if(dayTemps.size() >= unitsNum)
    {
        windStr = QString("%1").arg(dayWind[num]);
        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::cyan);
        p->drawText(x+unitSize*0.12, y+unitSize*0.48, windStr);
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, "м/c");
    }
    else
    {
        windStr = "—";
        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::cyan);
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, windStr);
    }
}

void MainWindow::drawPercipitation(QPainter *p, int xoffset, int yoffset)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(xoffset, yoffset, unitSize*unitsNum, unitSize*0.4);
    QFont font;
    font.setPixelSize(unitSize*0.36);
    p->setFont(font);
    p->setPen(Qt::lightGray);
    p->drawText(xoffset+unitSize*0.10, yoffset+unitSize*0.35, "Количество осадков:");
    for (int i = 0; i < unitsNum; i++)
        drawPercipitationUnit(p, xoffset+(i*unitSize), yoffset+unitSize*0.4, i);
}

void MainWindow::drawPercipitationUnit(QPainter *p, int x, int y, int num)
{
    QPen pen(Qt::black, 3);
    p->setPen(pen);
    p->setBrush(Qt::black);
    p->drawRect(x, y, unitSize, unitSize);

    QString windStr;
    if(dayTemps.size() >= unitsNum)
    {
        windStr = QString("%1").arg(dayPercipitation[num]);
        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::lightGray);
        p->drawText(x+unitSize*0.12, y+unitSize*0.48, windStr);
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, "мм");
    }
    else
    {
        windStr = "—";
        QFont font;
        font.setPixelSize(unitSize*0.36);
        p->setFont(font);
        p->setPen(Qt::lightGray);
        p->drawText(x+unitSize*0.34, y+unitSize*0.82, windStr);
    }
}

void MainWindow::parseTitles(QString str)
{
    int start = 1;
    int end = 0;
    while (start != -1)
    {
        start = str.indexOf("row-item item-day");
        str.remove(0, start+21);
        end = str.indexOf("</a>");

        QString mid = str.mid(0, end).remove("\"");
//        dayTitles.append();//end - start);
        start = mid.indexOf("class=day");
        mid.remove(0, start+10);
        end = mid.indexOf("</div>");
        dayTitles.append(mid.mid(0, end));

        start = mid.indexOf("class=date");
        mid.remove(0, start+11);
        end = mid.indexOf("</div>");
        dayNums.append(mid.mid(0, end).toInt());
    }
    if (dayTitles.size() > 14)
        for (int i = 0; i < dayTitles.size()-14; i++)
            dayTitles.removeLast();
    if (dayNums.size() > 14)
        for (int i = 0; i < dayNums.size()-14; i++)
            dayNums.removeLast();
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

void MainWindow::parseDescriptions(QString str)
{
    int start = 1;
    int end = 0;
    while (start != -1)
    {
        start = str.indexOf("weather-icon tooltip");
        str.remove(0, start+32);
        end = str.indexOf(">");
        dayDescriptions.append(str.mid(0, end).remove("\""));//end - start);
    }
    if (dayDescriptions.size() > 14)
        for (int i = 0; i < dayDescriptions.size()-14; i++)
            dayDescriptions.pop_back();
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
        parseDescriptions(tempStr);
        parseWind(tempStr);
        parsePercipitation(tempStr);
        parseTitles(tempStr);

        qDebug() << dayTemps;
        qDebug() << dayDescriptions;
        qDebug() << dayWind;
        qDebug() << dayPercipitation;
        qDebug() << dayTitles;
        qDebug() << dayNums;

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
