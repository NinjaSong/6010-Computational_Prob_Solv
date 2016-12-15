/*
 *  cellm.h
 *  Forest Fire Simulation Functions
 *
 *  This file includes functions:
 *  1. neighbor()
 *     To go through cell automata part.
 *  2. growthfire()
 *     Which includes the initial growth and the very first fire.
 *  3. growthfire_next()
 *     To do the time steps simulation and gives average P.
 *
 *  Created by Yunhe Song on 16/8/26.
 *  Copyright © 2016 Yunhe Song. All rights reserved.
 */

#ifndef cellm_h
#define cellm_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* Grid size & Probability that can be easily modified */

#define M 100       /* Grid size M */
#define N 100       /* Grid size N */
float growth_parameter = 0.5;    /* Growth cell with probability g */
float fire_probability = 0.05;    /* lightning strike probability f */


/*
 * neighbor()
 * cell-automaton simulation function
 * if an element is "3" (a tree that catches fire) but it's neighbor is a tree which is "1", this
 * neighbor will turn into "3". To ensure that every neighbor can be scanned, use a resursion, until there is no
 * more neighbor beside any on-fire tree (exist = 0)
 */
int (*neighbor( int Array[][N]))[N]
{
    int exist = 0;    /* If there still exist a tree that not lit up -- a boolean that gives an end to the recursion */
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            
            /* to change the neighbor tree( if it's the neighbor of a onfire tree  */
            if (Array[i][j]==3
                && Array[i+1][j] == 1
                && i<M)
            {
                Array[i+1][j] = 3;
                exist = 1;
            }
            if (Array[i][j]==3
                && Array[i-1][j] == 1
                && i>0)
            {
                Array[i-1][j] = 3;
                exist = 1;
            }
            if (Array[i][j]==3
                && Array[i][j+1] == 1
                && j<N)
            {
                Array[i][j+1] = 3;
                exist = 1;
            }
            if (Array[i][j]==3
                && Array[i][j-1] == 1
                && j>0)
            {
                Array[i][j-1] = 3;
                exist = 1;
            }
        }
    }
    if (exist == 0)
    {
        return Array;
    }
    return neighbor(Array);
}



/*  First pointer function: initial growth&fire  */

int  (*growthfire(int Ar[][N]))[N]
{
    int i = 0, j = 0;
    /*
     * the seed for random numbers is the time when we run this c file
     * as we assign :
     * srand((unsigned)time(NULL)) in the "growthfire()" function,
     * the result for the fisrt "growthfire()" and later one recalled is the same.
     */
     srand((unsigned)time(NULL));
    
    /* 
     * initialize the forest matrix by growth probability g
     * We have random numbers from 0-99(total:100) and if the number falls into the probability range,
     * let's say g=0.35 so the range is 0-34,  we will take the element as a growed tree.
     * And if the random number is not in that range, it still remains empty.
     */
    
    for (i = 0; i < M; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            int g = rand()%100;
            if(Ar[i][j] == 0 && g < 100*growth_parameter)
               Ar[i][j] = 1;
        }
    }

    /*  count tree numbers before fire  */
    {
        int n,tree_before_fire = 0,k=0;
        for(n=0;n<M;n++)
        {
            for(k=0;k<N;k++)
            {
                tree_before_fire = Ar[n][k] + tree_before_fire;
            }
        }
        printf("Initial tree-numbers P is:[%d]\n",tree_before_fire);
    }
 
    /** fire phase **/
    // The same as growth phase, every tree has probability f to catch the fire, let's mark those firing trees as "3".
    // And for those empty, remain "0"; for those trees which don't get on fire, remain "1".
    int l = 0, k = 0;
    for (l = 0; l < M; l++)
    {
        for (k = 0; k < N; k++)
        {
            int fire_probability_range = rand()%100;
            if(Ar[l][k] == 0)
            {Ar[l][k] = 0;}
            else if (Ar[l][k] == 1 && fire_probability_range >= 100*fire_probability)
            {Ar[l][k] = 1;}
            else if (Ar[l][k] == 1 && fire_probability_range < 100*fire_probability)
            {Ar[l][k] = 3;}
        }
    }

    /** cell-automaton simulation phase **/
    /* call neighbor function */
    
    neighbor(Ar);
    
    /* translate the maker "3" back to empty element "0" */
    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            if (Ar[m][n] ==3)
                Ar[m][n] =0;
        }
    }

    /*  count trees survived from the fire  **/
    {
        int n,treenumber_after_1st_fire = 0,k=0;
        for(n=0;n<M;n++)
        {
            for(k=0;k<N;k++)
            {
                treenumber_after_1st_fire=Ar[n][k] + treenumber_after_1st_fire;
            }
        }
        printf("Tree-number P after the first fire is:[%d]\n",treenumber_after_1st_fire);
    }
    
    return Ar;
}


//
//****************************************************//
//
/*
 ** Second pointer function: next-step growth&fire *
 **/

int  (*growthfire_next(int Array[][N],int t,int total_simu_times,int avg))[N]
{
    if (t == 0)    /*  when t=0, it's the end of this simulation */
        return Array;
    
    int i = 0, j = 0;
   
    /* modify the forest matrix by growth probability g */
    // We have random numbers from 0-99(total:100) and if the number falls into the probability range,
    // let's say g=0.35 so the range is 0-34,we will take the element as a growed tree.
    // And if the random number is not in that range, it still remains empty.
    // And if there exists a tree already, let it be.
    for (i = 0; i < M; ++i)
    {
        for (j = 0; j < N; ++j)
        {
            int g = rand()%100;
            if(Array[i][j] == 0 && g < 100* growth_parameter)
            {   Array[i][j] = 1;
            }
            else if (Array[i][j] == 1)
            {   Array[i][j] = 1;
            }
        }
    }

    /*  count tree numbers before fire  */
    {
        int n,tree_before_fire = 0,k=0;
        
        for(n=0;n<M;n++)
        {
            for(k=0;k<N;k++)
            {
                tree_before_fire = Array[n][k] + tree_before_fire;
                
            }
        }
        printf("After growth phase, the Trees-number P is:[%d]\n",tree_before_fire);
    }
    
    /* 
     * fire phase
     *
     * The same as growth phase, every tree has probability f to catch the fire, let's mark those firing trees as "3".
     * And for those empty, remain "0"; for those trees which don't get on fire, remain "1".
     */
    int l = 0, k = 0;
    for (l = 0; l < M; ++l)
    {
        for (k = 0; k < N; ++k)
        {
            int f = rand()%100;
            if(Array[l][k] == 0)
            {Array[l][k] = 0;}
            else if (Array[l][k] == 1 && f >= 100*fire_probability)
            {Array[l][k] = 1;}
            else if (Array[l][k] == 1 && f < 100*fire_probability)
            {Array[l][k] = 3;}
        }
    }
    
    /** cell-automaton simulation phase **/
    /*  call neighbor function */
    
    neighbor(Array);
    
    /* translate the maker "3" back to empty element "0" */
    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            if (Array[m][n] ==3)
                Array[m][n] =0;
        }
    }
    
    /*  count trees survived from the fire  */
    {
        int n,tree_after_this_fire = 0,k=0;
        for(n=0;n<M;n++)
        {
            for(k=0;k<N;k++)
            {
                tree_after_this_fire=Array[n][k] + tree_after_this_fire;
            }
        }
        printf("After the fire, the Trees-number P is:[%d]\n",tree_after_this_fire);
        avg = tree_after_this_fire + avg;
        int  averange_for_time_steps = avg/(total_simu_times-t+1);
        printf("After the fire, the avg Trees-number P is:[%d]\n",averange_for_time_steps);
        system("clear");
        sleep(1);
        growthfire_next(Array,t-1,total_simu_times,avg);
    }
    return Array;
}




#endif /* cellm_h */
