#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication::setApplicationDisplayName("vpdf");
  QApplication::setApplicationName("vpdf");
  QApplication::setOrganizationName("vpdf-developers");
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "vpdf_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  MainWindow w;
  if (a.arguments().size() == 2) {
    w.open_document(a.arguments().at(1));
  }

  w.show();
  return a.exec();
}
