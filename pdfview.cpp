#include "pdfview.h"
#include "calibration.h"
#include "calibrationdialog.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <cmath>
#include "measurerect.h"


PdfView::PdfView(QWidget* parent)
  : QWidget(parent)
{
}

void PdfView::start_calibration_mode(CalibrationDialog& calibration_dialog)
{
  m_calibration_dialog = &calibration_dialog;
  update();
}

void PdfView::stop_calibration_mode()
{
  m_calibration_dialog = nullptr;
  update();
}

void PdfView::set_calibration(const Calibration& calibration)
{
  m_calibration = calibration;
  update();
}

void PdfView::set_document(Poppler::Document& document)
{
  m_cache = {};
  m_document = &document;
  update();
}

void PdfView::set_page(int page)
{
  if (m_document == nullptr) {
    m_page.reset();
  } else {
    m_page.reset(m_document->page(page));
  }
  m_cache = {};
  update();
}

void PdfView::set_zoom_factor(double factor)
{
  m_zoom_factor = factor;
  m_cache = {};
  update();
}

void PdfView::mousePressEvent(QMouseEvent* e)
{
  m_last_press_pos = e->pos();
  m_last_pos = m_last_press_pos;
}

void PdfView::mouseMoveEvent(QMouseEvent* e)
{
  m_current_pos = e->pos();

  if ((e->modifiers() & Qt::ControlModifier) != 0u) {
    set_cursor_visible(false);
    m_draw_measure = true;
  } else {
    m_pan += m_last_pos - m_current_pos;
  }
  QWidget::mouseMoveEvent(e);
  update();

  m_last_pos = e->pos();
}

void PdfView::mouseReleaseEvent(QMouseEvent*)
{
  set_cursor_visible(true);
  m_draw_measure = false;
  update();
}

void PdfView::paintEvent(QPaintEvent*)
{
  if (m_page == nullptr) {
    return;
  }

  QPainter painter{this};
  const auto& pixmap = render();
  painter.drawPixmap(rect(), pixmap, {m_pan, size()});

  if (m_draw_measure) {
    const auto color = m_calibration_dialog == nullptr ? Qt::red : Qt::blue;
    draw_measure(painter, color);
  }
}

void PdfView::set_cursor_visible(bool visible)
{
  auto cursor = this->cursor();
  cursor.setShape(visible ? Qt::ArrowCursor : Qt::BlankCursor);
  setCursor(cursor);
}

const QPixmap& PdfView::render()
{
  if (m_cache.isNull()) {
    m_cache = QPixmap{(m_page->pageSizeF() * m_zoom_factor).toSize()};
    QPainter painter{&m_cache};
    painter.fillRect(m_cache.rect(), Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    const auto s = 72 * m_zoom_factor;
    m_page->renderToPainter(&painter, s, s);
  }
  return m_cache;
}

void PdfView::draw_measure(QPainter& painter, const QColor& color)
{
  static constexpr auto font_size = 20;
  auto font = painter.font();
  font.setBold(true);
  font.setPixelSize(font_size);
  painter.setFont(font);
  painter.setRenderHint(QPainter::Antialiasing);
  QPen pen;
  pen.setColor(color);
  pen.setStyle(Qt::DashLine);
  pen.setWidth(1);
  painter.setPen(pen);
  painter.drawLine(m_last_press_pos, m_current_pos);

  pen.setStyle(Qt::SolidLine);
  painter.setPen(pen);

  const MeasureRect measure_rect{m_last_press_pos, m_current_pos, "px"};
  const auto draw_cross = [&painter](const auto& cross) {
    const auto [a, b] = cross;
    painter.drawLine(a);
    painter.drawLine(b);
  };
  draw_cross(measure_rect.start_cross(size()));
  draw_cross(measure_rect.end_cross(size()));

  if (m_calibration_dialog != nullptr) {
    m_calibration_dialog->set_measure_rect(measure_rect.to_unit("", 1.0 / m_zoom_factor));
  }

  const auto calibrated = m_calibration.apply_to(measure_rect, m_zoom_factor);

  const auto text = calibrated.info();
  Q_EMIT text_changed(text);
  const auto fm = painter.fontMetrics();
  const auto text_size = fm.boundingRect(rect(), Qt::AlignLeft, text).size();
  const auto offset = measure_rect.dy() > text_size.height() ? QPoint{0, -text_size.height()} : QPoint{};
  painter.drawText(QRect{measure_rect.rect().bottomRight().toPoint() + offset, text_size}, Qt::AlignLeft, text);
}
