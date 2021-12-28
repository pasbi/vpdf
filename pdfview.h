#pragma once

#include <QtPdfWidgets/QPdfView>
#include "calibration.h"

class CalibrationDialog;

class PdfView : public QPdfView
{
  Q_OBJECT
public:
  using QPdfView::QPdfView;
  void start_calibration_mode(CalibrationDialog& calibration_dialog);
  void stop_calibration_mode();
  void set_calibration(const Calibration& calibration);

protected:
  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;
  void paintEvent(QPaintEvent* e) override;

private:
  QPoint m_last_press_pos;
  QPoint m_last_pos;
  QPoint m_current_pos;
  bool m_draw_measure = false;
  Calibration m_calibration;
  CalibrationDialog* m_calibration_dialog = nullptr;
  void set_cursor_visible(bool visible);
};
