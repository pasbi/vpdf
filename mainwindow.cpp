#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtPdfWidgets/QPdfView>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QPdfPageNavigation>
#include <QDebug>
#include <cmath>
#include "pdfview.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_ui(std::make_unique<Ui::MainWindow>())
{
  m_ui->setupUi(this);
  m_ui->pdf_view->setDocument(&m_doc);
}

bool MainWindow::open_document(const QString& filename)
{
  if (const auto error = m_doc.load(filename); error != QPdfDocument::NoError) {
    QMessageBox::critical(this,
                          tr("Failed to open PDF"),
                          tr("Failed to open PDF document %1: %2").arg(filename).arg(error));
    return false;
  }

  m_ui->sp_page->setRange(1, m_doc.pageCount());

  return true;
}

MainWindow::~MainWindow() = default;

void MainWindow::on_action_Open_triggered()
{
  static constexpr auto LAST_LOCATION_KEY = "last-location";
  const auto last_location = QSettings{}.value(LAST_LOCATION_KEY, QDir::homePath()).toString();
  const auto filename = QFileDialog::getOpenFileName(this,
                                                     tr("Open PDF"),
                                                     last_location,
                                                     tr("PDF documents (*.pdf)"));
  if (!filename.isEmpty() && open_document(filename)) {
    QSettings{}.setValue(LAST_LOCATION_KEY, filename);
  }
}

void MainWindow::on_action_Exit_triggered()
{
  QApplication::quit();
}

void MainWindow::on_sp_page_valueChanged(int page)
{
  m_ui->pdf_view->pageNavigation()->setCurrentPage(page - 1);
}

void MainWindow::on_sp_zoom_valueChanged(double exponent)
{
  static constexpr auto base = 10.0;
  const double zoom = std::pow(base, exponent);
  m_ui->pdf_view->setZoomFactor(zoom);
}

