#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>

void mdc_estendido(mpz_t g, mpz_t x, mpz_t y, const mpz_t a, const mpz_t b) {
	mpz_t r, q, x1, y1, multi, sub;
	mpz_inits(r, q, multi, sub, x1, y1, NULL);
	mpz_tdiv_r(r, a, b);
	mpz_fdiv_q(q, a, b);
	if(mpz_cmp_ui(r, 0) == 0) {
		mpz_set(g, b);
		mpz_set_ui(x, 0);
		mpz_set_ui(y, 1);
	}
	else {
		mdc_estendido(g, x, y, b, r);
		mpz_set(x1, x);
		mpz_set(y1, y);
		mpz_set(x, y);
		mpz_mul(multi, q, y1);
		mpz_sub(sub, x1, multi);
		mpz_set(y, sub);
	}
	mpz_clears(r, q, multi, sub, x1, y1, NULL);
}

int inverso_modular(mpz_t r, const mpz_t a, const mpz_t n) {
	mpz_t g, x, y;
	mpz_inits(g, x, y, NULL);
	mdc_estendido(g, x, y, a, n);
	if(mpz_cmp_ui(g, 1) == 0) {
		mpz_set(r, x);
		return 1;
	}
	else {
		return 0;
	}
	mpz_clears(g, x, y, NULL);
}

void main() {
	int inverso;
	mpz_t r, a, n;
	mpz_inits(r, a, n, NULL);
	printf("Digite um valor 'a': \n");
	gmp_scanf("%Zd", &a);
	printf("Digite o mod 'n': \n");
	gmp_scanf("%Zd", &n);
	inverso = inverso_modular(r, a, n);
	gmp_printf("\nOs numeros escolhidos sao: %Zd e %Zd\n", a, n);
	if(inverso == 1) {
		gmp_printf("\n O inverso existe e vale: %Zd \n", r);
	}
	else if(inverso == 0) {
		gmp_printf("\n O inverso não existe.\n");
	}
	mpz_clears(r, a, n, NULL);
}
