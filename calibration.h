#pragma once

#include <optional>
#include <QString>

class MeasureRect;

class Calibration
{
public:
  Calibration(const QString& unit, double unit_factor);
  Calibration() = default;
  [[nodiscard]] MeasureRect apply_to(const MeasureRect& rect, double zoom_factor) const;

  void store() const;
  void restore();

private:
  QString m_unit;
  double m_unit_factor;
};
