// Carlos Martinez Rabunal, carlos.martinez4@udc.es
// Anton Lopez Nunez, anton.lopez.nunez@udc.es


#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#define MAX 256000
#define confianza 500
#define kte 1000

struct nodo {
    int elem;
    int num_repeticiones;
    struct nodo *izq, *der;
};
typedef struct nodo *posicion;
typedef struct nodo *arbol;

// Cabezeras de funciones
arbol insertar(int e, arbol a);
arbol creararbol();
int esarbolvacio(arbol a);
posicion buscar(int elemento, arbol a);
arbol eliminararbol(arbol a);
posicion hijoizquierdo(arbol a);
posicion hijoderecho(arbol a);
int elemento(posicion p);
int numerorepeticiones(posicion p);
int altura(arbol a);
void visualizar(arbol a);

/* ************************************************************************* */
// Funciones para la medicion de tiempos

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

// Otras funciones auxiliares

void aleatorio(int v[], int n) {/*genera números pseudoaleatorio entre -n y +n*/
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void inicializar_semilla() {
    srand(time(NULL));
}

/* ************************************************************************* */
// Funciones de manejo de arboles binarios de busqueda

static struct nodo *crearnodo(int e) {
    struct nodo *p = malloc(sizeof(struct nodo));
    if (p == NULL) {
        printf("memoria agotada\n"); exit(EXIT_FAILURE);
    }
    p->elem = e;
    p->num_repeticiones = 1;
    p->izq = NULL;
    p->der = NULL;
    return p;
}

arbol insertar(int e, arbol a) {
    if (a == NULL)
        return crearnodo(e);
    else if (e < a->elem)
        a->izq = insertar(e, a->izq);
    else if (e > a->elem)
        a->der = insertar(e, a->der);
    else
        a->num_repeticiones++;
    return a;
}

arbol creararbol() {
    arbol a = NULL;
    return a;
}

int esarbolvacio(arbol a) {
    return a == NULL;
}

posicion buscar(int i, arbol a) {
    if(esarbolvacio(a)){
        return NULL;
    }
    if(i == a->elem){
        return a;
    }
    else if(i < a->elem){
        return buscar(i, a->izq);
    }
    else if(i > a->elem){
        return buscar(i, a->der);
    } else 
        return NULL;
}

arbol eliminararbol(arbol a) {
    if (a != NULL)
    {
        a->izq = eliminararbol(hijoizquierdo(a));
        a->der = eliminararbol(hijoderecho(a));
        free(a);
    }
    return NULL;
}

posicion hijoizquierdo(arbol a) {
    return a->izq;
}

posicion hijoderecho(arbol a) {
    return a->der;
}

int elemento(arbol a) {
    return a->elem;
}

int numerorepeticiones(posicion p) {
    return p->num_repeticiones;
}

int altura(arbol a) {
    int alturaIzq, alturaDer;
    if(a==NULL){
        return -1;
    }
    else{
        alturaIzq = altura(a->izq);
        alturaDer = altura(a->der);
        if(alturaIzq > alturaDer){
            return alturaIzq+1;
        }
        else return alturaDer+1;
    }
}

void visualizarRecursivo(arbol a) {
    if(a!=NULL){
        printf("(");
        visualizarRecursivo(a->izq);
        printf("%d",a->elem);
        visualizarRecursivo(a->der);
        printf(")");
    }
}

void visualizar(arbol a) {
    if(a == NULL)
        puts("Arbol Vacio () .");
    else{
        printf("Arbol: ");
        visualizarRecursivo(a);
        puts(".");
    }

}

/* ************************************************************************* */

void test() {
    int i;
    posicion p;
    arbol a;
    a = creararbol();
    visualizar(a);
    a = insertar(3,a);
    a = insertar(1,a);
    a = insertar(2,a);
    a = insertar(5,a);
    a = insertar(4,a);
    a = insertar(5,a);
    visualizar(a);
    printf("altura: %d\n", altura(a));
    for(i=0; i<7;i ++){
        printf("busco %d:", i);
        p = buscar(i,a);
        if(p == NULL){
            puts(" y no encuentro nada");
        }
        else{
            printf(" y encuentro %d repetido %d veces\n",i, 
            numerorepeticiones(p));
        }
    }
    printf("Borro todos los nodos liberando la memoria: \n");
    a = eliminararbol(a);
    visualizar(a);
    printf("Altrua del arbol: %d\n",altura(a));
}

void TestInsertarBuscar () {
    int n = 8000;
    int i, v[MAX], b[MAX];
    //posicion p = buscar(b[i], n);
    arbol a = creararbol();
    aleatorio(v, n);
    aleatorio(b, n);
    

    double t1, t2, t3, t4;
    printf("%6s%15s%15s\n", "n", "t_ins(n)", "t_bus(n)");

    // Comienza en 8000 para evitar valores menores que 500 microsegundos
    for (n = 8000; n <= 256000; n = n*2) { 
        a = creararbol();

        t1 = microsegundos();
        for (i = 0; i < n; i++) {
            a = insertar(v[i], a);
        }
        t2 = microsegundos();

        t3 = microsegundos();
        for (i = 0; i < n; i++) {
            buscar(b[i], a);
        }
        t4 = microsegundos();
        
        eliminararbol(a);
        if (t2-t1 < 500) {
            printf("%7d%12.0f%15.0f\n", n, t2-t1, t4-t3);
        }
        
    }
}

double tiempos_ins(arbol (*alg)(int v, arbol a), int tamanho, int k, arbol arb){
    int i;
    double t1, t2, t;
    int *v = malloc(tamanho * sizeof(int));

    aleatorio(v, tamanho);
    t1 = microsegundos();
    for (i = 0; i < tamanho; i++)
        arb = alg(v[i], arb);
    t2 = microsegundos();
    t = t2 - t1;
    free(v);
    return t;
}

double tiempos_bus(arbol (*alg)(int v, arbol a), int tamanho, int k, arbol arb){
    int i, j;
    double t1, t2, t;
    int *v = malloc(tamanho * sizeof(int));

    aleatorio(v, tamanho);
    t1 = microsegundos();
    for (i = 0; i < tamanho; i++)
        alg(v[i], arb);
    t2 = microsegundos();
    t = t2 - t1;
    if (t < 500)
    {
        printf("(*)");
        t1 = microsegundos();
        for (i = 0; i < k; i++)
            for (j = 0; i < tamanho; i++)
                alg(v[j], arb);
        t2 = microsegundos();
        t = t2 - t1;
    }
    free(v);
    return t;
}

void print_tabla() {
    arbol arb = creararbol();
    int i, n, k = 1000, max = 512000;
    int *v = malloc(max * sizeof(int));
    double tiempo = 0.0, tmax = 1500000.0;
    printf("\nInserción de n elementos:\n");
    printf("%7s%14s%18s%15s%15s\n",
    "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    for (n = 8000; n <= max && tiempo <= tmax; n = n * 2) {
        arb = creararbol();
        tiempo = tiempos_ins(insertar, n, k, arb);

        if (tiempo >= 500) /* con n >= 8000 debería imprimir siempre */
            printf("%8d%15.3f%15f%15f%15f\n", n, tiempo, tiempo/pow(n, 1.1),
            tiempo/pow(n, 1.25), tiempo/pow(n, 1.3));
        arb = eliminararbol(arb);
    }
    tiempo = 0.0;
    printf("\nBúsqueda de n elementos:\n");
    printf("%7s%14s%18s%15s%15s\n", 
    "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
    for (n = 8000; n <= max && tiempo <= tmax; n = n * 2) {
        aleatorio(v, n);
        for (i = 0; i < n; i++)
            arb = insertar(v[i], arb);
        tiempo = tiempos_bus(buscar, n, k, arb);
        printf("%8d%15.3f%15f%15f%15f\n",
               n, tiempo, tiempo / pow(n, 1.1), tiempo / pow(n, 1.25),
               tiempo / pow(n, 1.3));
        arb = eliminararbol(arb);
    }
    free(v);
}

int main() {
    int v[MAX];
    int n = 8000;
    inicializar_semilla();
    aleatorio(v, n);

    printf("Test de funcionamiento de las funciones de manejo del ABB: \n\n");
    test();
    printf("\n");

    printf("Test de Insertar y buscar n elementos en un arbol vacio: \n\n");
    TestInsertarBuscar();
    printf("\n");
    
    printf("Medicion de tiempos de insercion y busqueda: \n\n");
    print_tabla();
    printf("\n");

    return 0;
}
