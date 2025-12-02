1-D AESA Radar Simulation — C++ Project

A structured, object-oriented implementation of the fundamental physics and signal-processing math behind a 1-Dimensional Active Electronically Scanned Array (AESA) radar.

This project demonstrates:

RF propagation fundamentals

Phase-steered uniform linear arrays

Array factor computation

Beamwidth and main-lobe estimation

Null detection

Grating lobe conditions

Array gain in linear and dB form

Radar range equation

Doppler frequency and phase evolution

Angle estimation via phase differences

All computation is implemented through dedicated C++ classes, emphasizing modular, aerospace-grade software structure.

Project Structure
Core Classes
Class	Purpose
aesa_1d::RF_Settings	Computes wavelength λ and wavenumber k from radar frequency.
Calculation_Settings	Computes the phase term 
𝑢
=
𝑘
𝑑
(
sin
⁡
𝜓
−
sin
⁡
𝜃
)
u=kd(sinψ−sinθ).
Array_Settings	Computes the array factor for a uniform linear array.
Null_Direction_Testing	Detects when the array factor is in a null condition.
Main_lobe_angle_rad_Settings	Returns the main-lobe steering angle ψ.
Beamwidth_Settings	Computes FNBW and HPBW for the array.
Grating_lobe_condition_Settings	Computes grating-lobe RHS and angle.
aesa_1d::Array_Gain_Settings	Computes array gain (linear and dB).
aesa_1d::Radar_Range_Equation_Settings	Implements the monostatic radar range equation.
aesa_1d::Doppler_Settings	Computes Doppler frequency and phase evolution.
aesa_1d::Angle_Estimation_Settings	Computes angle from phase difference and resolution limit.
Mathematical Foundations
Phase Term
𝑢
=
𝑘
𝑑
(
sin
⁡
𝜓
−
sin
⁡
𝜃
)
u=kd(sinψ−sinθ)
Array Factor
𝐴
𝐹
(
𝑢
)
=
sin
⁡
(
𝑁
𝑢
/
2
)
sin
⁡
(
𝑢
/
2
)
AF(u)=
sin(u/2)
sin(Nu/2)
	​

Beamwidth

First Null Beamwidth:

F
N
B
W
≈
2
𝜆
𝑁
𝑑
FNBW≈
Nd
2λ
	​


Half-Power Beamwidth:

H
P
B
W
≈
0.886
𝜆
𝑁
𝑑
cos
⁡
𝜓
HPBW≈
Ndcosψ
0.886λ
	​

Grating Lobe Condition
sin
⁡
𝜃
𝑚
=
sin
⁡
𝜓
+
𝑚
𝜆
𝑑
sinθ
m
	​

=sinψ+m
d
λ
	​

Array Gain
𝐺
lin
≈
𝑁
,
𝐺
dB
=
10
log
⁡
10
𝑁
G
lin
	​

≈N,G
dB
	​

=10log
10
	​

N
Radar Range Equation
𝑃
𝑟
=
𝑃
𝑡
𝐺
2
𝜆
2
𝜎
(
4
𝜋
)
3
𝑅
4
P
r
	​

=
(4π)
3
R
4
P
t
	​

G
2
λ
2
σ
	​

Doppler Frequency
𝑓
𝑑
=
2
𝑣
𝑟
𝜆
f
d
	​

=
λ
2v
r
	​

	​

Angle Estimation
𝜓
=
arcsin
⁡
(
𝜆
Δ
𝜙
2
𝜋
𝑑
)
ψ=arcsin(
2πd
λΔϕ
	​

)
Example main() Workflow

Your main_aesa.cpp demonstrates:

Constructing RF settings → λ and k

Computing phase parameter 
𝑢
u

Computing array factor magnitude

Identifying nulls

Determining beamwidths (FNBW, HPBW)

Computing grating-lobe angles

Computing array gain (linear & dB)

Applying the radar range equation

Computing Doppler frequency and Doppler phase

Estimating angle of arrival and resolution

Each block prints values to the console for verification.

Compilation

To compile:

g++ -std=c++20 -O2 main_aesa.cpp -o aesa_sim


To run:

./aesa_sim

Future Extensions

2-D AESA (azimuth + elevation)

Amplitude tapering (Taylor, Chebyshev, Dolph-Chebyshev)

FFT-based digital beamforming

Range/Doppler processing chain

EW/ESM angle tracking (Mono-Pulse, MUSIC, ESPRIT)

Visualization with Python or C++ plotting

