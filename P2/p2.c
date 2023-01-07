//CARLOS MARTINEZ RABUNAL carlos.martinez4
//ANTON LOPEZ NUNEZ anton.lopez.nunez

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define confianza 500
#define kte 1000

#define n_test 10
#define n_max 32000

#define UMBRAL 1

/* EJERCICIO 1 */

void ord_ins (int v[], int n){
    int x, j;
    for(int i = 1; i < n ; i++){
        x = v[i];
        j = i-1;
        while(j>=0 && v[j]>x){
            v[j+1] = v[j];
            j = j-1;
        }
        v[j+1] = x;
    }
}

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v [], int n) {/* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void descendente(int v[], int n){
    int i;
    for(i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void copiarVector(int original[], int n, int copia[]){
    for(int i=0; i<n; i++){
        copia[i] = original[i];
    }
}

void tablaInsercion(int v[], int n){
    double ta, tb, t1, t2, t;
    double f, g, h;
    int vcopia[n];
    copiarVector(v, n, vcopia);
    ta = microsegundos();
    ord_ins(v,n);
    tb = microsegundos();
    t = tb-ta;

    if (t<confianza) {
        ta = microsegundos();
        for (int i = 0; i < kte; ++i) {
            copiarVector(vcopia, n, v);
            ord_ins(v, n);
        }
        tb = microsegundos();
        t1 = tb-ta;
        ta = microsegundos();
        for(int i=0; i<kte; ++i){
            copiarVector(vcopia, n, v);
        }
        tb= microsegundos();
        t2=tb-ta;
        t=(t1-t2)/kte;
    }
    f = pow(n, 1.8);
    g = (n*n);
    h = pow(n, 2.2);

    //f = 1;
    //g = 1;
    //h = 1;

    printf("%10d%15.3f%15.8f%15.10f%15.11f\n", n, t, t/f , t/g , t/h);
}

void testInsercion(int v[], int n){
    aleatorio(v, n_test);

    printf("%-60s","Ordenacion por insercion con inicializacion aleatoria:");
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("");

    ord_ins(v,n);

    printf("%-60s","Numeros ordenados:");
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("");

    printf("%-60s","Numeros en descendente:");
    descendente(v,n);
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("");
    printf("%-60s","Ordenacion por insercion con inicializacion descendente:");
    ord_ins(v,n);
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("\n");
}

void intercambiar(int *n1, int *n2){
    int aux = *n1;
    *n1 = *n2;
    *n2 = aux;
}

void ordenarAux(int v[], int izq, int der){
    int pivote, i, j, x;
    if(izq + UMBRAL <= der){
        x = (rand() % ( der - izq + 1 ) + izq);
        pivote = v[x];
        intercambiar(&v[izq], &v[x]);
        i = izq +1;
        j = der;
        while(i <= j){
            while(i <= der && v[i] < pivote){
                i = i + 1;
            }
            while(v[j] > pivote){
                j = j-1;
            }
            if( i <= j) {
                intercambiar(&v[i], &v[j]);
                i = i+1;
                j = j-1;
            }
        }
        intercambiar(&v[izq], &v[j]);
        ordenarAux(v, izq, j-1);
        ordenarAux(v, j-1, der);
    }
}

void ordenacionRapida(int v[], int n){
    ordenarAux(v,0,n-1);
    if(UMBRAL > 1){
        ord_ins(v,n);
    }
}

void test_OrdRapida(int v[], int n){
    aleatorio(v, n);

    printf("%-60s","Inicializacion aleatoria:");
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("");

    ordenacionRapida(v,n);

    printf("%-60s","Ordenacion por insercion con inicializacion aleatoria:");
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("");

    printf("%-60s","Numeros en descendente:");
    descendente(v,n);
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("");
    printf("%-60s","Ordenacion por insercion con inicializacion descendente:");
    ordenacionRapida(v,n);
    for(int i=0; i<n; i++){
        printf("%3d ", v[i]);
    }
    puts("\n");

}

void auxOrdenacionRapida(int v[], int n) {
    double ta, tb, t1, t2, t;
    double f, g, h;
    int vcopia[n];
    copiarVector(v, n, vcopia);
    ta = microsegundos();
    ord_ins(v,n);
    tb = microsegundos();
    t = tb-ta;

    if (t<confianza) {
        ta = microsegundos();
        for (int i = 0; i < kte; ++i) {
            copiarVector(vcopia, n, v);
            ordenacionRapida(v, n);
        }
        tb = microsegundos();
        t1 = tb-ta;
        ta = microsegundos();
        for(int i=0; i<kte; ++i){
            copiarVector(vcopia, n, v);
        }
        tb= microsegundos();
        t2=tb-ta;
        t=(t1-t2)/kte;
    }
    f = pow(n, 1.8);
    g = (n*log(n));
    h = pow(n, 2.2);
    //f = 1;
    //g = 1;
    //h = 1;

    printf("%10d%15.3f%15.8f%15.10f%15.11f\n", n, t, t/f , t/g , t/h) ; // NO VA
}

void tablaOrdenacionRapida(int v[], int n){
    int i, t[n_test];

    puts("Ordenacion Rapida, con inicializacion ascendente:");
    printf("%10s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/n^(1.8)", "t(n)/n^(2)", "t(n)/n^(2.2)" );
    for(i=500; i <= n_max; i = 2*i){
        aleatorio(v,i);
        ascendente(v,i);
        auxOrdenacionRapida(v,i);
    }
    puts("Ordenacion Rapida, con inicializacion descendente:");
    printf("%10s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/n^(1.8)", "t(n)/n^(2)", "t(n)/n^(2.2)" );
    for(i=500; i <= n_max; i = 2*i){
        aleatorio(v,i);
        descendente(v,i);
        auxOrdenacionRapida(v,i);
    }
    puts("Ordenacion Rapida, con inicializacion aleatoria:");
    printf("%10s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/n^(1.8)", "t(n)/n^(2)", "t(n)/n^(2.2)" );
    for(i=500; i <= n_max; i = 2*i){
        aleatorio(v,i);
        auxOrdenacionRapida(v,i);
    }
}

int main() {
    inicializar_semilla();
    int i;
    int v[n_max];
    int t[n_test];

    puts("Test Ordenacion por Insercion:\n");
    testInsercion(t, n_test);
    puts("Ordenacion por insercion, con inicializacion ascendente:");
    printf("%10s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/n^(1.8)", "t(n)/n^(2)", "t(n)/n^(2.2)" );
    for(i=500; i <= n_max; i = 2*i){
        aleatorio(v,i);
        ascendente(v,i);
        tablaInsercion(v,i);
    }
    puts("Ordenacion por insercion, con inicializacion descendente:");
    printf("%10s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/n^(1.8)", "t(n)/n^(2)", "t(n)/n^(2.2)" );
    for(i=500; i <= n_max; i = 2*i){
        aleatorio(v,i);
        descendente(v,i);
        tablaInsercion(v,i);
    }
    puts("Ordenacion por insercion, con inicializacion aleatoria:");
    printf("%10s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/n^(1.8)", "t(n)/n^(2)", "t(n)/n^(2.2)" );
    for(i=500; i <= n_max; i = 2*i){
        aleatorio(v,i);
        tablaInsercion(v,i);
    }

    puts("Test Ordenacion Rapida:\n");
    test_OrdRapida(t,n_test);
    tablaOrdenacionRapida(v, i);

    return 0;
}
