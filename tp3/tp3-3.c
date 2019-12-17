#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
	for(i; i<iter; i++) {
		numero_aleatorio(random, n, rnd);
		primo = talvez_primo(random, n, n1, t, q);
		if(primo == 0) {
			break;
		}
	}
	if(i == iter) {
		return 1;
	}
	else {
		return 0;
	}
	mpz_clears(random, n1, q, NULL);
}

void primo_aleatorio(mpz_t r, unsigned int b, gmp_randstate_t rnd) {
	int primo;
	while(1) {
		mpz_urandomb(r, rnd, b);
		primo = provavelmente_primo(r, 20, rnd);
		if(primo == 1) {
			gmp_printf("Primo gerado: %Zd \n", r);
			break;
		}
	}
}

void gera_chaves(mpz_t n, mpz_t e, mpz_t d, gmp_randstate_t rnd) {
	mpz_t p, q, p1, q1, fiden;
	mpz_inits(p, q, p1, q1, fiden, NULL);
	int inverso;
	mpz_set_ui(e, 65537);
	primo_aleatorio(p, 2048, rnd);
	primo_aleatorio(q, 2048, rnd);
	gmp_printf("Os primos são %Zd e %Zd\n", p, q);
	mpz_mul(n, p, q);
	gmp_printf("O n e: %Zd\n", n);
	mpz_sub_ui(p1, p, 1);
	mpz_sub_ui(q1, q, 1);
	mpz_mul(fiden, p1, q1);
	while(1) {
		inverso = inverso_modular(d, e, fiden);
		if(inverso == 1) {
			break;
		}
		mpz_add_ui(e, e, 2);
	}
	gmp_printf("O e e: %Zd\n", e);
	gmp_printf("O d e: %Zd\n", d);
	mpz_clears(p, q, p1, q1, fiden, NULL);
}

char *decodifica(const mpz_t n) {
	char *str;
	str = (char*) malloc((500)*sizeof(char));
	mpz_t resto, aux;
	mpz_inits(resto, aux, NULL);
	int i;
	i = 0;
	mpz_set_ui(resto, 1);
	mpz_set(aux, n);
	while(1) {
		if(mpz_cmp_ui(resto, 0) == 0) {
			break;
		}
		mpz_tdiv_r_ui(resto, aux, 256);
		str[i] = mpz_get_ui(resto);
		i++;
		mpz_sub(aux, aux, resto);
		mpz_fdiv_q_ui(aux, aux, 256);
	}
	mpz_clears(resto, aux, NULL);
	return str;
}

void main() {
	gmp_randstate_t rnd;
	gmp_randinit_default(rnd);
	gmp_randseed_ui(rnd, 95615498);
	mpz_t r;
	mpz_init(r);
	char str[500];
	char *strd;
	printf("Digite a mensagem codificada: ");
	gmp_scanf("%Zd", &r);
	strd = decodifica(r);
	printf("A mensagem e: %s \n", strd);
	mpz_clear(r);
}