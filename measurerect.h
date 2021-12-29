#pragma once

#include <QPoint>
#include <QRect>
#include <QSize>
#include <QLine>
#include <QString>

class MeasureRect
{
public:
  explicit MeasureRect(const QPointF& start, const QPointF& end, const QString& unit);
  [[nodiscard]] double d() const;
  [[nodiscard]] double dx() const;
  [[nodiscard]] double dy() const;
  [[nodiscard]] double area() const;
  [[nodiscard]] std::pair<QLineF, QLineF> start_cross(const QSize& size) const;
  [[nodiscard]] std::pair<QLineF, QLineF> end_cross(const QSize& size) const;
  [[nodiscard]] QString info() const;
  [[nodiscard]] MeasureRect to_unit(const QString& unit, double factor) const;
  [[nodiscard]] QRectF rect() const;

private:
  QPointF m_start;
  QPointF m_end;
  QString m_unit;
};
