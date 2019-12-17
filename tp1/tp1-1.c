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

void main() {
	mpz_t g, x, y, a, b;
	mpz_inits(g, x, y, a, b, NULL);
	printf("Calculadora de MDC estendido de Euclides.\n");
	printf("Digite um valor 'a': \n");
	gmp_scanf("%Zd", &a);
	printf("Digite outro valor 'b': \n");
	gmp_scanf("%Zd", &b);
	gmp_printf("\nOs numeros escolhidos sao: %Zd e %Zd \n", a, b);
	mdc_estendido(g, x, y, a, b);
	printf("\nA equacao que satisfaz ax + by = g e: \n");
	gmp_printf("\n%Zd(%Zd) + %Zd(%Zd) = %Zd\n", a, x, b, y, g);
	mpz_clears(g, x, y, a, b, NULL);
}
