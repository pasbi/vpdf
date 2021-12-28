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
#include "calibration.h"
#include "calibrationdialog.h"

namespace
{
  constexpr auto index_pixel = 0;
  constexpr auto index_pdf_unit = 1;
  constexpr auto index_manual = 2;
  constexpr auto zoom_levels = std::array{0.1, 0.3, 0.5, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 7.0, 10.0, 15.0};
}  // namespace

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_ui(std::make_unique<Ui::MainWindow>())
  , m_calibration_dialog(std::make_unique<CalibrationDialog>(this))
{
  m_ui->setupUi(this);
  m_ui->pdf_view->setDocument(&m_doc);
  connect(m_calibration_dialog.get(), &QDialog::finished, m_ui->pdf_view, &PdfView::stop_calibration_mode);
  connect(m_calibration_dialog.get(), &QDialog::accepted, m_ui->pdf_view, [this]() {
    m_ui->pdf_view->set_calibration(m_calibration_dialog->calibration());
  });
  connect(m_ui->cb_calibration, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int index) {
    m_ui->pdf_view->set_calibration([index, this]() {
      switch (index) {
      case index_pixel:
        return Calibration::pixel_calibration();
      case index_pdf_unit:
        return Calibration::unit_calibration("", 1.0);
      case index_manual:
        return m_calibration_dialog->calibration();
      default:
        Q_UNREACHABLE();
        return Calibration::pixel_calibration();
      }
    }());
  });
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

void MainWindow::zoom(int increment)
{
  m_zoom = std::clamp(m_zoom + increment, static_cast<std::size_t>(0), zoom_levels.size() - 1);
  m_ui->pdf_view->setZoomFactor(zoom_levels[m_zoom]);
}

void MainWindow::on_pb_calibrate_clicked()
{
  m_ui->pdf_view->start_calibration_mode(*m_calibration_dialog);
  m_calibration_dialog->show();
}

void MainWindow::on_cb_calibration_currentIndexChanged(int index)
{
  m_ui->pb_calibrate->setEnabled(index_manual == index);
}

void MainWindow::on_pb_zoom_in_clicked()
{
  zoom(1);
}

void MainWindow::on_pb_zoom_out_clicked()
{
  zoom(-1);
}

