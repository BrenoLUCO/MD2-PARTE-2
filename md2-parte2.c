#include <stdio.h>
#include <stdlib.h>

// Função para verificar se um número é primo
int eh_primo(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) {
            return 0;
        }
    }
    return 1;
}

// Função para calcular o MDC usando o algoritmo de Euclides
int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Função para verificar coprimalidade
int sao_coprimos(int a, int b) {
    return mdc(a, b) == 1;
}

// Função para calcular o inverso modular usando o algoritmo estendido de Euclides
int inverso_modular(int a, int n) {
    int t = 0, novo_t = 1;
    int r = n, novo_r = a;
    
    while (novo_r != 0) {
        int quociente = r / novo_r;
        
        int temp_t = t;
        t = novo_t;
        novo_t = temp_t - quociente * novo_t;
        
        int temp_r = r;
        r = novo_r;
        novo_r = temp_r - quociente * novo_r;
    }
    
    if (r > 1) return -1; // Não existe inverso
    if (t < 0) t += n;
    
    return t;
}

// Função para calcular a função totiente de Euler (n)
int totient_euler(int n) {
    if (n == 1) return 1;
    
    int resultado = n;
    
    // Verifica divisibilidade por 2
    if (n % 2 == 0) {
        resultado -= resultado / 2;
        while (n % 2 == 0) n /= 2;
    }
    
    // Verifica divisibilidade por números ímpares
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            resultado -= resultado / i;
            while (n % i == 0) n /= i;
        }
    }
    
    // Se n for um número primo maior que 2
    if (n > 1) resultado -= resultado / n;
    
    return resultado;
}

// Função para exponenciação modular eficiente
int exp_modular(int base, int expoente, int modulo) {
    if (modulo == 1) return 0;
    
    int resultado = 1;
    base = base % modulo;
    
    while (expoente > 0) {
        if (expoente % 2 == 1) {
            resultado = (resultado * base) % modulo;
        }
        expoente = expoente >> 1;
        base = (base * base) % modulo;
    }
    
    return resultado;
}

int main() {
    int H, G, n, x, n1;
    
    printf("=== Sistema de Verificação Modular ===\n");
    printf("Digite os valores de entrada:\n");
    printf("H: "); scanf("%d", &H);
    printf("G: "); scanf("%d", &G);
    printf("n: "); scanf("%d", &n);
    printf("Expoente x: "); scanf("%d", &x);
    printf("Modulo n1: "); scanf("%d", &n1);
    
    printf("\n=== PASSO 1: Verificar se G e n sao coprimos ===\n");
    if (mdc(G, n) != 1) {
        printf("G (%d) e n (%d) nao sao coprimos (MDC = %d).\n", G, n, mdc(G, n));
        printf("A divisao modular nao e possivel.\n");
        return 0;
    } else {
        printf("G (%d) e n (%d) sao coprimos (MDC = 1).\n", G, n);
    }
    
    printf("\n=== PASSO 2: Calcular o inverso de G em Zn ===\n");
    int inv_G = inverso_modular(G, n);
    if (inv_G == -1) {
        printf("Nao existe inverso modular para G (%d) em Z%d.\n", G, n);
        return 0;
    } else {
        printf("O inverso de G (%d) em Z%d e: %d\n", G, n, inv_G);
    }
    
    printf("\n=== PASSO 3: Calcular a base a = H * G^-1 mod n ===\n");
    int a = (H * inv_G) % n;
    printf("a = (H * G^-1) mod n = (%d * %d) mod %d = %d\n", H, inv_G, n, a);
    
    printf("\n=== PASSO 4: Verificar se a e n1 sao coprimos ===\n");
    if (sao_coprimos(a, n1)) {
        printf("a (%d) e n1 (%d) sao coprimos (MDC = 1).\n", a, n1);
    } else {
        printf("a (%d) e n1 (%d) nao sao coprimos (MDC = %d).\n", a, n1, mdc(a, n1));
    }
    
    printf("\n=== PASSO 5: Verificar se n1 e primo ===\n");
    int n1_primo = eh_primo(n1);
    if (n1_primo) {
        printf("n1 (%d) e primo.\n", n1);
    } else {
        printf("n1 (%d) nao e primo.\n", n1);
    }
    
    printf("\n=== PASSO 6/7: Ajustar o expoente ===\n");
    int novo_x;
    if (n1_primo) {
        novo_x = x % (n1 - 1);
        printf("Como n1 e primo, aplicamos o Pequeno Teorema de Fermat.\n");
        printf("x ajustado = x mod (n1-1) = %d mod %d = %d\n", x, n1 - 1, novo_x);
    } else {
        int phi = totient_euler(n1);
        novo_x = x % phi;
        printf("Como n1 nao e primo, aplicamos o Teorema de Euler.\n");
        printf("φ(n1) = %d\n", phi);
        printf("x ajustado = x mod (n1) = %d mod %d = %d\n", x, phi, novo_x);
    }
    x = novo_x;
    
    printf("\n=== PASSO 8: Decompor o expoente x ===\n");
    int q, r;
    if (n1_primo) {
        q = n1 - 1;
    } else {
        q = totient_euler(n1);
    }
    r = x % q;
    int x1 = x / q;
    printf("x = x1 * q + r = %d * %d + %d\n", x1, q, r);
    
    printf("\n=== PASSO 9/10: Calcular os valores intermediarios ===\n");
    int a_x1 = exp_modular(a, x1, n1);
    printf("a^x1 mod n1 = %d^%d mod %d = %d\n", a, x1, n1, a_x1);
    
    int a_x1_q = exp_modular(a_x1, q, n1);
    printf("(a^x1)^q mod n1 = %d^%d mod %d = %d\n", a_x1, q, n1, a_x1_q);
    
    int a_r = exp_modular(a, r, n1);
    printf("a^r mod n1 = %d^%d mod %d = %d\n", a, r, n1, a_r);
    
    printf("\n=== PASSO 11: Combinar os resultados ===\n");
    int resultado = (a_x1_q * a_r) % n1;
    printf("Resultado final: ((a^x1)^q * a^r) mod n1 = (%d * %d) mod %d = %d\n", 
           a_x1_q, a_r, n1, resultado);
    
    printf("\n=== RESULTADO FINAL ===\n");
    printf("%d^%d mod %d ≡ %d\n", a, x, n1, resultado);
    
    return 0;
}