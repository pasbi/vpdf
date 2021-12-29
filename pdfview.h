#pragma once

#include "calibration.h"
#include "poppler/qt5/poppler-qt5.h"
#include <QWidget>

class CalibrationDialog;

class PdfView : public QWidget
{
  Q_OBJECT
public:
  explicit PdfView(QWidget* parent = nullptr);
  void start_calibration_mode(CalibrationDialog& calibration_dialog);
  void stop_calibration_mode();
  void set_calibration(const Calibration& calibration);
  void set_document(Poppler::Document& document);
  void set_page(int page);
  void set_zoom_factor(double factor);

Q_SIGNALS:
  void text_changed(const QString& text);

protected:
  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void paintEvent(QPaintEvent*) override;

private:
  QPoint m_last_press_pos;
  QPoint m_last_pos;
  QPoint m_current_pos;
  QPoint m_pan;
  bool m_draw_measure = false;
  Calibration m_calibration;
  CalibrationDialog* m_calibration_dialog = nullptr;
  void set_cursor_visible(bool visible);
  double m_zoom_factor = 1.0;
  Poppler::Document* m_document = nullptr;
  std::unique_ptr<Poppler::Page> m_page;

  const QPixmap& render();
  QPixmap m_cache;
  void draw_measure(QPainter& painter, const QColor& color);
};
