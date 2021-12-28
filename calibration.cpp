#include "calibration.h"
#include "measurerect.h"

MeasureRect Calibration::apply_to(const MeasureRect& rect, const double zoom_factor)
{
  if (m_unit_factor) {
    return rect.to_unit(m_unit, *m_unit_factor / zoom_factor);
  }
  return rect;
}

Calibration Calibration::unit_calibration(const QString& unit, const double unit_factor)
{
  return {unit, unit_factor};
}

Calibration Calibration::pixel_calibration()
{
  return {"px", {}};
}

Calibration::Calibration(const QString& unit,
                         const std::optional<double>& unit_factor)
  : m_unit(unit), m_unit_factor(unit_factor)
{
}

QString Calibration::unit() const
{
  return m_unit;
}
