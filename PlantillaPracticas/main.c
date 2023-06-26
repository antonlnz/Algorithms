#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>

void intercambiar(int v[], int i, int j) { // Funcion auxiliar
	int aux;
	aux=v[i];
	v[i]=v[j];
	v[j]=aux;
}

void ordenar(int v [], int n) { 
	// Escribir aqui el algoritmo de ordenación a emplear
}

//INICIALIZACION VECTORES
void inicializar_semilla() {
	srand(time(NULL));
}

void aleatorio(int v [], int n) {
/* se generan números pseudoaleatorio entre -n y +n */
	int i, m;
	m=2*n+1;
	for (i=0; i < n; i++)
		v[i] = (rand() % m) - n;
}

void ascendente(int v [], int n) {
	int i;
	for (i=0; i < n; i++)
		v[i] = i;
}

void descendente(int v [], int n) {
	int i;
	for (i=0; i<n; i++)
		v[i] = n-i;
}

//TIEMPO

double microsegundos() {
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0)
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}

float calcularTiempo(void (*Vec)(int[], int), void (*Ord)(int[], int), int n){ 
    //se introduce la inicialización y el algoritmo de ordenación
	int v[n];
	double t1, t2, t;
	Vec(v, n);
	t1 = microsegundos();
	Ord(v, n);
	t2 = microsegundos();
	t = t2 - t1;
	return t;
}

float calcTMenor500(void(*Vec)(int[], int),void(*Ord)(int[],int),int n){
	int f, v[n], k;
	double t1, t2, ta, tb, t;
	k=10000;
	t1 = microsegundos();
	for(f=0; f<k; f++){
		Vec(v, n);
		Ord(v, n);
	}
	t2 = microsegundos();
	ta = t2 - t1;


	t1 = microsegundos();
	for(f=0; f<k; f++){
		Vec(v, n);
	}
	t2 = microsegundos();
	tb = t2 - t1;
	t=ta-tb;
	return t/k;
}

//IMPRESION
void barra(){
	int i;
	for (i=0; i<75; i++) 
		printf("-");
	printf("\n");
}


void imprimirLinea(int n, float t, char c, float x, float y, float z){
	printf("%12d%15.4f%c", n, t, c);
	printf("%15.6f%15.6f%15.7f\n", x, y, z);
}

void cotas(float *x, float *y, float *z, int tipo, int n, float t) { // HAY QUE MODIFICAR LAS COTAS
//x, y, z : resultado cotas //tipo : vector //n : it //t : medicion del tiempo
	if (tipo==1){ //x - sube, y - estable, z - desciende
		//aleatorio
		*x = t / pow(n, 1.9);
		*y = t / pow(n,2.);
		*z = t / pow(n, 2.1);
	}else if (tipo==2){
		//descendente
		*x = t / pow(n, 1.5); 
		*y = t / pow(n, 2.);
		*z = t / pow(n, 2.1);

	}else if (tipo==3){
		//ascendente
		*x = t / pow(n, 1.9);
		*y = t / pow(n, 2.);
		*z = t / pow(n, 2.1); 
	}
}

void ejecucion(int n, int tipo, void(*Vec)(int[], int)) {
	float x, y, z, t;
	char menor_500;
	t=calcularTiempo(Vec, ordenar, n);
	if (t < 500) {
		t=calcTMenor500(Vec, ordenar, n); 
		menor_500='*';
	}
	else menor_500=' ';
	cotas(&x, &y, &z, tipo, n, t);
	imprimirLinea(n, t, menor_500, x, y, z);
}

void imprimir() { // HAY QUE MODIFICAR LAS COTAS A IMPRIMIR
	int n;
	
	printf("\nVector ascendente:\n");
	printf("%11s%19s%14s%15s%15s", 
	    "n", "t(n)(μs)", "t/n^1.9", "t/n^2", "t/n^2.1\n");
	barra();
	for (n=250; n<=32000; n=2*n) ejecucion(n, 3, ascendente);

	printf("\nVector descendente:\n");
	printf("%11s%19s%14s%15s%15s", 
	    "n", "t(n)(μs)", "t/n^1.5", "t/n^2", "t/n^2.1\n");
	barra();
	for (n=250; n<=32000; n=2*n) ejecucion(n, 2, descendente);

	printf("\nVector aleatorio:\n");
	printf("%11s%19s%14s%15s%15s", 
	    "n", "t(n)(μs)", "t/n^1.9", "t/n^2", "t/n^2.1\n");
	barra();
	for (n=250; n<=32000; n=2*n) ejecucion(n, 1, aleatorio);
}

//TEST

void imprimirVector(int v[], int n){
	int i;
	for (i=0; i < n-1; i++)
		printf("%d, ", v[i]);
	printf("%d", v[n-1]);
	printf("\n");
}

bool estaOrdenado(int v[], int n){
	int i;
	for (i=0; i<n-1;i++){
		if(v[i]>v[i+1]) return false;
	}
	return true;
}

void testear(){
	int n;
	n=15;
	inicializar_semilla();	
	int v1[n], v2[n], v3[n];
	aleatorio(v1, n);
	descendente(v2, n);
    ascendente(v3, n);
	
	printf("Ordenacion con inicializacion aleatoria\n");
	imprimirVector(v1, n);
	printf("ordenado? %s\n", estaOrdenado(v1, n)?"1":"0");
	printf("ordenando...\n");
	ordenar(v1, n); // Algoritmo de ordenación
	imprimirVector(v1, n);
	printf("ordenado? %s\n", estaOrdenado(v1, n)?"si":"no");
	printf("\n\n");

    printf("Ordenacion con inicializacion ascendente\n");
	imprimirVector(v3, n);
	printf("ordenado? %s\n", estaOrdenado(v3, n)?"1":"0");
	printf("ordenando...\n");
	ordenar(v3, n); // Algoritmo de ordenación
	imprimirVector(v3, n);
	printf("ordenado? %s\n", estaOrdenado(v3, n)?"si":"no");
    printf("\n\n");

	printf("Ordenacion con inicializacion descendente\n");
	imprimirVector(v2, n);
	printf("ordenado? %s\n", estaOrdenado(v2, n)?"1":"0");
	printf("ordenando...\n");
	ordenar(v2, n); // Algoritmo de ordenación
	imprimirVector(v2, n);
	printf("ordenado? %s\n", estaOrdenado(v2, n)?"si":"no");
}

//PROGRAMA

void noValoresAnomalos(int n, int tipo, void(*Vec)(int[], int)) {
	float t;
	t=calcularTiempo(Vec, ordenar, n);
	if (t < 500) {
		t=calcTMenor500(Vec, ordenar, n); 
	}
}

int main(){ 
	inicializar_semilla();
	testear();
	
	//Evitar valores anomalos:
	noValoresAnomalos(1000, 1, descendente);

	imprimir();
	
}	