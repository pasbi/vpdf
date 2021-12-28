#pragma once

#include <QtPdfWidgets/QPdfView>

class PdfView : public QPdfView
{
  Q_OBJECT
public:
  using QPdfView::QPdfView;

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
};
