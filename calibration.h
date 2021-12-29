#pragma once

#include <optional>
#include <QString>

class MeasureRect;

class Calibration
{
public:
  Calibration() = default;
  [[nodiscard]] MeasureRect apply_to(const MeasureRect& rect, double zoom_factor) const;

  [[nodiscard]] static Calibration unit_calibration(const QString& unit, double unit_factor);
  [[nodiscard]] static Calibration pixel_calibration();

protected:
  Calibration(const QString& unit,
              const std::optional<double>& unit_factor);

private:
  QString m_unit;
  std::optional<double> m_unit_factor;
};
