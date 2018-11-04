//
// Created by Daehyun You on 11/4/18.
//

#ifndef DLTOOLS_UNITS_H
#define DLTOOLS_UNITS_H

namespace dltools { namespace units {
    const double ma = 1.66053892173e-27;  // Atomic mass
    const double c = 299792458;  // Speed of light
    const double me = 9.1093829140e-31;  // Electron rest mass
    const double e = 1.60217656535e-19;  // Elementary charge
    const double hbar = 1.05457172647e-34;  // Planck constant
    const double k = 8.9875517873681e9;  // Coulomb's constant
    const double alpha = k * e * e / hbar / c;  // Fine-structure constant
    const double bohr = hbar / me / c / alpha;  // Bohr radius
    const double hartree = alpha * alpha * me * c * c;  // Hartree energy

    inline double to_milli_meter(double v) {
        return v / 1e-3 * bohr;
    }

    inline double to_nano_sec(double v) {
        return v / 1e-9 / hartree * hbar;
    }
}}

#endif //DLTOOLS_UNITS_H
