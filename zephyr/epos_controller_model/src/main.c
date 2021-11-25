#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#include "assdf.h"


void main()
{
    assdf_initialize();

    for(float i = 0; i < 10; i+=0.01)
    {
        if( i >= 1.0 && i < 2.0 )
        {
            assdf_U.In1 = 1.0;
        }else{
            assdf_U.In1 = 0.0;
        }

        assdf_step();

        printf("X: %.2f Y: %.2f\n", assdf_U.In1, assdf_Y.Out1);
    }

	return 0;
}
