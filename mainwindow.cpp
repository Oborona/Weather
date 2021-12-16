#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    QNetworkAccessManager* nam = new QNetworkAccessManager();
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    connect(nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    QString url = "https://www.gismeteo.ru/weather-moscow-4368/10-days/";
    QNetworkRequest request;
    request.setUrl(url);
    nam->get(request);

    edit = new QTextEdit();
    layout = new QGridLayout();
    layout->addWidget(edit, 0, 0);
    this->setLayout(layout);
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
            fprintf(stderr, "\t");
        for (int j = 0; j < tabLevel; j++)
            single.prepend("    ");

        edit->append(single);
        fprintf(stderr, "%s\n", single.toStdString().data());
        if (single.contains("}"))
            tabLevel--;
    }
}

void MainWindow::finished(QNetworkReply* r)
{
    if (!r->error())
    {
        QByteArray replyString = r->readAll();
        QString str = replyString;
        int start = str.indexOf("M.state.popup.dates");
        int end = str.indexOf("M.state.popup.horizontal");
        QString jsonStr = str.mid(start, end - start);
//        qDebug() << jsonStr.simplified();
        jsonStr.remove(0, 22);

        printJson(jsonStr);
        QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toLocal8Bit());
        QJsonObject json = doc.object();
//        QStringList strl = str.split("\\n");
//        for (int i = 0; i < strl.size(); i++)
//        {
//            str = strl[i];
//            if (str.contains("M.state.popup.dates"))
//                qDebug() << strl;
//        }
    }
    else
        qDebug() << "Reply contains errors!";
}

void MainWindow::sslErrors(QNetworkReply* r, QList<QSslError> errors)
{
    Q_UNUSED(r);
    qDebug() << errors;
}
