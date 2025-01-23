/*
* mpgkm.c / Practice Assignment / Program C
*
* Paws Husky / CS5600 / Northeastern University
* Student: Shorena K. Anzhilov
* Spring 2025 / Jan  8, 2025 
*
*/

#include "mpg2km.h"

// Converts miles per gallon (MPG) to kilometers per liter (KML).
double mpg2kml(double mpg) {
    return mpg * 0.425144;
}

// Converts miles per gallon (MPG) to liters per 100 kilometers (LPHM).
 double mpg2lphm(double mpg) {
    if (mpg <= 0) {
    // Handle division by zero or negative input
        return -1;  
    }
    return 235.214 / mpg;
}

// Converts liters per 100 kilometers (LPH) to miles per gallon (MPG).
double lph2mpg(double lph) {
    if (lph <= 0) {
    // Handle invalid negative or zero input
        return -1;  
    }
    return 235.214 / lph;
}
