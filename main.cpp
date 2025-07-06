#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "backend.h"
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Trpg_Table_Server_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Backend::getInstance().loadProductJson("products.json");
    Backend::getInstance().loadCustomerJson("customer.json");
    Backend::getInstance().loadOrderJson("order.json");
    Backend::getInstance().ShowProductList();
    Backend::getInstance().ShowCustomerList();
    Backend::getInstance().ShowOrderList();
    MainWindow w;
    w.show();
    return a.exec();
}
