#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

void exp_binaria(mpz_t r, const mpz_t b, const mpz_t e, const mpz_t n) {
	mpz_t a1, p, e1, resto, multi, sub;
	mpz_inits(a1, p, e1, resto, multi, sub, NULL);
	mpz_set(a1, b);
	mpz_set_ui(p, 1);
	mpz_set(e1, e);
	while(1) {
		mpz_tdiv_r_ui(resto, e1, 2);
		if(mpz_cmp_ui(e1, 0) == 0) {
			mpz_set(r, p);
			break;
		}
		else if(mpz_cmp_ui(resto, 0) != 0) {
			mpz_mul(multi, a1, p);
			mpz_tdiv_r(p, multi, n);
			mpz_sub_ui(sub, e1, 1);
			mpz_fdiv_q_ui(e1, sub, 2);
		}
		else if(mpz_cmp_ui(resto, 0) == 0) {
			mpz_fdiv_q_ui(e1, e1, 2);
		}
		mpz_mul(a1, a1, a1);
		mpz_tdiv_r(a1, a1, n);
	}
	mpz_clears(a1, p, e1, resto, multi, sub, NULL);		
}

void main() {
	mpz_t r, b, e, n;
	mpz_inits(r, b, e, n, NULL);
	printf("Digite o valor da base 'b': \n");
	gmp_scanf("%Zd", &b);
	printf("Digite o expoente 'e': \n");
	gmp_scanf("%Zd", &e);
	printf("Digite o mod 'n': \n");
	gmp_scanf("%Zd", &n);
	exp_binaria(r, b, e, n);
	gmp_printf("O valor de r vale: %Zd\n", r);
	mpz_clears(r, b, e, n, NULL);
}
