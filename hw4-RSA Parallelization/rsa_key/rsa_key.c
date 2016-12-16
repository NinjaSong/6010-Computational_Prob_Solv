//
//  main.c
//  rsa
//
//  Created by Yunhe Song on 16/11/15.
//  Copyright © 2016 Yunhe Song. All rights reserved.
//

//********************************************************************************************//
//********参考： http://blog.csdn.net/qmickecs/article/details/39676655 ************************
//********************************************************************************************//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include "rsa_key.h"


RSAKey *generate_keypair (int siz) {

    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time(NULL));   //like in std C : grt = srand(time(NULL));
    
    RSAKey *p =(RSAKey *) malloc(sizeof(RSAKey));
    p->siz = siz;
    mpz_init(p->p);
    mpz_init(p->q);    //initialize type mpz_t [一个mpz_t类型的变量必须在初始化后才能被使用]
    
    mpz_urandomb(p->p, grt, siz);
    mpz_urandomb(p->q, grt, siz); //random number grt in 0~2^sz [随机生成一个在0~2^sz-1之间的随机数]
    
    if(mpz_even_p(p->p))
        mpz_add_ui(p->p, p->p, 1);
    if(mpz_even_p(p->q))
        mpz_add_ui(p->q, p->q, 1);    //if the random number is an even number , add 1 [如果生成的随机数为偶数，则加一]
    
    while((!mpz_probab_prime_p(p->p, 25)) > 0)  //iteration to check is it a prime number [逐个检查比p大的奇数是否为素数]
        mpz_add_ui(p->p, p->p, 2);
    while((!mpz_probab_prime_p(p->q, 25)) > 0)
        mpz_add_ui(p->q, p->q, 2);
    
    // gmp_printf("%Zd\n", p->p);   //以十六进制的形式输出生成的素数
    // gmp_printf("%Zd\n", p->q);


    mpz_init(p->n);
    
    mpz_mul(p->n, p->p, p->q); //culculate p * q，save it into key_n [计算p*q，并将结果储存在key_n中]
    mpz_init(p->f);
    
    mpz_sub_ui(p->p, p->p, 1);    //p=p-1
    mpz_sub_ui(p->q, p->q, 1);    //q=q-1
    mpz_mul(p->f, p->p, p->q);   //culculate (p - 1) * (q - 1)，save it into key_f [计算(p - 1) * (q - 1)，并将结果储存在key_f中]
    
    mpz_init_set_ui(p->e, 65537);//initialize e, set it 65537 [初始化并设置e为65537]
    
    // gmp_printf("%s (%ZX, %ZX)\n", "public key is:", p->n, p->e); //output public key(n,e) 输出公钥(n, e)


    mpz_init(p->d);
    
    mpz_invert(p->d, p->e, p->f); //get key-d by invert e 求e的数论倒数d

    mpz_add_ui(p->p, p->p, 1);    //p=p-1
    mpz_add_ui(p->q, p->q, 1);    //q=q-1
    
    // gmp_printf("%s (%ZX, %ZX)\n", "private key is:", p->n, p->d);//output private key(n,d) 输出私钥(n, d)
    
    return p;

}


void printRSAKey(RSAKey *rsaKey) {

	if (rsaKey == NULL) {
		printf("NULL\n");
		return;
	}

	printf("siz = %d\n", rsaKey->siz);
	gmp_printf("p = %Zd, q = %Zd, f = %Zd\n", rsaKey->p, rsaKey->q, rsaKey->f);
	gmp_printf("n = %Zd, e = %Zd, d = %Zd\n", rsaKey->n, rsaKey->e, rsaKey->d);

}


int main(int argc, char **argv)
{

	if (argc < 1) {
		printf("The program takes in 1 argument: siz\n");
		return -1;
	}

    int siz = atoi(argv[1]);
    
    RSAKey *rsaKey = generate_keypair(siz);

    printRSAKey(rsaKey);

}  
