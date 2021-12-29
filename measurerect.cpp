#include "measurerect.h"
#include <cmath>

namespace
{

QPointF sety(QPointF p, double y)
{
  p.setY(y);
  return p;
}

QPointF setx(QPointF p, double x)
{
  p.setX(x);
  return p;
}

auto cross(const QPointF& pos, const QSize& size)
{
  return std::pair{
    QLineF{setx(pos, 0), setx(pos, size.width())},
    QLineF{sety(pos, 0), sety(pos, size.height())}
  };
}

}  // namespace

MeasureRect::MeasureRect(const QPointF& start, const QPointF& end, const QString& unit)
  : m_start(start)
  , m_end(end)
  , m_unit(unit)
{
}

double MeasureRect::d() const
{
  const auto d = m_start - m_end;
  return std::sqrt(QPointF::dotProduct(d, d));
}

double MeasureRect::dx() const
{
  return std::abs(m_start.x() - m_end.x());
}

double MeasureRect::dy() const
{
  return std::abs(m_start.y() - m_end.y());
}

double MeasureRect::area() const
{
  return dx() * dy();
}

std::pair<QLineF, QLineF> MeasureRect::start_cross(const QSize& size) const
{
  return cross(m_start, size);
}

std::pair<QLineF, QLineF> MeasureRect::end_cross(const QSize& size) const
{
  return cross(m_end, size);
}

QString MeasureRect::info() const
{
  return QString{"Δx = %2 %1\nΔy = %3 %1\nΔ = %4 %1\nA= %5 %1"}
           .arg(m_unit).arg(dx()).arg(dy()).arg(d()).arg(area()) + "²";
}

MeasureRect MeasureRect::to_unit(const QString& unit, double factor) const
{
  return MeasureRect{factor * m_start, factor * m_end, unit};
}

QRectF MeasureRect::rect() const
{
  return {m_start, m_end};
}
