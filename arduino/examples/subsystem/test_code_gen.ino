#include "Arduino.h"


void setup()
{
    Serial.begin(115200);
}

double f(double z)
{
    double z;
}
double g(double z, double y, double phi_l,double M_M )
{
    return ( M_M/0.0770 - 0.7682*z - 4.7009*y - 4.7009*phi_l  );
}
double step(double h, double z, double y, double phi_l, double M_M)
{
    double k_0 = h*f(z);
    double l_0 = h*g(z,y,phi_l,M_M);

    double k_1 = h*f(z+0.5*l_0);
    double l_1 = h*g(z+0.5*l_0,y+0.5*k_0,phi_l,M_M);

    double k_2 = h*f(z+0.5*l_1);
    double l_2 = h*g(z+0.5*l_1,y+0.5*k_1,phi_l,M_M);

    double k_3 = h*f(z+l_2);
    double l_3 = h*g(z+l_2,y+k_2,phi_l,M_M);

    return ( y + (k_0 + 2*k_1 + 2*k_2 + k_3)/6 );
}

void loop()
{
    double M_M = 
    Subsystem_initialize();
    printf("coderRand=%g\n", Subsystem());
    Subsystem_terminate();
}
