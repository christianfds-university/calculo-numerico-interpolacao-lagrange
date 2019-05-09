#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//3 -2 2 0 -2 4 1

void multiplicaPolinomio(int n1, int n2, double *polinomio1, double *polinomio2);
void somaPolinomio(int n, double *polinomio1, double *polinomio2);
void *calculaL(int n, double *vetorx, double *vetorFx, int pol, double *polinomio);
double *calculaResultado(int n, double *vetorx, double *vetorFx);
void lerPontos(int n, double *vetorx, double *vetorFx);
void imprimeResultado(int n, double *resultado);
void testaValores(int n, double *resultado);

int main()
{

	int n;
	double *vetorx, *vetorFx;
	double *resultado;

	printf("Quantos pontos serão lidos?\n");
	scanf("%d", &n);

	vetorx    = (double *) calloc(n,sizeof(double));
	vetorFx   = (double *) calloc(n,sizeof(double));

	lerPontos(n,vetorx,vetorFx);
	putchar('\n');
	resultado = calculaResultado(n,vetorx,vetorFx);
	imprimeResultado(n,resultado);

	testaValores(n,resultado);

	free(vetorx);
	free(vetorFx);
	free(resultado);
}

void imprimeResultado(int n, double *resultado)//Imprime o polinômio interpolador dos pontos inseridos
{
	int i;
	printf("Polinômio Interpolador:\nP(x)= ");
	for(i = n-1; i >= 0; i--)
	{
		if(i==0) printf("%+2.3lf\n", resultado[i]);
		else 
		if(i==1) printf("%+2.3lfx ", resultado[i]);
		else     printf("%+2.3lf(x^%d) ", resultado[i], i);
	}
}


void multiplicaPolinomio(int n1, int n2, double *polinomio1, double *polinomio2)//Realiza a multiplicação de dois polinômios
{ //Resultado será devolvido no polinômio 1

	double resultado[n1+n2-1];
	
	int i,j;

	for(i = 0; i < (n1+n2-1); i++)
	{
		resultado[i] = 0;//Inicializa o vetor de resultados
	}

	for(i = 0; i < n1; i++)
	{
		for(j = 0; j < n2; j++)
		{
			resultado[i+j] += polinomio1[i] * polinomio2[j];//Realiza a multiplicação
		}
	}

	for(i = 0; i < (n1+n2-1); ++i)
	{
		polinomio1[i] = resultado[i];//Salva as alterações no polinômio1
	}

}

void somaPolinomio(int n, double *polinomio1, double *polinomio2)//Realiza a soma de dois polinômios
{ // Resultado será devolvido no polinômio 1

	int i;
	for(i = 0; i < n; ++i)
	{
		polinomio1[i] += polinomio2[i];
	}

}

void *calculaL(int n, double *vetorx, double *vetorFx, int pol, double *polinomio)//Realiza os calculos referentes ao L
{ // Resultado devolvido em polinomio

	double numerador[n-1][2];
	double denominador = 1;

	int i;

	for(i = 0; i < n; i++)
	{
		polinomio[i] = 0;//Inicializa o polinômio com 0
	}

	int aux=0;//Variável auxiliar para evitar x1-x1
	for(i = 0; i < n; i++)
	{
		if(i==pol)
		{
			aux++;
			continue;
		}
		numerador[i-aux][1]=1;
		numerador[i-aux][0]= -vetorx[i];

		denominador *= (vetorx[pol]-vetorx[i]);
	}

	polinomio[0] = numerador[0][0];//Inicializa o polinômio com os valores do primeiro numerador
	polinomio[1] = numerador[0][1];//Inicializa o polinômio com os valores do primeiro numerador

	for(i = 1; i < n-1; i++)
	{
		multiplicaPolinomio(2+i-1,2,polinomio,numerador[i]);//Realiza a multiplicação dos polinômios numeradores
	}

	for(i = 0; i < n; ++i)
	{
		polinomio[i] *= vetorFx[pol]/denominador;//Divide o polinômio resultante pelo denominador
	}

}

double *calculaResultado(int n, double *vetorx, double *vetorFx)//Faz a chamada dos métodos
{

	double *resultado = calloc(n,sizeof(double));
	double polinomios[n][n];
	int i;

	for(i = 0; i < n; i++)
	{
		calculaL(n, vetorx, vetorFx, i, polinomios[i]);//Calcula o L de cada linha
	}
	
	for(i = 0; i < n; i++)
	{
		somaPolinomio(n,resultado,polinomios[i]);//Soma todos os L's para ter uma função única
	}

	return resultado;
}

void lerPontos(int n, double *vetorx, double *vetorFx)//Le os pontos inseridos pelo usuário
{
	int i;

	for(i = 0; i < n; i++)
	{
		
		printf("x%d: ", i);
		scanf("%lf",vetorx+i);//Le x

		printf("f(x%d): ", i);
		scanf("%lf",vetorFx+i);//Le f(x)

	}

}

void testaValores(int n, double *polinomio)
{
	char a;
	double x;
	double resultado;
	int i;
	getchar();
	printf("\n\nDeseja testar algum valor? s/n\n");
	a = getchar();
	while(a=='s')
	{

		scanf("%lf", &x);
		resultado = 0;
		for(i = 0; i < n; i++)
		{
			if(i==0) resultado += polinomio[i];
			else     resultado += polinomio[i] * pow(x,i);
		}

		printf("\nResultado para f(%2.3lf): %5.5lf\n\n", x, resultado);

		getchar();
		printf("Deseja testar algum valor? s/n\n");
		a = getchar();
	}
}
