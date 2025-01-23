/*
* learn.c / Practice Assignment / Program C
*
* Shorena K. Anzhilov / CS5600 / Northeastern University
* Spring 2025 / Jan  8, 2025 
*
*/

#include <stdio.h>
#include "mpg2km.h"

int main() {
    // Test cases with valid values
    double mpg1 = 30.0;
    double mpg2 = 15.0;
    double lph = 7.8;

    printf("Converting 30 MPG to KML: %.2f km/l\n", mpg2kml(mpg1));
    printf("Converting 30 MPG to LPHM: %.2f l/100km\n", mpg2lphm(mpg1));
    printf("Converting 7.8 LPH to MPG: %.2f mpg\n", lph2mpg(lph));

    printf("Converting 15 MPG to KML: %.2f km/l\n", mpg2kml(mpg2));
    printf("Converting 15 MPG to LPHM: %.2f l/100km\n", mpg2lphm(mpg2));

    // Test cases with "bad" values
    double bad_mpg = 0.0;
    double bad_lph = -5.0;

    printf("\nTesting invalid inputs:\n");
    printf("Converting 0 MPG to KML (invalid case): %.2f km/l\n", mpg2kml(bad_mpg));
    printf("Converting 0 MPG to LPHM (invalid case): %.2f l/100km\n", mpg2lphm(bad_mpg));
    printf("Converting -5 LPH to MPG (invalid case): %.2f mpg\n", lph2mpg(bad_lph));

    return 0;
}
