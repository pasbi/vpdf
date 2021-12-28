#pragma once

#include "calibration.h"
#include <memory>
#include <QDialog>

QT_BEGIN_NAMESPACE
  namespace Ui { class CalibrationDialog; }
QT_END_NAMESPACE

class CalibrationDialog : public QDialog
{
  Q_OBJECT
public:
  explicit CalibrationDialog(QWidget* parent = nullptr);
  ~CalibrationDialog() override;

  void set_measure_rect(const MeasureRect& rect);
  Calibration calibration() const;

public Q_SLOTS:
  void on_pb_calibrate_dx_clicked();
  void on_pb_calibrate_dy_clicked();
  void on_pb_calibrate_d_clicked();
  void on_cb_unit_currentTextChanged(const QString& text);

private:
  Calibration m_calibration;
  std::unique_ptr<Ui::CalibrationDialog> m_ui;
  void compute_calibration(double measure);
};
