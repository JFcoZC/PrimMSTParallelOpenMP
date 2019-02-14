//Librerias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

int NUMTHREADS = 5;
int VERTICES = 0;
int MAXCHAR = 300;		//Hasta 300 chars por linea EN ARCHIVO
int MAXSTR = 100;		//Hasta 100 lineas por archivo

//Funciones
//--------------------------------------------------------------
char **readFile(char* filename)
{
	int stringActual;
	FILE *fp;
	char str[MAXCHAR];		//Linea actual
	char* strsInFile[MAXSTR];		
	
	stringActual = 0;
	VERTICES = 0;
	
	//Abrir archivo para lectura
	fp = fopen(filename, "r");
	
	//No se encontro archivo
	if(fp == NULL)
	{
		printf("No se pudo abrir el archivo %s \n\n", filename);
	}//Fin if 1
	else
	{
		while(fgets(str, MAXCHAR, fp) != NULL)
		{
			//----------------------
			//Linea por linea que se va leyendo del archivo
			//printf("%s\n", str);
			//----------------------
			
			//***strsInFile[stringActual] = str;
			//1)Asignar la memoria suficiente para cada puntero (Cada linea)
			strsInFile[stringActual] = (char *) malloc(MAXCHAR);
			//2)Copiar el numero de bytes indicado por MAXCHAR de str a strsInFile[stringActual]
			//recordar que en c cada caracter ocupa un byte
			memcpy(strsInFile[stringActual],str, MAXCHAR);
		
			//--------------------------
			//printf("a) %s %x \n", str, &str);
			//printf("b) %s %x \n", strsInFile[stringActual], &strsInFile[stringActual]);
			//--------------------------
			
			stringActual++;
			VERTICES++;
		}//FIN WHILE
		
	}//Fin else 1
	
	fclose(fp);
	
	//Regresar arreglo de arreglos/punteros que apuntatan a otros punteros
	return strsInFile;
	
}//FIn funcion readFile
//--------------------------------------------------------------
int** getGraph()
{
	char **filasArchivo;
	int filaActual;
	int colActual;
	char *filaActualFile;
	char* tokenizedstr;
	int actualNum;
	int** graph;

	//Guardar filas leidas del archivo en un arreglo de strings, donde cada fila es una string
	//100 lineas se guardan en 800 bits .:. 1 linea/fila ocupa 8 bits
	filasArchivo = (char**) malloc(800);
	memcpy(filasArchivo, readFile("matrizPruebaDos.txt"), 800);
	
	printf("Numero de vertices en matriz de archivo: %i \n", VERTICES);
	//printf("Size of int: %i \n", sizeof(int));
	//int graph[VERTICES][VERTICES];	//Declarar despues de que se manda a llamara readFile()
	filaActual = 0;
	colActual = 0;
	
	//Asignar espacio de memoria para el numero total de filas; 1 FILA OCUPA: (SizeOfIntPointer)
	//Todas las filas/Arreglo de apuntadores = (FILAS)*(SizeOfIntPointer)
	graph = (int **)malloc( (VERTICES)*sizeof(int*));
	
	//Asignar el espacio para cada fila; cada fila ocupa NumeroColumnas*EspacioMemoria. 1 Celda ocupa	= (SizeOfInt)
	//.:. Espacio ne cada fila = NumeroColumnas*(SizeOfInt)

	for(int k = 0; k  < VERTICES; k++)
	{
		//Espacio en fila j = NumeroColumnas*(SizeOfInt)
		graph[k] = (int*)malloc(VERTICES*sizeof(int));
		
	}//Fin for 0
	
	//Llenar la matriz con los numeros leidos del archivo
	for(int i = 0; i < VERTICES; i++)
	{
		
		//Fila de achivo actual
		filaActualFile = filasArchivo[i];
		//-------
		//printf("Fila actual: %s \n", filaActualFile);
		//-------
		
		//Tokenizacion de cada linea del archivo
		tokenizedstr = strtok(filaActualFile, " ");	//Primera llamada strok	
		actualNum = atoi(tokenizedstr);				//Convert str to int
		//-----
		//printf("Actual line number first %i \n", actualNum);
		//-----
		graph[i][0] = actualNum;
		
		for(int j = 1; j < VERTICES; j++)
		{
			//Tokenizacion de cada linea del archivo
			tokenizedstr = strtok(NULL, " ");	//Pasar NULL para continuar tokenizando a partir de la string pasada en la primera llamada
			actualNum = atoi(tokenizedstr);		//Convert str to int
			graph[i][j] = actualNum;
			
		}//Fin for 2
		
		
	}//Fin for 1
	
	//lIBERAR ESPACIO
	free(filasArchivo);
	filasArchivo = NULL;

	//---------------------
	//printMatrix(graph);
	//---------------------
	
	return graph;
	
}//Fin funcion get graph
//--------------------------------------------------------------
//Function that returns the number of the vertex with the shortest distance that has
//not been visited before
int minimumVertex(int* weights, int* alreadyVisited)
{
	int actualMinWeight = 99999;
	int minimumVertex;
	int iNumThreads;
	
	#pragma omp parallel num_threads(NUMTHREADS)
	{
		iNumThreads = omp_get_num_threads();
		
		#pragma omp for
		for(int i = 0; i < VERTICES; i++)
		{
			//If the vertex has not been visited and its weight/distance is shorted than the
			//actual minimum weight
			if( (alreadyVisited[i] == 0) && (weights[i] < actualMinWeight) )
			{
				//New minimum vertex found
				actualMinWeight = weights[i];
				minimumVertex = i;
				
			}//Fin if 
			
			
		}//Fin for 1		
		
	}//Fin omp paralle_get_num_threads
	
	
	return minimumVertex;
	
}//Fin minimumVertex
//---------------------------------------------------------------------------
void paralelPrimMST()
{
	//INITIALIZE MATRIX GRAPH AND ALSO VERTICES VARIABLE
	int **graph;
	graph = getGraph();
	
	int vertexVisitedFrom[VERTICES];
	int weights[VERTICES];
	int numThreads;
	int alreadyVisitedVertices[VERTICES]; //1=Visited 0 = NOT visited

	//Initialize arrays; saying that any vertice has been visited
	for(int i = 0; i < VERTICES; i++)
	{
		weights[i] = 99999;
		alreadyVisitedVertices[i] = 0;
		
	}//Fin for
	
	//Set Start vertex as vertex n° 0
	weights[0] = 0;
	vertexVisitedFrom[0] = -1; //Vertex 0 visited from 0 because it is the start VERTEX
	
	for(int j = 0; j < VERTICES-1; j++)
	{
		int actualMinVertex = minimumVertex(weights,alreadyVisitedVertices); //Min local solution
		alreadyVisitedVertices[actualMinVertex] = 1; //Mark as visited
		
		int k;
		
		#pragma omp parallel num_threads(NUMTHREADS)
		{
			numThreads = omp_get_num_threads();
			
			#pragma omp for
			for(k = 0; k < VERTICES; k++)
			{
				//If actual k verte has not been alreadyvisited by any thread = alreadyVisitedVertices[k]
				//and there is a valid path to k from actualVertex (weight different from 0) = graph[actualMinVertex][k] = (graph[actualMinVertex][k] != 0)
				//and the actual weight to vertex k is lower than actual weight to that vertex
				if( (graph[actualMinVertex][k] && (alreadyVisitedVertices[k] == 0) ) && (graph[actualMinVertex][k] < weights[k]) )
				{
					#pragma omp critical
					{
						//actualMinVertex is a new best vertex to go to vertex k
						vertexVisitedFrom[k] = actualMinVertex; 
						//update weights with the value calculated from actualMinVertex to vertex k in order all the threads know which is the best
						//weight to go to vertex k
						weights[k] = graph[actualMinVertex][k];  //d) Add k to Weights if edge to k is a valid edge (Not 0) 
						
					}//Fin critical pragma
					
				}//Fin if
				
			}//Fin for 3
			
		}//Fin pragma 1
		
	}//Fin for 2
	
	//Imprimir resultado
	for(int l = 0; l < VERTICES; l++)
	{
		printf("De: %i ir a %i \n", vertexVisitedFrom[l], l);
		
	}//Fin for 1
	printf("FIN!\n");
	
}//Fin paralelPrimMST


//Inicio del progrma/main
int main()
{
	//Declaracion de variables
	
	//Datos de entrada
	
	//Proceso
	paralelPrimMST();
	
	//Datos de salida
	
	//FIn del programa
	puts("Fin del programa v1");
	
	//Detener la pantalla
	scanf("%");
	
	//Indicar al SO que todo termina OK
	return 0;
	
}//Fin main

