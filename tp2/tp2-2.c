#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

void numero_aleatorio(mpz_t r, const mpz_t n, gmp_randstate_t rnd) {
	mpz_t n1;
	mpz_init(n1);
	mpz_set(n1, n);
	mpz_sub_ui(n1, n1, 1);
	mp_bitcnt_t num_bits = mpz_sizeinbase(n, 2);
	do {
		mpz_urandomb(r, rnd, num_bits);
	} while (!(mpz_cmp_ui(r, 2) >= 0 && mpz_cmp(r, n1) <= 0));
	mpz_clear(n1);
}

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

int talvez_primo(const mpz_t a, const mpz_t n, mpz_t n1, unsigned int t, mpz_t q) {
	mpz_t i, r, n1aux, two;
	mpz_inits(i, r, n1aux, two, NULL);
	mpz_set_ui(two, 2);
	mpz_sub_ui(n1, n,  1);
	mpz_set(n1aux, n1);
	t = 0;
	while(1) {
		mpz_fdiv_q_ui(q, n1aux, 2);
		mpz_tdiv_r_ui(r, q, 2);
		t = t + 1;
		if(mpz_cmp_ui(r, 0) != 0) {
			break;
		}
		else {
			mpz_set(n1aux, q);
		}
	}
	exp_binaria(r, a, q, n);
	mpz_set_ui(i, 0);
	while(1) {
		if(mpz_cmp_ui(i,0) == 0 && mpz_cmp_ui(r,1) == 0) {
			return 1;
		}
		if(mpz_cmp_ui(i, 0) >= 0 && mpz_cmp(r, n1) == 0) {
			return 1;
		}
		mpz_add_ui(i, i, 1);
		exp_binaria(r, r, two, n);
		if(mpz_cmp_ui(i, t) >= 0) {
			return 0;
		}
	}
	mpz_clears(i, r, n1aux, two, NULL);
}

int provavelmente_primo(const mpz_t n, unsigned int iter, gmp_randstate_t rnd) {
	mpz_t random, n1, q;
	mpz_inits(random, n1, q, NULL);
	unsigned int t;
	int i, primo;
	i = 0;
	for(i; i < iter; i++) {
		numero_aleatorio(random, n, rnd);
		primo = talvez_primo(random, n, n1, t, q);
		if(primo == 0) {
			break;
		}
	}
	if(i == (iter)) {
		return 1;
	}
	else {
		return 0;
	}
	mpz_clears(random, n1, q, NULL);
}

void main() {
	gmp_randstate_t rnd;
	gmp_randinit_default(rnd);
	gmp_randseed_ui(rnd, 95615498);
	mpz_t n;
	mpz_init(n);
	unsigned int iter;
	int primo;
	printf("Digite o numero a ser testado: ");
	gmp_scanf("%Zd", &n);
	printf("Digite a quantidade de iteracoes: (quanto maior, mais preciso) ");
	gmp_scanf("%u", &iter);
	primo = provavelmente_primo(n, iter, rnd);
	if(primo == 1) {
		gmp_printf("\n O numero e provavelmente primo. \n");
	}
	else if(primo == 0) {
		gmp_printf("\n O numero e definitivamente composto.\n");
	}
	mpz_clear(n);
}