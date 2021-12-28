#include "pdfview.h"
#include "calibration.h"
#include "calibrationdialog.h"
#include <QPdfPageNavigation>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include "measurerect.h"

namespace
{

void scroll(QScrollBar& scroll_bar, int d)
{
  const auto v = std::clamp(scroll_bar.value() + d, scroll_bar.minimum(), scroll_bar.maximum());
  scroll_bar.setValue(v);
}

}  // namespace

void PdfView::start_calibration_mode(CalibrationDialog& calibration_dialog)
{
  m_calibration_dialog = &calibration_dialog;
  viewport()->update();
}

void PdfView::stop_calibration_mode()
{
  m_calibration_dialog = nullptr;
  viewport()->update();
}

void PdfView::set_calibration(const Calibration& calibration)
{
  m_calibration = calibration;
  viewport()->update();
}

void PdfView::mousePressEvent(QMouseEvent* e)
{
  m_last_press_pos = e->pos();
  m_last_pos = m_last_press_pos;
}

void PdfView::mouseMoveEvent(QMouseEvent* e)
{
  m_current_pos = e->pos();

  if (e->modifiers() & Qt::ControlModifier) {
    set_cursor_visible(false);
    m_draw_measure = true;
  } else {
    const auto diff = m_last_pos - m_current_pos;
    ::scroll(*horizontalScrollBar(), diff.x());
    ::scroll(*verticalScrollBar(), diff.y());
  }
  QWidget::mouseMoveEvent(e);
  viewport()->update();

  m_last_pos = e->pos();
}

void PdfView::mouseReleaseEvent(QMouseEvent* e)
{
  set_cursor_visible(true);
  m_draw_measure = false;
  QPdfView::mouseReleaseEvent(e);
  viewport()->update();
}

void PdfView::paintEvent(QPaintEvent *e)
{
  QPdfView::paintEvent(e);
  if (m_draw_measure) {
    const auto color = m_calibration_dialog == nullptr ? Qt::red : Qt::blue;

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(color);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(m_last_press_pos, m_current_pos);

    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    const MeasureRect measure_rect{m_last_press_pos, m_current_pos, "px"};
    const auto draw_cross = [&painter](const auto& cross) {
      const auto [a, b] = cross;
      painter.drawLine(a);
      painter.drawLine(b);
    };
    draw_cross(measure_rect.start_cross(viewport()->size()));
    draw_cross(measure_rect.end_cross(viewport()->size()));

    if (m_calibration_dialog != nullptr) {
      m_calibration_dialog->set_measure_rect(measure_rect.to_unit("", 1.0 / zoomFactor()));
    }

    const auto calibrated = m_calibration.apply_to(measure_rect, zoomFactor());

    const auto text = calibrated.info();
    const auto fm = painter.fontMetrics();
    const auto text_size = fm.boundingRect(rect(), Qt::AlignLeft, text).size();
    const auto offset = measure_rect.dy() > text_size.height() ? QPoint{0, -text_size.height()} : QPoint{};
    painter.drawText(QRect{measure_rect.rect().bottomRight().toPoint() + offset, text_size}, Qt::AlignLeft, text);
  }
}

void PdfView::set_cursor_visible(bool visible)
{
  auto cursor = this->cursor();
  cursor.setShape(visible ? Qt::ArrowCursor : Qt::BlankCursor);
  setCursor(cursor);
}

