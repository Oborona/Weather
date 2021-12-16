#include <QApplication>

#include <mainwindow.h>

int main(int ac, char** av)
{
    QApplication* app = new QApplication(ac, av);
    MainWindow* mw = new MainWindow();
    mw->show();
    return app->exec();
}
