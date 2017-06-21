#define MAXN 100000000
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include "Lista.h"

using std::cout;
using std::endl;
using std::ostream;
using autoreferencia::Lista;

// Manipulação de arquivos
FILE* abreArquivo(char modo, char caminho[100])
{
    FILE *arquivo;
    switch(modo)
    {
        case 'g':
            arquivo = fopen(caminho, "wt");
            break;
        case 'l':
            arquivo = fopen(caminho, "rt");
            break;
        case 'a':
            arquivo = fopen(caminho, "a");
            break;
    }
    if(arquivo == NULL)
	{
        printf("Nao foi possivel abrir o arquivo.");
        exit(0);
	}
	return arquivo;
}

void fechaArquivo(FILE* arquivo)
{
    fclose(arquivo);
}

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
	Elemento vet[MAXN]; //vetor  de tamanho mamximo definido contendo um Elemento(vertice, peso)

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
  		Elemento minElem, temp;
		temp = (Elemento)this->vet[0]; //configura o primeiro elemento como o menor peso
		menorP = temp._peso();
		posMenorP = 0;

		minElem = temp;
		for(int i= 1; i< this->tamanho; i++){
	    	temp = this->vet[i];
    		int pesoAtual =temp._peso();
	    	if(menorP > pesoAtual){
	      		menorP = pesoAtual;
	      		minElem = temp;
	      		posMenorP = i;
	    	}
	  	}
	  	for (int i=posMenorP; i<this->tamanho-1; i++ ){//retira o elemento mínimo e move os próximos para a sua posição
	  		this->vet[i]= this->vet[i+1];

		}
		this->tamanho--;
		return minElem;
	}

	void diminuiChave(int dado, int prior){
		Elemento temp;
		if (this->tamanho < 1)
			throw logic_error ("Erro: fila vazia");
		temp = this->vet[0];
		if (prior < 0)
	    	throw logic_error ("Erro: chaveNova com valor incorreto");
		for(int i= 0; i< this->tamanho; i++){
	    	temp = this->vet[i]; //
	    	if(dado == temp._vertice()){
				if(prior < this->vet[i]._peso())
	      			this->vet[i].atribuiPeso(prior);
            }
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
	        this->vertice = cel.vertice;
	        this->peso = cel.peso;
	        return *this;
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
	    // Retorna a primeira aresta que o vertice v participa ou null se a lista de adjacencia de v for vazia
	    Celula *item = this->adj[v]._primeiro();
	    return item != NULL ? new Aresta(v,item->vertice,item->peso) : NULL;
    }

	Aresta *proxAdj (int v) {
	    // Retorna a proxima aresta que o vertice v participa ou NULL se a lista de adjacencia de v estiver no fim
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
		int n = this->grafo->_numVertices();

	    if (this->p)
			delete [] this->p;

	    this->p = new int[n];
	    int *vs = new int[n];

	    if (this->antecessor)
			delete [] this->antecessor;
	    this->antecessor = new int[n];

	    for (int u = 0; u < n; u ++) {
	      this->antecessor[u] = -1;
	      p[u] = INT_MAX;
	      vs[u] = u;
	    }

	    p[raiz] = 0;
	    FPNaoOrdenado *fila = new FPNaoOrdenado();

		int valorVertice;
	    int valorPeso;
	    for (int i = 0; i<n; i++){
	    	valorVertice = (int) vs[i];
	    	valorPeso = (int)p[i];
	    	fila->insere(valorVertice,valorPeso);
		}
	    while (!fila->vazio()){

	      	int u = fila->remove()._vertice(); //u é o número do vertice extraido

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

  	~Dijkstra () {
  		this->grafo = NULL;
    	if (this->p)
			delete [] this->p;
  		if (this->antecessor)
		  	delete [] this->antecessor;
  }

};




//Programa main
int main(){


	FILE *arquivoEntrada;

    char prefixo[10];
    char caminho[100];
    int valor1, valor2, valor3;

    int nVertices = 0;
	int nArestas = 0;
	int raiz = INT_MAX;

    // Variáveis para medir o tempo de execução
    float tempo;
    clock_t t_inicio, t_fim;

	Grafo *grafo = new Grafo (nVertices+1);

    printf("\nDigite o nome do arquivo que contém o grafo: ");
    scanf("%s", &caminho);
    arquivoEntrada = abreArquivo('l', caminho);

    // Prepara o nome do arquivo de saída
	char *nomeArquivoSaida;
    // Renomeia arquivo de saída para não sobrescrever a entrada
    nomeArquivoSaida = strncat(caminho, ".out", 4);

	if(arquivoEntrada == NULL)
   		printf("Nao foi possivel abrir o arquivo!");

	while(!feof(arquivoEntrada)){
        fscanf(arquivoEntrada, "%s %d %d %d" , &prefixo, &valor1, &valor2, &valor3);

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
		    }
            Grafo::Aresta *a = new Grafo::Aresta (valor1, valor2, valor3);
			grafo->insereAresta (a->_v1 (), a->_v2 (), a->_peso ());
            //grafo->insereAresta (a->_v2 (), a->_v1 (), a->_peso ());
            delete a;
        }
	}


	fechaArquivo(arquivoEntrada);

	//printf("Digite o nó de origem entre 0 e %d :\n\n", nVertices);
	//scanf("%i", &raiz);

	printf("Calculando Dijkstra...\n\n");

	t_inicio = clock(); // Guarda o horário do início da execução

    Dijkstra dj (grafo);

    dj.calculaDijkstraVetorNaoOrdenado(raiz);

	t_fim = clock(); // Guarda o horário do fim da execução

    tempo = (float)(t_fim - t_inicio)/CLOCKS_PER_SEC; // Calcula o tempo de execução

    FILE *arquivoSaida;
    arquivoSaida = abreArquivo('a', nomeArquivoSaida);

	fprintf(arquivoSaida, "\nTempo total de execucao: %f segundos.\n\n", tempo);

    for (int i = 1; i <= nVertices; ++i){
    	fprintf(arquivoSaida, "Origem: %i \t Destino: %d \t Distância: %d\n", raiz, i, dj._peso(i));
	}

    fechaArquivo(arquivoSaida);

    printf("Dijkstra calculado para o grafo. Arquivo com os resultados gerado.\n\n\n");

    delete grafo;
    return 0;
}
