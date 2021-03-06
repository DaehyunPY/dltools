#ifndef DLTOOLS_SACLA_MODEL_H
#define DLTOOLS_SACLA_MODEL_H

#include <math.h>
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include "../hittype.h"

namespace dltools
{
namespace sacla
{
/**
 * SACLA momentum model in the z direction.
 * @param r Detected location in the r direction. This value has to be after calibration.
 * @param t Detected time (flight time). This value has to be after calibration.
 * @param coeffs Coefficients of the model.
 * @return Momentum in the z direction.
 */
double _pz_model(double r, double t, const std::array<double, 7> &coeffs);

/**
 * SACLA momentum model in the r direction.
 * @param r Detected location in the r direction. This value has to be after calibration.
 * @param t Detected time. This value has to be after calibration.
 * @param coeffs Coefficients of the model.
 * @return Momentum in the r direction.
 */
double _pr_model(double r, double t, const std::array<double, 6> &coeffs);

/**
 * SACLA momentum model.
 */
class Model
{
    const double __mass, __fr, __to, __t0, __th, __x0, __y0, __dx, __dy;
    const std::array<double, 7> __pz_coeffs;
    const std::array<double, 6> __pr_coeffs;

  public:
    /**
     * Initialize SACLA momentum model.
     * @param mass Mass of the model particle, such as ion or electron.
     * @param pz_coeffs Coefficients of momentum model in the z direction.
     * @param pr_coeffs Coefficients of momentum model in the r direction.
     * @param fr Lower limit of flight time.
     * @param to Upper limit of flight time.
     * @param x1 A calibration factor.
     * @param y1 A calibration factor.
     */
    Model(double mass, std::array<double, 7> pz_coeffs, std::array<double, 6> pr_coeffs,
          double fr = 0, double to = INFINITY,
          double t0 = 0, double th = 0, double x0 = 0, double y0 = 0, double dx = 1, double dy = 1);

    /**
     * Analyze momentum of a Hit. Hit (t, x, y) will be calibrated to...
     *     (t) -> (t - t0),
     *     (x, y) -> ((x, y) * rot(th) - (x0, y0)) * (dx, dy).
     * @param hit Hit to be analyzed.
     * @return A pointer of momentum and kinetic energy. If the Hit is within the flight time limit,
     * the pointer will have a value, if not, it will be nullptr.
     */
    std::shared_ptr<AnalyzedHit> operator()(const Hit &hit) const;

    inline double _mass() const
    {
        return __mass;
    }

    inline double _fr() const
    {
        return __fr;
    }

    inline double _to() const
    {
        return __to;
    }

    inline double _t0() const
    {
        return __t0;
    }

    inline double _th() const
    {
        return __th;
    }

    inline double _x0() const
    {
        return __x0;
    }

    inline double _y0() const
    {
        return __y0;
    }

    inline double _dx() const
    {
        return __dx;
    }

    inline double _dy() const
    {
        return __dy;
    }

    inline std::array<double, 7> _pz_coeffs() const
    {
        return __pz_coeffs;
    }

    inline std::array<double, 6> _pr_coeffs() const
    {
        return __pr_coeffs;
    }
};

/**
 * SACLA momentum models.
 */
class Models
{
    const std::unordered_map<std::string, Model> __models;

  public:
    /**
     * Initialize SACLA momentum models.
     * @param models Map to Model.
     * @param t0 A calibration factor.
     * @param th A calibration factor in radians.
     * @param x0 A calibration factor.
     * @param y0 A calibration factor.
     * @param dx A calibration factor.
     * @param dy A calibration factor.
     */
    explicit Models(std::unordered_map<std::string, Model> models);

    /**
     * Analyze momentum of a Hit.
     * @param hit Hit to be analyzed.
     * @return Hit with a map to momentum and kinetic energy.
     */
    Hit operator()(const Hit &hit) const;

    /**
     * Analyze momentum of Hits.
     * @param hits Hits to be analyzed.
     * @return Hits with maps to momentum and kinetic energy.
     */
    dltools::Hits operator()(const dltools::Hits &hits) const;

    inline const std::unordered_map<std::string, Model> &_models() const
    {
        return __models;
    }
};
} // namespace sacla
} // namespace dltools

#endif //DLTOOLS_SACLA_MODEL_H
