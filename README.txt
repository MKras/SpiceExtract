How to install

This program depends on the next libraries:
LAPACK and F2C - (search in your repository)
GSL - http://www.gnu.org/software/gsl/
OPT++ - https://software.sandia.gov/opt++/
GALib - http://lancet.mit.edu/ga/
LevMar - http://www.ics.forth.gr/~lourakis/levmar/


- install LAPACK and F2C for your dirstributive;
- download and install GSL (you will need libgslcblas.so, libgsl.so);

# Thes optimization libraries is optional now. Default is disabled.
- download and install OPT (you will need libnewton.so, libopt.so, libnewmat.so) (To enable type with CMake -DWITH_OPTPP=ON);
- download and install GALib (you will need llibga.so) (To enable type with CMake -DWITH_GALIB=ON);
- download and install LevMar (you will need liblevmar.a)(To enable type with CMake -DWITH_LEVMAR=ON);

… and also you will need libf2c.a (in /usr/lib/libf2c.a) library

Building with presetted external libraries or only with internal optimization methods is default.

The main unresolved problem is obtaining of CV characteristics for BSIM (and other libraries).
