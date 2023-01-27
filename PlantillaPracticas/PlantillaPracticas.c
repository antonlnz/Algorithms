#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define n_test 10
#define n_max 64000

// Funciones de ayuda de otras practicas
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

void aleatorio(int v [], int n) {
	int i, m=2*n+1;
	for(i=0; i < n; i++)
		v[i] = (rand() % m) - n;
/* se generan números pseudoaleatorio entre -n y +n */
}

void ascendente (int v[], int n){
	int i;
	for(i=0; i<n; i++){
		v[i]=i;
	}
}	

void descendente (int v[], int n){
	int i,j=n-1;
	for(i=0; i<n; i++){
		v[i]=j;
		j--;
	}
}

void listar_vector(int v[], int n){
	int i;
	for(i=0; i<n; i++){
		if(v[i]>=0){+
			printf("%d ", v[i]);
		}else printf("%d ", v[i]);
	}
	printf("\n");
}

bool estaOrdenado(int v[], int length) {
	int i;

	for (i = 0; i < length - 1; i++) {
		if (v[i] > v[i + 1])
			return false;
	}
	return true;
}

// -----------------Aqui comienza el codigo del algoritmo----------------------
// Cambiar la palabra "ordenar" en el codigo, por el nombre de la funcion de ordenacion
// -----------------Aqui finaliza el codigo del algoritmo----------------------

// Funcion para validar el funcionamiento del algoritmo dado
void test_fun(void(*func)(int v[],int n),void(*ord)(int v[],int n)){
	int n = n_test;
    int v[n];

	func(v,n);

	if(func==aleatorio){printf("\nOrdenación "
	"con inicialización aleatoria\n");
	}else if(func==ascendente){printf("\nOrdenación "
	"con inicialización ascendente\n");
	}else if(func==descendente){printf("\nOrdenación "
	"con inicalización descendente\n");
	}

	listar_vector(v,n);
	printf("Ordenado? %d\n", estaOrdenado(v,n));
	ord(v,n);
	printf("ordenando... \n");
	listar_vector(v,n);
	printf("Esta ordenado\n\n");
}

void cotas(int n, double t, float va, float vb, float vc){
	double a,b,c;
	a=t/(pow(n,va));
	b=t/(pow(n,vb));
	c=t/(pow(n,vc));
	printf("%9d  %13.5f%16f%16f%16f\n",n,t,a,b,c);
}

void imprimir(void(*func)(int v[],int n),void(*ord)(int v[],int n)){
	if(func==aleatorio){
		printf("\nOrdenación con inicialización aleatoria: "
        "\n\tn\tt(n)\t\tt(n)/n¹⁹\tt(n)/n²\t\tt(n)n²¹\t\n"); // Modificar con las cotas nuevas
	}else if(func==descendente){
        printf("\nOrdenación con inicialización descendente: "
        "\n\tn\tt(n)\t\tt(n)/n¹⁹\tt(n)/n²\t\tt(n)n²¹\t\n"); // Modificar con las cotas nuevas
	}else if(func==ascendente){
		printf("\nOrdenación con inicialización ascendente: "
        "\n\tn\tt(n)\t\tt(n)/n¹⁹\tt(n)/n²\t\tt(n)n²¹\t\n"); // Modificar con las cotas nuevas
	}	
}

void MedirTiempos(void(*func)(int v[],int n),void(*ord)(int v[],int n)){
	int v[n_max],k=1000,n=2000,i=0,j=0;
	double ta,tb,t,t1,t2;
	imprimir(func,ord);
	for(n=2000;n<=n_max;n=n*2){
		func(v,n);
		ta=microsegundos();
		ord(v,n);
		tb=microsegundos();
		t =tb-ta;
		if(t<500){
			ta=microsegundos();
			for(i=0;i<k;i++){
				func(v,n);
				ord(v,n);}
			tb=microsegundos();
			t1=tb-ta;
			ta=microsegundos();
			for(j=0;j<k;j++){
				func(v,n);}	
			tb=microsegundos();
			t2=tb-ta;
			t=(t1-t2)/k;
        }
        if(func==aleatorio){
            cotas(n,t,1.9,2,2.1); // Modificar con las cotas nuevas
		}else if(func==ascendente){
			cotas(n,t,0.8,1,1.3); // Modificar con las cotas nuevas
		}else if(func==descendente){
            cotas(n,t,1.8,2,2.2); // Modificar con las cotas nuevas
        }
	}
}

int main() {
    
    int i;
	inicializar_semilla();


	test_fun(aleatorio, ordenar);
	test_fun(ascendente, ordenar);
	test_fun(descendente, ordenar);

    // Calculamos la complejidad para 3 situaciones
    for(i=0;i<6;i++){
		printf("%d intento",i+1);
		MedirTiempos(aleatorio,ordenar);
		printf("----------------------------------------------------------\n");
		MedirTiempos(descendente,ordenar);
		printf("-----------------------------------------------------------\n");
        MedirTiempos(ascendente,ordenar);
		printf("-----------------------------------------------------------\n");
	}
	return 0;

}
