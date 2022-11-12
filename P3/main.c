// Carlos Martinez Rabunal, carlos.martinez4@udc.es
// Anton Lopez Nunez, anton.lopez.nunez@udc.es

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

/* ******************************************************************************************************************* */
// Funciones para la medicion de tiempos

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

// Otras funciones auxiliares

void aleatorio(int v [], int n) { /* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

/* ******************************************************************************************************************* */
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
    }
}

arbol eliminararbol(arbol a) {
    if(esarbolvacio(a->der) && esarbolvacio(a->izq)){
        free (a);
    }
    else if(!esarbolvacio(a->der)){
        eliminararbol(a->der);
    }
    else if(!esarbolvacio(a->izq)){
        eliminararbol(a->izq);
    }
}

posicion hijoizquierdo(arbol a) {
    return a->der;
}

posicion hijoderecho(arbol a) {
    return a->izq;
}

int elemento(arbol a) {
    return a->elem;
}

int numerorepeticiones(posicion p) {
    return p->num_repeticiones;
}

int alturaRecursiva(arbol a) {
    int alturaderecha, alturaizquierda;

    alturaderecha = altura(a->der);
    alturaizquierda = altura(a->izq);

    if(alturaizquierda > alturaderecha){
        return(alturaizquierda+1);
    }
    else
        return(alturaderecha+1);
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

/* ******************************************************************************************************************* */

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
            printf(" y encuentro %d repetido %d veces\n",i, numerorepeticiones(p));
        }
    }
    printf("Borro todos los nodos liberando la memoria: \n");
    a = eliminararbol(a);
    visualizar(a);
    printf("Altrua del arbol: %d\n",altura(a));
}

void InsertBuscar (int v[], int n) {
    n = 8000;
    int i, b[MAX];
    posicion p;
    arbol a = creararbol();
    aleatorio(b, n);
    

    double t1, t2, t3, t4;
    printf("%6s%15s%15s\n", "n", "t_ins(n)", "t_bus(n)");


    for (n = 8000; n <= 256000; n = n*2) {

        t1 = microsegundos();
        for (i = 0; i < n; i++) {
            a = insertar(v[i], a);
        }
        t2 = microsegundos();

        t3 = microsegundos();
        for (i = 0; i < n; i++) {
            p = buscar(b[i], a);
        }
        t4 = microsegundos();

        printf("%7d%15f%15f\n", n, t2-t1, t4-t3);
    }
}

void TablaInsercion (int v[], int n) {
    n = 8000;
    int i;
    aleatorio(v, n);

    arbol a;
    a = creararbol();

    double t, t1, t2;
    printf("%6s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");

    for (n = 8000; n <= 256000; n = n*2) {
        t1 = microsegundos();
        for (i = 0; i < n; i++) {
            a = insertar(v[i], a);
        }
        t2 = microsegundos();
        t = t2-t1;
        printf("%7d%15f%13f%16f%16f\n", n, t, t/n, t/log(n), t/n*log(n)); // La complejidad media es O(log(n))
    }
}

void TablaBusqueda(int v[], int n) {
    n = 8000;
    int i, b[MAX];
    posicion p;
    aleatorio(b, n);

    arbol a;
    a = creararbol();

    double t, t1, t2;
    printf("%6s%15s%15s%15s%15s\n", "n", "t(n)", "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");

    for (n = 8000; n <= 256000; n = n*2) {
        for (i = 0; i < n; i++) {
            a = insertar(v[i], a);
        }

        t1 = microsegundos();
        for (i = 0; i < n; i++) {
            p = buscar(b[i], a);
        }
        t2 = microsegundos();

        t = t2-t1;
        printf("%7d%16f%13f%16f%14f\n", n, t, t/n, t/log(n), t/n*log(n)); // La complejidad media es O(log(n))
    }
}

int main() {
    int v[MAX];
    int n = 8000;
    aleatorio(v, n);

    printf("Test de funcionamiento del arbol binario de busqueda: \n\n");
    test();
    printf("\n");

    printf("Test de medicion de tiempos de insercion y busqueda: \n\n");
    InsertBuscar(v, n);
    printf("\n");
    
    printf("Insercion de n elementos\n\n");
    TablaInsercion(v, n);
    printf("\n");

    printf("Busqueda de n elementos\n\n");
    TablaBusqueda(v, n);
    printf("\n");

    return 0;
}

/* 
- No haria falta mandar como argumentos los vectores.
- Ns si hay que hacer un aleatorio() cada linea de la tabla (en el for)
*/