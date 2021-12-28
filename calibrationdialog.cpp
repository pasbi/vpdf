#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "measurerect.h"


CalibrationDialog::CalibrationDialog(QWidget* parent)
  : QDialog(parent)
  , m_ui(std::make_unique<Ui::CalibrationDialog>())
{
  m_ui->setupUi(this);
  setModal(false);
  on_cb_unit_currentTextChanged(m_ui->cb_unit->currentText());
}

void CalibrationDialog::set_measure_rect(const MeasureRect& rect)
{
  m_ui->sp_d->setValue(rect.d());
  m_ui->sp_dx->setValue(rect.dx());
  m_ui->sp_dy->setValue(rect.dy());
}

Calibration CalibrationDialog::calibration() const
{
  return m_calibration;
}

void CalibrationDialog::on_pb_calibrate_dx_clicked()
{
  compute_calibration(m_ui->sp_dx->value());
  accept();
}

void CalibrationDialog::on_pb_calibrate_dy_clicked()
{
  compute_calibration(m_ui->sp_dy->value());
  accept();
}

void CalibrationDialog::on_pb_calibrate_d_clicked()
{
  compute_calibration(m_ui->sp_d->value());
  accept();
}

void CalibrationDialog::on_cb_unit_currentTextChanged(const QString& text)
{
  m_ui->sp_user_value->setSuffix(QString{" %1"}.arg(text));
}

void CalibrationDialog::compute_calibration(double measure)
{
  const auto factor = m_ui->sp_user_value->value() / measure;
  m_calibration = Calibration::unit_calibration(m_ui->cb_unit->currentText(), factor);
}

CalibrationDialog::~CalibrationDialog() = default;
