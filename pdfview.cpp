#include "pdfview.h"
#include <QPdfPageNavigation>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <cmath>

namespace
{

void scroll(QScrollBar& scroll_bar, int d)
{
  const auto v = std::clamp(scroll_bar.value() + d, scroll_bar.minimum(), scroll_bar.maximum());
  scroll_bar.setValue(v);
}

double l2_norm(const QPoint p)
{
  return std::sqrt(static_cast<double>(QPoint::dotProduct(p, p)));
}

}  // namespace

void PdfView::mousePressEvent(QMouseEvent* e)
{
  m_last_press_pos = e->pos();
  m_last_pos = m_last_press_pos;
}

void PdfView::mouseMoveEvent(QMouseEvent* e)
{
  m_current_pos = e->pos();

  if (e->modifiers() & Qt::ControlModifier) {
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
  m_draw_measure = false;
  QPdfView::mouseReleaseEvent(e);
  viewport()->update();
}

void PdfView::paintEvent(QPaintEvent *e)
{
  QPdfView::paintEvent(e);
  if (m_draw_measure) {
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(m_last_press_pos, m_current_pos);

    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    static constexpr auto sety = [](QPoint p, int y) { p.setY(y); return p; };
    static constexpr auto setx = [](QPoint p, int x) { p.setX(x); return p; };
    painter.drawLine(setx(m_last_press_pos, 0), setx(m_last_press_pos, viewport()->width()));
    painter.drawLine(setx(m_current_pos, 0), setx(m_current_pos, viewport()->width()));
    painter.drawLine(sety(m_last_press_pos, 0), sety(m_last_press_pos, viewport()->height()));
    painter.drawLine(sety(m_current_pos, 0), sety(m_current_pos, viewport()->height()));


    const QRectF measure_rect{m_last_press_pos, m_current_pos};
    const auto d = l2_norm(m_last_press_pos - m_current_pos);
    const auto dx = measure_rect.width();
    const auto dy = measure_rect.height();

    const auto text = QString{"Δx = %2 %1\nΔy = %3 %1\nΔ = %4 %1\nA= %5 %1"}
                        .arg("px").arg(dx).arg(dy).arg(d).arg(dx * dy) + "²";

    const auto fm = painter.fontMetrics();
    const auto text_size = fm.boundingRect(rect(), Qt::AlignLeft, text).size();
    const auto offset = dy > text_size.height() ? QPoint{0, -text_size.height()} : QPoint{};
    painter.drawText(QRect{measure_rect.bottomRight().toPoint() + offset, text_size}, Qt::AlignLeft, text);
  }
}
