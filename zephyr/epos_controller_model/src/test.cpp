/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#include "assdf.h"


void main(void)
{
	ExtU_assdf_T inp{0.0};

    assdfModelClass* subsys = new assdfModelClass();

    subsys->initialize();

    subsys->setExternalInputs(&inp);

    for(float i = 0; i < 10; i+=0.01)
    {
        if( i >= 1.0 && i < 2.0 )
        {
            inp.In1 = 1.0;
        }else{
            inp.In1 = 0.0;
        }

        subsys->setExternalInputs(&inp);
        subsys->step();

        // std::cout << inp.In1 << " " << (subsys->getExternalOutputs()).Out1 << "\n";
    }

    subsys->~assdfModelClass();

	return 0;
}
