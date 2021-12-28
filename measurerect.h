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
  double d() const;
  double dx() const;
  double dy() const;
  double area() const;
  std::pair<QLineF, QLineF> start_cross(const QSize& size) const;
  std::pair<QLineF, QLineF> end_cross(const QSize& size) const;
  const QString info() const;
  MeasureRect to_unit(const QString& unit, double factor) const;
  QRectF rect() const;

private:
  QPointF m_start;
  QPointF m_end;
  QString m_unit;
};
