#include <iostream>
#include <QApplication>
#include "mainwindow.hpp"
#include <QLocale>

int main(int argc, char* argv[])
{
    QLocale::setDefault(QLocale::Russian);
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    QString a("АБ");
    for(auto i: a) std::cout << i.unicode() << std::endl;
    return app.exec();
}
