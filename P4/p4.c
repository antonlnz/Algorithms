/* Anton Lopez Nunez y Carlos Martinez Rabunal */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define confianza 500
#define kte 1000

#define TAM 512000
#define VTEST 7

typedef struct {
    int vector[TAM];
    int ultimo;
} monticulo;

/* FUNCIONES DE VECTOR (P2) ****************************************************************************************** */

void inicializar_semilla() {
    srand(time(NULL));
}

double microsegundos() {
/* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void aleatorio(int v [], int n) {/* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void descendente(int v[], int n) {
    int i;
    for(i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}

void imprimirVector(int v[], int ultimo) {
    int i;
    if(ultimo >= 0){
        printf("[ ");
        if(ultimo > 0){
            for (i=0; i < ultimo; i++){
                printf("%d, ", v[i]);
            }
        }
        printf("%d ]\n", v[ultimo]);
    }
    else {
        printf("[ ]\n");
    }
}

void copiarVector(int original[], int n, int copia[]) {
    int i;
    for(i=0; i<n; i++) {
        copia[i] = original[i];
    }
}

/* FUNCIONES DE MONTICULOS ******************************************************************************************* */

void crearMonticuloVacio(monticulo *mont) {
    mont->ultimo = -1;
}
int es_monticulo_vacio(monticulo mont) {
    if (mont.ultimo == -1)
        return 1;
    else
        return 0;
}

void hundir(monticulo *m, int i) {
    int hijoder, hijoizq, j, aux;
    do {
        hijoizq=2*i+1;
        hijoder=2*i+2;
        j=i;
        if (hijoder<=m->ultimo && m->vector[hijoder]>m->vector[i]) i=hijoder;
        if (hijoizq<=m->ultimo && m->vector[hijoizq]>m->vector[i]) i=hijoizq;
        aux=m->vector[i];
        m->vector[i]=m->vector[j];
        m->vector[j]=aux;
    } while (j!=i);
}

void crear_monticulo(int vector[], int tam, monticulo *mont) {
    int i;
    for (i=0;i<tam;i++)
        mont->vector[i] = vector[i];
    mont->ultimo=tam-1;
    for (i=((tam-1) / 2); i>=0 ;i--) hundir(mont,i);
}

int eliminar_mayor(monticulo *mont) {
    int x;
    if (mont->ultimo == -1) {
        printf ("Error: Monticulo vacío\n");
        return -1;
    }
    else {
        x=mont->vector[0];
        mont->vector[0]=mont->vector[mont->ultimo];
        mont->ultimo=mont->ultimo-1;
        if (mont->ultimo>=0)
            hundir(mont,0);
        return x;
    }
}

void ord_monticulo(int v[], int n) {
    monticulo m;
    int i;

    crearMonticuloVacio(&m);
    crear_monticulo(v,n,&m);
    for (i = n-1 ; i >= 0 ; i--)
        v[i]=eliminar_mayor(&m);
}

/* ANALISIS ********************************************************************************************************** */

void test_crear_elimMayor( void (*llenarV) (int[], int) ) {
    monticulo mont;
    int i = 1;
    int vector[VTEST];
    llenarV(vector, VTEST);
    printf("El vector");
    imprimirVector(vector, (VTEST-1));
    printf("se coloca en el monticulo, de esta forma: \n");
    crear_monticulo(vector,VTEST, &mont);
    imprimirVector(mont.vector, mont.ultimo);

    while(!es_monticulo_vacio(mont)) {
        printf("Se elimina el maximo %d veces y el vector queda asi:  ", i);
        eliminar_mayor(&mont);
        imprimirVector(mont.vector, mont.ultimo);
        i++;
    }
}

void test_On( void (*llenarV) (int[], int) ) {
    int i;
    int vectorAux[TAM];
    double ta,tb, t;
    char c;
    monticulo mont;

    crearMonticuloVacio(&mont);

    printf("Test para demostrar que la complejidad de crear un monticulo es O(n):\n");
    printf("%-8s %15s %20s\n","nº de elementos", "t", "t / n");
    for(i=2000; i <= 512000; i = 2*i) {
        c = ' ';
        llenarV(vectorAux,i);
        ta = microsegundos();
        crear_monticulo(vectorAux,i,&mont);
        tb = microsegundos();
        t = tb-ta;
        if(t < 500)
            c = '*';
        printf("%c %8d %20f %20f \n",c, i, t, t/i );
        //printf("Crear un monticulo de %d elementos tarda: %.3f microsegundos\n", i, t);
        crearMonticuloVacio(&mont);
    }
    //puts("Podemos deducir que crear un monticulo el doble de grande que el anterior tarda el doble de tiempo.\n"
    //     "Por lo que la complejidad es O(n) .");
}

void test_ord( void (*llenarV) (int[], int)  ) {
    int vector[VTEST];
    monticulo mont;
    crearMonticuloVacio(&mont);

    llenarV(vector, VTEST);
    crear_monticulo(vector,VTEST, &mont);
    printf("Monticulo:\n");
    imprimirVector(mont.vector, (VTEST -1));
    printf("Monticulo Ordenado:\n");
    ord_monticulo(mont.vector, VTEST);
    imprimirVector(mont.vector, (VTEST -1));
}

void tests() {
    test_crear_elimMayor(aleatorio);
    //test_crear_elimMayor(ascendente);
    //test_crear_elimMayor(descendente);

    puts("");

    //test_On(aleatorio);
    test_On(ascendente);
    //test_On(descendente);

    puts("");

    //test_ord(aleatorio);
    //test_ord(ascendente);
    test_ord(descendente);

    puts("");
}

void imprimir_tabla_asc(int vector[], int len) {
    int vcopia[TAM];
    char c;
    double ta, tb, t1, t2, t;
    double f, g, h;
    int i;
    monticulo mont;
    crearMonticuloVacio(&mont);
    c = ' ';

    copiarVector(vector, len, vcopia);

    ta = microsegundos();
    ord_monticulo(vcopia, len);
    tb = microsegundos();

    t = tb - ta;
    if (t < confianza) {
        c = '*';
        ta = microsegundos();
        for (i = 0; i < kte; i++) {
            copiarVector(vector, len, vcopia);
            ord_monticulo(vcopia, len);
        }
        tb = microsegundos();
        t1 = tb - ta;
        ta = microsegundos();
        for (i = 0; i < kte; i++) {
            copiarVector(vector, len, vcopia);
        }
        tb = microsegundos();
        t2 = tb - ta;
        t = (t1 - t2) / kte;
    }

    f = len;
    g = len * log(len);
    h = pow (len, 1.2);

    printf("%c %-10d\t%15.5f\t%20.10f\t%20.10f\t%20.10f\n", c, len, t, t/f , t/g , t/h) ;
}

void tablas_monticulos_asc() {
    int vector[TAM];
    int n;
    printf("Medicion de tiempos de ordenar monticulos, con vector ordenado ascendente: \n%-10s\t%15s\t%20s\t%20s\t%20s\n",
     "n", "t(n)","t(n) / f(n)", "t(n) / g(n)", "t(n) / h(n)");
    for (n = 2000; n <= TAM; n = 2*n){
        ascendente(vector, n);
        imprimir_tabla_asc(vector, n);
    }
    printf("\n");
}

void imprimir_tabla_desc(int vector[], int len) {
    int vcopia[TAM];
    char c;
    double ta, tb, t1, t2, t;
    double f, g, h;
    int i;
    monticulo mont;
    crearMonticuloVacio(&mont);
    c = ' ';
    copiarVector(vector, len, vcopia);

    ta = microsegundos();
    ord_monticulo(vcopia, len);
    tb = microsegundos();

    t = tb - ta;
    if (t < confianza) {
        c = '*';
        ta = microsegundos();
        for (i = 0; i < kte; i++) {
            copiarVector(vector, len, vcopia);
            ord_monticulo(vcopia, len);
        }
        tb = microsegundos();
        t1 = tb - ta;
        ta = microsegundos();
        for (i = 0; i < kte; i++) {
            copiarVector(vector, len, vcopia);
        }
        tb = microsegundos();
        t2 = tb - ta;
        t = (t1 - t2) / kte;
    }

    f = pow(len, 1.1);
    g = len * log(len);
    h = pow (len, 1.2);

    printf("%c %-7d\t%15.5f\t%20.10f\t%20.10f\t%20.10f\n",c, len,t, t/f , t/g , t/h) ;
}

void tablas_monticulos_desc() {
    int vector[TAM];
    int n;
    printf("Medicion de tiempos de ordenar monticulos, con vector ordenado descendente : \n%-10s\t%15s\t%20s\t%20s\t%20s\n",
     "n", "t(n)","t(n) / f(n)", "t(n) / g(n)", "t(n) / h(n)");
    for (n = 2000; n <= TAM; n = 2*n){
        descendente(vector, n);
        imprimir_tabla_desc(vector, n);
    }
    printf("\n");
}

void imprimir_tabla_aleatorio(int vector[], int len) {
    int vcopia[TAM];
    char c;
    double ta, tb, t1, t2, t;
    double f, g, h;
    int i;
    monticulo mont;
    crearMonticuloVacio(&mont);
    c = ' ';
    copiarVector(vector, len, vcopia);

    ta = microsegundos();
    ord_monticulo(vcopia, len);
    tb = microsegundos();

    t = tb - ta;
    if (t < confianza) {
        c = '*';
        ta = microsegundos();
        for (i = 0; i < kte; i++) {
            copiarVector(vector, len, vcopia);
            ord_monticulo(vcopia, len);
        }
        tb = microsegundos();
        t1 = tb - ta;
        ta = microsegundos();
        for (i = 0; i < kte; i++) {
            copiarVector(vector, len, vcopia);
        }
        tb = microsegundos();
        t2 = tb - ta;
        t = (t1 - t2) / kte;
    }

    f = pow(len, 1.1);
    g = len * log(len);
    h = pow (len, 1.2);

    printf("%c %-10d\t%15.5f\t%20.10f\t%20.10f\t%20.10f\n",c, len, t, t/f , t/g , t/h) ;
}

void tablas_monticulos_aleatorio() {
    int vector[TAM];
    int n;
    printf("Medicion de tiempos de ordenar monticulos, con vector desordenado :\n%-10s\t%15s\t%20s\t%20s\t%20s\n",
     "n", "t(n)","t(n) / f(n)", "t(n) / g(n)", "t(n) / h(n)");
    for (n = 2000; n <= TAM; n = 2*n) {
        aleatorio(vector, n);
        imprimir_tabla_aleatorio(vector, n);
    }
    printf("\n");
}


int main() {
    inicializar_semilla();

    tests();

    tablas_monticulos_asc();
    tablas_monticulos_desc();
    tablas_monticulos_aleatorio();

    return 0;
}
