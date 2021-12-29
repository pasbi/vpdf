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
      QApplication::installTranslator(&translator);
      break;
    }
  }

  MainWindow w;
  if (const auto args = QApplication::arguments(); args.size() == 2) {
    w.open_document(args.at(1));
  }

  w.show();
  return QApplication::exec();
}
