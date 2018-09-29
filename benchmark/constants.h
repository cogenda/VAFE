// Mathematical and physical constants

`ifdef CONSTANTS_H
`else
`define CONSTANTS_H


`define M_E         2.71828182845904523536
`define M_LOG2E     1.44269504088896340736
`define M_LOG10E    0.43429448100325182765
`define M_LN2       0.693147180559945309417
`define M_LN10      2.30258509299404568402
`define M_PI        3.14159265358979323846
`define M_TWO_PI    6.28318530717958647652
`define M_PI_2      1.57079632679489661923
`define M_PI_4      0.785398163397448309616
`define M_1_PI      0.318309886183790671538
`define M_2_PI      0.636619772367581343076
`define M_2_SQRTPI  1.12837916709551257390
`define M_SQRT2     1.41421356237309504880
`define M_SQRT1_2   0.707106781186547524401

// Physical constants
// Notes on physical values
//
// Most analog simulators including SIMetrix
// use values for K and q that were the best known
// at the time when SPICE 2 was developed. These
// values have since been updated. Below are both
// sets of values. To use the new values, set the
// preprocessor variable SX_USE_NEW_PHYSICAL_CONSTANTS

`ifdef SX_USE_NEW_PHYSICAL_CONSTANTS

// Modern values for physical constants

`define P_Q         1.602176462E-19
`define P_K         1.3806503E-23
`define P_C         2.99792458E8
`define P_H         6.62606876E-34
`define P_EPS0      8.85418781762038985E-12 
`define P_U0        1.25663706143591729E-6 
`define P_CELSIUS0  273.15



`else

// SPICE values for physical constants

`define P_Q         1.6021918E-19
`define P_K         1.3806226E-23
`define P_C         2.99792458E8
`define P_H         6.626069311E-34
`define P_EPS0      8.85418781762038985E-12 
`define P_U0        1.25663706143591729E-6 
`define P_CELSIUS0  273.15

`endif


`endif

