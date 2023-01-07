#include <math.h>
#include <stdio.h>
#include <sys/time.h>

#define kte 1000
#define confianza 500

/* EJERCICIO 1*/

int fib1(int n){
    if(n<2)
        return n;
    else return(fib1(n-1) + fib1(n-2));
}

int fib2(int n){
    int i=1, j=0, k=0;
    for(k=0; k<n; k++){
        j=i+j;
        i=j-i;
    }
    return j;
}

int fib3(int n){
    int i=1; int j=0; int k=0; int h=1; int t=0;

    while(n>0){
        if(n % 2 != 0){
            t = j*h;
            j = i*h + j*k + t;
            i = i*k +t;
        }
        t = h*h;
        h = 2*k*h + t;
        k = k*k + t;
        n = n/2;
    }
    return j;
}

/* EJERCICIO 2*/

void test(int k){
    int n = 1;
    puts("n\tfib1(n)\tfib2(n)\tfib3(n)");
    for(n=1; n<=k; n++){
        printf("%d\t%4d\t%4d\t%4d\n",n ,fib1(n) , fib2(n), fib3(n));
    }
}

/*EJERCICIO 3*/


double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void tablafib1(int n) {
    int i = 0;
    double ta, tb, t, phi = (1 + sqrt(5))/2;
    ta = microsegundos();
    fib1(n);
    tb = microsegundos();
    t = tb-ta;
    if (t<confianza) {
        ta = microsegundos();
        for (i = 0; i < kte; ++i) {
            fib1(n);
        }
        tb = microsegundos();
        t = (tb-ta)/kte;
    }
    printf("%-10d %-15f %-15f %-15f %-15f\n", n, t, t/pow(1.1,n), t/pow(phi,n), t/pow(2,n));
}

void tablafib2(int n) {
    int i = 0;
    double ta, tb, t;
    ta = microsegundos();
    fib2(n);
    tb = microsegundos();
    t = tb-ta;
    if (t<confianza) {
        ta = microsegundos();
        for (i = 0; i < kte; ++i) {
            fib2(n);
        }
        tb = microsegundos();
        t = (tb-ta)/kte;
    }
    printf("%-10d %-15f %-15f %-15f %-15f\n", n, t, t/pow(n,0.8), t/n, t/(n*log10(n)));
}

void tablafib3(int n) {
    int i = 0;
    double ta, tb, t;
    ta = microsegundos();
    fib3(n);
    tb = microsegundos();
    t = tb-ta;
    if (t<confianza) {
        ta = microsegundos();
        for (i = 0; i < kte; ++i) {
            fib3(n);
        }
        tb = microsegundos();
        t = (tb-ta)/kte;
    }
    printf("%-10d %-15f %-15f %-15f %-15f\n", n, t, t/sqrt(log10(n)), t/log10(n), t/pow(n,0.5));
}

int main() {
    int i = 0;
    test(10); // Verifica si los algoritmos funcionan correctamente

    printf("Algoritmo fib1:\n");
    printf("%-10s %-15s %-15s %-15s %-15s \n", "n", "t(n)", "t(n)/1.1^n", "t(n)/1.62^n", "t(n)/2^n" );
    for (i = 2; i <= 32; i = 2*i) {
        tablafib1(i);
    }

    printf("Algoritmo fib2:\n");
    printf("%-10s %-15s %-15s %-15s %-15s \n", "n", "t(n)", "t/(n^0.8)","t/n","t/(n*log(n)");
    for (i = 1000; i <= 10000000; i = 10*i) {
        tablafib2(i);
    }

    printf("Algoritmo fib3:\n");
    printf("%-10s %-15s %-15s %-15s %-15s \n", "n", "t(n)", "t/sqrt(log(n))", "t/log(n)", "t/(n^0.5)");
    for (i = 1000; i <= 10000000; i = 10*i) {
        tablafib3(i);
    }

    return 0;
}