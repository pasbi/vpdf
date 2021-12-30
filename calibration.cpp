#include "calibration.h"
#include "measurerect.h"
#include <QSettings>

namespace
{

constexpr auto UNIT_KEY = "unit";
constexpr auto FACTOR_KEY = "factor";

}  // namespace

Calibration::Calibration(const QString& unit, double unit_factor)
  : m_unit(unit), m_unit_factor(unit_factor)
{
}

MeasureRect Calibration::apply_to(const MeasureRect& rect, const double zoom_factor) const
{
  return rect.to_unit(m_unit, m_unit_factor / zoom_factor);
}

void Calibration::store() const
{
  QSettings settings;
  settings.setValue(UNIT_KEY, m_unit);
  settings.setValue(FACTOR_KEY, m_unit_factor);
}

void Calibration::restore()
{
  QSettings settings;
  m_unit = settings.value(UNIT_KEY, "").toString();
  m_unit_factor = settings.value(FACTOR_KEY, 1.0).toDouble();
}
