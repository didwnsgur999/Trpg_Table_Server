#include "mainwindow.h"
#include "backend.h"
#include <QLocale>
#include <QTranslator>
#include <QApplication>

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
    std::unique_ptr<Backend> backend = std::make_unique<Backend>();
    MainWindow w(backend.get());
    w.show();
    return a.exec();
}
