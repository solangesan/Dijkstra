#define MAXN 100000000
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <iostream>
//#include <Lista.h>
#include <time.h>
//#include <windows.h>
#include "Lista.h"

using std::cout;
using std::endl;
using std::ostream;
using autoreferencia::Lista;

/*
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}
*/

class FPNaoOrdenado	{

	class Elemento{ //elemento da fila
		int vertice;
		int peso;

		public:
		Elemento(){

		}
  		Elemento  ( int v , int p )  {
			this->vertice  =  v ;
			this->peso = p ;
		}
		int _peso  ( )  {
			return  this->peso ;
		}
		int _vertice  ( )  {
	 		return  this->vertice ;
		}
		void atribuiPeso(int peso){
			this->peso = peso;
		}
	};
	public:
	int tamanho; //tamanho da fila
	Elemento vet[MAXN]; //vetor  de tamanho mamximo definido; contendo um Elemento(vertice, peso)

	FPNaoOrdenado (){//construtor apenas inicializa o tamanho para zero
		this->tamanho = 0;
	}

	void insere(int dado, int prior){
		if (this->tamanho == MAXN)
			throw logic_error ("Erro: Fila cheia");
		else{
			Elemento e =  Elemento (dado,prior);
			this->vet[tamanho] = e;
			this->tamanho++;
		}
	}

	Elemento remove(){  //remove o Elemento de maior prioridade = menor peso
		int menorP, posMenorP;
	  	if (this->tamanho < 1)
			throw logic_error ("Erro: fila vazia");
  		Elemento minElem, temp; //
		temp = (Elemento)this->vet[0]; //configura o primeiro elemento como o menor peso
		menorP = temp._peso();
		posMenorP = 0;

		minElem = temp;
		for(int i= 1; i< this->tamanho; i++){
	    	temp = this->vet[i]; //
    		int pesoAtual =temp._peso();
	    	if(menorP > pesoAtual){ //se o peso do 0, for maior que o peso da posicao 1, configura o menor para o da posicao 1;
	      		menorP = pesoAtual;
	      		minElem = temp;
	      		posMenorP = i;
	    	}
	    	//printf(" menorP= %d \n",  menorP);
	  	}
	  	for (int i=posMenorP; i<this->tamanho-1; i++ ){//retira o minElem, movendo os proximos pra posicao dele
	  		this->vet[i]= this->vet[i+1]; //
			/*
	  		Elemento aux = (Elemento)this->vet[i]; //
			printf(" vet[%d]= %d \n", i, this->vet[i]);
	  		printf(" peso[%d]= %d \n", i, aux._peso());
	  		printf(" vertice[%d]= %d \n", i, aux._vertice());
	  		*/
		}
		this->tamanho--;
		return minElem;
	}

	void diminuiChave(int dado, int prior){
		Elemento temp;
		if (this->tamanho < 1)
			throw logic_error ("Erro: fila vazia");
		temp = this->vet[0];
		//printf(" prior= %d \n",  prior);
		if (prior < 0)
	    	throw logic_error ("Erro: chaveNova com valor incorreto");
		for(int i= 0; i< this->tamanho; i++){
	    	temp = this->vet[i]; //
				if(prior < this->vet[i]._peso())
	      			this->vet[i].atribuiPeso(prior);
	    }
	}

	bool vazio () {
		return this->tamanho == 0;
	}

};

class Grafo  {
	public:
	class Aresta  {
		int v1 ,  v2 ,  peso;
	  	public:
		Aresta  ( int v1 , int v2 , int peso )  {
			this->v1  =  v1 ;
			this->v2  =  v2 ;
			this->peso = peso ;
		}

		int _peso  ( )  {
			return  this->peso ;
		}
		int _v1  ( )  {
	 		return  this->v1 ;
		}
		int _v2  ( )  {
	 		return  this->v2 ;
		}
	};

	class Celula {
	    friend class Grafo;
	    friend ostream& operator<< (ostream& out, const Celula& celula) {
	      out << "vertice:" << celula.vertice << endl;
	      out << "peso:"    << celula.peso    << endl;
	      return out;
	    }
		int vertice, peso;
		Celula *prox;
		Celula (int v, int p) {
		    	this->vertice = v;
				this->peso = p;
		}
		public:
		Celula (const Celula& cel) { *this = cel; }
	      bool operator== (const Celula& celula) const {
	        return this->vertice == celula.vertice;
	      }
	      bool operator!= (const Celula& celula) const {
	        return this->vertice != celula.vertice;
	      }
	      const Celula& operator= (const Celula& cel) {
	        this->vertice = cel.vertice; this->peso = cel.peso;
	        return *this; // @{\it permite atribui\c{c}\~oes encadeadas}@
	      }
	      ~Celula () {}
	};

	Lista<Celula> *adj;
    int numVertices;
    public:
    Grafo (int numVertices) {
  		this->adj = new Lista<Celula>[numVertices];
  		this->numVertices = numVertices;
	}
	void insereAresta (int v1, int v2, int peso) {
    	Celula item(v2, peso);
    	this->adj[v1].insere(item);
  	}
	bool existeAresta (int v1, int v2) const {
	    Celula item(v2, 0);
	    return (this->adj[v1].pesquisa(item) != NULL);
	}
	bool listaAdjVazia (int v) {
		return this->adj[v].vazia();
	}
	Aresta *primeiroListaAdj (int v) {
	    // @{\it Retorna a primeira aresta que o vertice v participa ou}@
	    // @{\it {\bf NULL} se a lista de adjacencia de v for vazia}@
	    // Retorna a primeira aresta que o vertice v participa ou null se a lista de adjacencia de v for vazia
	    Celula *item = this->adj[v]._primeiro();
	    return item != NULL ? new Aresta(v,item->vertice,item->peso) : NULL;
	  }

	Aresta *proxAdj (int v) {
	    // @{\it Retorna a proxima aresta que o vertice v participa ou}@
	    // @{\it {\bf NULL} se a lista de adjacencia de v estiver no fim}@
	    Celula *item = this->adj[v].proximo();
	    return item != NULL ? new Aresta(v,item->vertice,item->peso) : NULL;
	}

	Aresta *retiraAresta (int v1, int v2) {
	    Celula chave(v2, 0);
	    Celula *item = this->adj[v1].retira(chave);
	    Aresta *aresta = item != NULL ? new Aresta(v1,v2,item->peso) : NULL;
	    delete item;
		return aresta;
	}
  	void imprime(){
	    for (int i = 0; i < this->numVertices; i++) {
		    cout << "Vertice " << i << ":" << endl;
		    Celula *item = this->adj[i]._primeiro ();
		    while (item != NULL) {
		      	cout << "  " << item->vertice << " (" <<item->peso<< ")" << endl;
		    	item = this->adj[i].proximo ();
		    }
	    }
  	}
	int _numVertices () {
		return this->numVertices;
	}

  ~Grafo () {
    delete []this->adj;
  }

};

class Dijkstra {
	private:
	  int *antecessor; //vetor de antecessores dos vertices
	  int *p;//vetor de pesos do vertice

	public:
	Grafo *grafo;
	Dijkstra (Grafo *grafo) {
  		this->grafo = grafo;
		this->antecessor = NULL;
		this->p = NULL;
  	}

	void calculaDijkstraVetorNaoOrdenado (int raiz) throw (logic_error) {
		//printf("chegou aqui - 0 \n");
		int n = this->grafo->_numVertices();
		//printf("chegou aqui - 1 \n");

	    if (this->p)
			delete [] this->p;
	    // vetor de peso dos vertices - no final do algoritmo ele estará marcado na
	    //posicao [0..n] o menor caminho do vertice inicial até cada vertice u em [0..n]
	    this->p = new int[n];
	    int *vs = new int[n];

	    if (this->antecessor)
			delete [] this->antecessor;
	    this->antecessor = new int[n];

	    for (int u = 0; u < n; u ++) {
	      this->antecessor[u] = -1;
	      p[u] = INT_MAX;
	      vs[u] = u; // {\it Heap indireto a ser construido}
	    }

	    p[raiz] = 0;
	    FPNaoOrdenado *fila = new FPNaoOrdenado();

		int valorVertice;
	    int valorPeso;
	    for (int i = 0; i<n; i++){
	    	valorVertice = (int) vs[i];
	    	valorPeso = (int)p[i];
	    	//printf("i= %d \n", i);
	    	fila->insere(valorVertice,valorPeso);
		}
	    while (!fila->vazio()){

	      	int u = fila->remove()._vertice(); //u é o numero do vertice extraido

	      	if (!this->grafo->listaAdjVazia (u)) {
	        	Grafo::Aresta *adj = grafo->primeiroListaAdj (u);
				while (adj != NULL) {
	          		int v = adj->_v2 ();
	          		//o vertice v, nao está mais na mesma posicao da fila, assim, o p[v] está incorreto, buscar a posicao de v na heap
	          		if (this->p[v] > (this->p[u] + adj->_peso ())) {
	            		antecessor[v] = u;
	            		if(this->p[u] != INT_MAX){
		            		fila->diminuiChave(v, this->p[u] + adj->_peso ());
		            		this->p[v] = this->p[u] + adj->_peso ();
	            		}
	          		}
	          		delete adj;
			  		adj = grafo->proxAdj (u);
	        	}
	      	}
	    }
	    delete [] vs;
	}

  	int _antecessor (int u) {
  		return this->antecessor[u];
	}
  	int _peso (int u){
	  	return this->p[u];
	}
  	void imprime () {
    	for (int u = 0; u < this->grafo->_numVertices (); u++)
      		if (this->antecessor[u] != -1)
        		cout << "(" << antecessor[u] <<  "," << u << ") -- p:" << _peso (u) << endl;
  	}
 	void imprimeCaminho (int origem, int v) const {
    	if (origem == v)
			cout << origem << endl;
    	else if (this->antecessor[v] == -1)
      	cout << "Nao existe caminho de " << origem << " ate " << v << endl;
    	else {
      		imprimeCaminho (origem, this->antecessor[v]);
      		cout << v << endl;
    	}
  	}
  	~Dijkstra () {
  		this->grafo = NULL;
    	if (this->p)
			delete [] this->p;
  		if (this->antecessor)
		  	delete [] this->antecessor;
  }


void fechaArquivo(FILE* arquivo)
{
    fclose(arquivo);
}
};



void fechaArquivo(FILE* arquivo)
{
    fclose(arquivo);
}



// Programa main para rodar as funcoes

int main(){


	FILE *arquivoEntrada;
//	FILE *arquivoSaida;
    char prefixo[10];
    int valor1, valor2, valor3;

    // Variáveis para medir o tempo de execução
    float tempo;
    clock_t t_inicio, t_fim;

	int nVertices = 0;
	int nArestas = 0;
	int raiz = INT_MAX;

	Grafo *grafo = new Grafo (nVertices+1);

    arquivoEntrada =fopen("/home/solange/Documentos/TrabalhoAPA2/grafos_5/check/check_v5_s2.dat", "r");

	printf("abriu o arquivo \n\n");
	if(arquivoEntrada == NULL)
   		printf("Nao foi possivel abrir o arquivo!");

	while(!feof(arquivoEntrada)){
        fscanf(arquivoEntrada, "%s %d %d %d" , &prefixo, &valor1, &valor2, &valor3);
       // printf("prefixo %s \n", prefixo);
       // printf("valor1 %d \n", valor1);
       // printf("valor2 %d \n", valor2);
       // printf("valor3 %d \n", valor3);
        if(strcmp(prefixo, "V") == 0){
            printf("Total de vertices do grafo: %d \n\n", valor1);
            nVertices = valor1;
            valor2 = 0;
			valor3 = 0;
            grafo = new Grafo (valor1+1);

        }
        if(strcmp(prefixo, "E") == 0){

            if(valor1 < raiz){
            	raiz = valor1;
            	//std::cout << "Nova raiz :" << valor1 << endl;
            	//system("pause");
			}
            Grafo::Aresta *a = new Grafo::Aresta (valor1, valor2, valor3);
			grafo->insereAresta (a->_v1 (), a->_v2 (), a->_peso ());
            //grafo->insereAresta (a->_v2 (), a->_v1 (), a->_peso ());
            //fprintf(arquivoSaida, "%s %d %d %d\n", prefixo, valor1, valor2, valor3);
            //fprintf(arquivoSaida,"%d %d %d\n", valor1, valor2, valor3);
            delete a;
        }
	}


	fechaArquivo(arquivoEntrada);
//	fechaArquivo(arquivoSaida);

	printf("terminou de ler  o arquivo \n\n");

    //grafo->imprime ();

    // Variáveis para medir o tempo de execução
    double tf1, tf2;


	// Variáveis para medir o tempo de execução
	std::cout << "Iniciando Dijkstra..." << endl;
	t_inicio = clock(); // Guarda o horário do início da execução
	//StartCounter();

    Dijkstra dj (grafo);
    //dj.obterArvoreCMC(0);
    dj.calculaDijkstraVetorNaoOrdenado(raiz);

	//tf1 = GetCounter();
	t_fim = clock();

    tempo = (float)(t_fim - t_inicio)/CLOCKS_PER_SEC; // Calcula o tempo de execução

    // imprime as menores distancias calculadas
    //imprime(dist, V);
    FILE *arquivoSaida;

	arquivoSaida =fopen("/home/solange/Documentos/TrabalhoAPA2/grafos_5/check/check_v5_s2.dat.out_3", "a");

	//printf("chegou aqui 0 2 \n");
	// Imprime o tempo de execução
	fprintf(arquivoSaida, "\nTempo total de execucao: %f segundos.\n\n", tempo);

	//printf("chegou aqui 0 3 \n");

    for (int i = 0; i < nVertices; ++i){
    	//printf("chegou aqui 0 4 \n");
    	fprintf(arquivoSaida, "Origem: %i \t Destino: %d \t Distância: %d\n", raiz, i, dj._peso(i));
    	//fprintf(arquivoSaida, "%d \t %d\n", i, 1);
	}

    fechaArquivo(arquivoSaida);

    delete grafo;
    return 0;
}
