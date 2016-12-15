/*
 *  main.c
 *  Forest Fire Simulation
 *  
 *  This file includes the main program, simulate several time steps of growth&fire phase.
 *
 *  Created by Yunhe Song on 16/8/26.
 *  Copyright © 2016 Yunhe Song. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *cellm.h is functions which are related to this cell automaton main progress.
 *cellm.h includes 2 functions: the first one is to initialize the forest matrix, and the second one is to mimic the next step
 *(when there exist some trees and then again we apply growth&fire).
 */
#include "cellm.h"

/* 
 * To get plenty of experiments data I apply severral time steps,
 * But actaully you can just use one
 * It will simulate every time step.
 */

int main(void){

    printf("- growth probability g = %f .\n- fire probability f = %f .\n",growth_parameter,fire_probability);
    int Arr[M][N] = {{0}};
    int avg = 0;
    printf("［Time step = 1 ］\n");
    growthfire(Arr);
    
    printf("［Time step = 2 ］\n");
    growthfire_next(growthfire(Arr),2,2,avg);
    
    printf("［Time step = 3 ］\n");
    growthfire_next(growthfire(Arr),3,3,avg);
    
    printf("［Time step = 4 ］\n");
    growthfire_next(growthfire(Arr),4,4,avg);
    
    printf("［Time step = 5 ］\n");
    growthfire_next(growthfire(Arr),5,5,avg);
    
    printf("［Time step = 10 ］\n");
    growthfire_next(growthfire(Arr),10,10,avg);
    
    printf("［Time step = 30 ］\n");
    growthfire_next(growthfire(Arr),30,30,avg);
    
    printf("［Time step = 50 ］\n");
    growthfire_next(growthfire(Arr),50,50,avg);
    
    printf("［Time step = 100 ］\n");
    growthfire_next(growthfire(Arr),100,100,avg);
    
    printf("［Time step = 300 ］\n");
    growthfire_next(growthfire(Arr),300,300,avg);
    
    printf("［Time step = 500 ］\n");
    growthfire_next(growthfire(Arr),500,500,avg);

    return 0;
}
