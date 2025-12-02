#ifndef AESA_1D_HPP
#define AESA_1D_HPP

#include <cstddef>
#include <cmath>
#include <algorithm>
#include <limits>

namespace aesa_1d
{

// ==================== CONSTANTS ====================

constexpr double M_PI_RAD       = 3.1415927;
constexpr double m_euler_number = 2.71828;

// ==================== BASIC WAVE HELPERS ====================

// λ = v / f  (wavelength in feet)
inline double wavelength_from_velocity(double wave_velocity_ft_per_sec,
                                       double frequency_hz)
{
    return wave_velocity_ft_per_sec / frequency_hz;
}

// k = 2π / λ  (wavenumber in rad/ft)
inline double wavenumber_from_wavelength(double wavelength_ft)
{
    return 2.0 * M_PI_RAD / wavelength_ft;
}

// ==================== ULA PHASE & ARRAY FACTOR ====================

// Phase parameter for ULA:
// u = k * d * (sin(steer_angle) - sin(observation_angle))
// steer_angle_rad       = ψ
// observation_angle_rad = θ
// wavenumber_rad_per_ft = k
// element_spacing_ft    = d
inline double calculate_u(double steer_angle_rad,
                          double observation_angle_rad,
                          double wavenumber_rad_per_ft,
                          double element_spacing_ft)
{
    return wavenumber_rad_per_ft * element_spacing_ft *
           (std::sin(steer_angle_rad) -
            std::sin(observation_angle_rad));
}

// Array factor (your chosen form):
// AF(θ) = | sin(u/2) * sin(Nu/2) |
inline double array_factor_from_u(double u, int num_elements)
{
    const double half_u       = 0.5 * u;
    const double sin_half_u   = std::sin(half_u);
    const double sin_N_half_u = std::sin(num_elements * half_u);
    const double product      = sin_half_u * sin_N_half_u;

    return std::abs(product); // | sin(u/2) * sin(Nu/2) |
}

// Null direction test (textbook condition):
// sin(Nu/2) = 0  → nulls
inline bool is_null_direction(double u, int num_elements)
{
    const double arg = 0.5 * static_cast<double>(num_elements) * u; // Nu/2
    return std::abs(std::sin(arg)) < 1e-12;
}

// Main lobe angle = steering angle
inline double main_lobe_angle_rad(double steer_angle_rad)
{
    return steer_angle_rad;
}

// ==================== BEAMWIDTH FORMULAS ====================

// First Null Beamwidth (FNBW) ≈ 2λ / (N d)
inline double FNBW_rad(double wavelength_ft,
                       int    num_elements,
                       double element_spacing_ft)
{
    return (2.0 * wavelength_ft) /
           (static_cast<double>(num_elements) * element_spacing_ft);
}

// Half-Power Beamwidth (HPBW) ≈ 2λ / (N d cos θ)
inline double HPBW_rad(double wavelength_ft,
                       int    num_elements,
                       double element_spacing_ft,
                       double observation_angle_rad)
{
    return (2.0 * wavelength_ft) /
           (static_cast<double>(num_elements) *
            element_spacing_ft *
            std::cos(observation_angle_rad));
}

// Broadside HPBW (θ = 0): HPBW ≈ 2λ / (N d)
inline double HPBW_broadside_rad(double wavelength_ft,
                                 int    num_elements,
                                 double element_spacing_ft)
{
    return (2.0 * wavelength_ft) /
           (static_cast<double>(num_elements) * element_spacing_ft);
}

// ==================== GRATING LOBES ====================

// Grating lobe condition:
//   sin(theta_g) = sin(psi) + m * (lambda / d)
//
// Returns grating-lobe angle theta_g (radians) for order m.
// If RHS is outside [-1, 1], returns NaN (no real grating lobe).
inline double grating_lobe_angle_rad(double steer_angle_rad,
                                     int    m,
                                     double wavelength_ft,
                                     double element_spacing_ft)
{
    const double rhs = std::sin(steer_angle_rad) +
                       m * (wavelength_ft / element_spacing_ft);

    if (rhs < -1.0 || rhs > 1.0)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return std::asin(rhs); // theta_g
}

// Rule to avoid grating lobes: d <= λ / 2
inline bool spacing_is_safe_no_grating_lobes(double element_spacing_ft,
                                             double wavelength_ft)
{
    return element_spacing_ft <= 0.5 * wavelength_ft;
}


// ==================== ARRAY GAIN ====================

// Approximate array gain: G ≈ N
inline double array_gain_linear(int num_elements)
{
    return static_cast<double>(num_elements);
}

// G_dB = 10 * log10(N)
inline double array_gain_dB(int num_elements)
{
    return 10.0 * std::log10(static_cast<double>(num_elements));
}

// ==================== RADAR RANGE EQUATION ====================

// Received power:
//   P_r = (P_t * G^2 * λ^2 * σ) / ( (4π)^3 * R^4 )
inline double radar_received_power(double transmit_power,
                                   double array_gain_linear_value,
                                   double wavelength_ft,
                                   double rcs,
                                   double range_ft)
{
    const double four_pi     = 4.0 * M_PI_RAD;
    const double four_pi_cub = four_pi * four_pi * four_pi;
    const double G2          = array_gain_linear_value *
                               array_gain_linear_value;
    const double lambda2     = wavelength_ft * wavelength_ft;
    const double R4          = range_ft * range_ft * range_ft * range_ft;

    return (transmit_power * G2 * lambda2 * rcs) /
           (four_pi_cub * R4);
}

// ==================== DOPPLER ====================

// Doppler frequency: f_d = (2 * v_r) / λ
inline double doppler_frequency(double radial_velocity_ft_per_sec,
                                double wavelength_ft)
{
    return (2.0 * radial_velocity_ft_per_sec) / wavelength_ft;
}

// Doppler phase evolution: Δφ(t) = 2π f_d t
inline double doppler_phase_delta(double doppler_freq_hz,
                                  double time_seconds)
{
    return 2.0 * M_PI_RAD * doppler_freq_hz * time_seconds;
}

// Instantaneous phase: φ(t) = φ0 + 2π f_d t
inline double doppler_phase_instant(double phi0_rad,
                                    double doppler_freq_hz,
                                    double time_seconds)
{
    return phi0_rad + 2.0 * M_PI_RAD * doppler_freq_hz * time_seconds;
}

// ==================== ANGLE ESTIMATION ====================

// Angle from phase difference between two elements:
//   psi = arcsin( (λ * Δφ) / (2π d) )
inline double angle_from_phase_difference(double phase_difference_rad,
                                          double wavelength_ft,
                                          double element_spacing_ft)
{
    const double arg =
        (wavelength_ft * phase_difference_rad) /
        (2.0 * M_PI_RAD * element_spacing_ft);

    const double clamped = std::clamp(arg, -1.0, 1.0);

    return std::asin(clamped); // psi (radians)
}

// Angle uncertainty (resolution limit):
//   Δpsi_min ≈ λ / (N d)
inline double angle_resolution_limit(double wavelength_ft,
                                     int    num_elements,
                                     double element_spacing_ft)
{
    return wavelength_ft /
           (static_cast<double>(num_elements) * element_spacing_ft);
}

} // namespace aesa_1d

#endif // AESA_1D_HPP













}