//Librerias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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
//-----------------------------------------------------------------
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
//-------------------------------------------------------------
int** generateRandomGraph(int numVertices)
{
	int **randomGraph;
	int randomValue;
	
	VERTICES = numVertices;
	
	//Asignar espacio de memoria que ocupa cada fila. Ya que cada fila es un arreglo de apuntadores
	//de enteros el espacio que se reserva en cada fila/renglon = (SizeOfPointerInt*)
	//.:. Espacio a reservar para todas las fila = NumeroFilas*(SizeOfPointerInt*)
	randomGraph = (int **)malloc( (numVertices)*sizeof(int*));
	
	//Asignar el espacio para cada elemento dentro de cada fila. Cada fila tiene k columnas
	//.:. Para cada fila j reservar espacio = NumeroColumnas*(SizeOfInt)
	for(int k = 0; k  < VERTICES; k++)
	{
		randomGraph[k] = (int*)malloc(numVertices*sizeof(int));
		
	}//Fin for 1
	
	//Generar numeros aleatorios del 0 al 20 y llenar con ellos todos los espacios en matriz
	for(int i = 0; i < numVertices; i++)
	{
		for(int j = 0; j < numVertices; j++)
		{
			//srand(time(NULL));	//Seed to generate random numbers
			randomValue = rand() % 20;
			
			//Los valores que den 0 se guardan como INFINITO
			if(randomValue == 0)
			{
				randomValue = 9999;
			}//Fin if
			
			//Hacer que los valores de los caminos sean los mismos
			//en ambas direcciones
			randomGraph[i][j] = randomValue;
			randomGraph[j][i] = randomValue; //Transpuesta de matriz
			
		}//Fin for 3
		
	}//Fin for 2
	
	//Asegurarse que la diagonal principal de la matriz es de puros 0s(Los vertices no pueden tener camino asi mismos)
	for(int l = 0; l < numVertices; l++)
	{
		randomGraph[l][l] = 0;
		
	}//FIn for 4
	
	return randomGraph;
	
}//FIn metodo generateRandomGraph
//--------------------------------------------------------------
void writeFile(char* filename, char* content)
{
	FILE *fp;
	int error;
	
	//Open a text file for append stream to the end
	//If there is no file, it is created
	fp = fopen(filename,"a+");
	

	//Write content
	error = fputs(content,fp);
	if( error  == EOF )
	{
		printf("Error al escribir en archivo!\n");
		printf("ERROR: %i = %s\n", error ,strerror(errno));
		//clearerr(fp);
		//fflush(stdout);
		//Llamada recursiva para ver si ya se hace la escritura sin error
		//writeFile(filename,content);
		
	}//Fin if 1
	
	//Cerrar archivo
	fclose(fp);
	
}//Fin writeFile
//----------------------------------------------------
void printMatrix( int **matrix )
{
	for(int i = 0; i < VERTICES; i++)
	{
		
		printf("(%2i) ", i);
		
		for(int j = 0; j < VERTICES; j++)
		{
			printf("%4i ", matrix[i][j]);
			
		}//Fin for 2
		
		printf("\n");
		
	}//Fin for 1
	
	
}//Fin funcion printMatrix
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
void paralelPrimMST(int NumVertices)
{
	//INITIALIZE MATRIX GRAPH AND ALSO VERTICES VARIABLE
	int **graph;
	//graph = getGraph();
	graph = generateRandomGraph(NumVertices);
	
	//---- Imprimir matriz generada automaticamente ---
	//printMatrix(graph);
	//-------------------------------------------------
	
	int vertexVisitedFrom[VERTICES];
	int weights[VERTICES];
	int numThreads;
	int alreadyVisitedVertices[VERTICES]; //1=Visited 0 = NOT visited
	int randomRoot;
	
	char procesadores[10];
	char strtottime[15];
	double starttime;
	double endtime;
	double totaltime;

	//Initialize arrays; saying that any vertice has been visited
	for(int i = 0; i < VERTICES; i++)
	{
		weights[i] = 99999;
		alreadyVisitedVertices[i] = 0;
		
	}//Fin for
	
	//Set Start vertex as random
	srand(time(NULL));	//Seed to generate random numbers
	randomRoot = rand() % VERTICES;
	weights[randomRoot] = 0;
	vertexVisitedFrom[randomRoot] = -1; //Vertex 0 visited from 0 because it is the start VERTEX
	
	//--------- INICIO ALGORITMO PARALELO ------
	starttime = omp_get_wtime();
	
	for(int j = 0; j < VERTICES-1; j++)
	{
		int actualMinVertex = minimumVertex(weights,alreadyVisitedVertices); 	//MIN LOCAL SOLUTION
		alreadyVisitedVertices[actualMinVertex] = 1; //Mark as visited			//MIN LOCAL SOLUTION
		
		int k;
		
		#pragma omp parallel num_threads(NUMTHREADS)
		{
			numThreads = omp_get_num_threads();
			
			#pragma omp for
			for(k = 0; k < VERTICES; k++)
			{
				//---------- MIN GLOBAL SOLUTION --------
				//If actual k verte has not been alreadyvisited by any thread = alreadyVisitedVertices[k]
				//and there is a valid path to k from actualVertex (weight different from 0) = graph[actualMinVertex][k] = (graph[actualMinVertex][k] != 0)
				//and the actual weight to vertex k is lower than actual weight to that vertex
				if( (graph[actualMinVertex][k] && (alreadyVisitedVertices[k] == 0) ) && (graph[actualMinVertex][k] < weights[k]) )
				{
					
					#pragma omp critical
					{
						//-------- BROADCAST NEW VERTEX ADED TO MST -------------
						//actualMinVertex is a new best vertex to go to vertex k
						vertexVisitedFrom[k] = actualMinVertex; 
						
						//-------- LOCAL UPDATE OF WeightsToEachVertex MADE BY EACH PROCESSOR -------------
						//update weights with the value calculated from actualMinVertex to vertex k in order all the threads know which is the best
						//weight to go to vertex k
						weights[k] = graph[actualMinVertex][k];  //d) Add k to Weights if edge to k is a valid edge (Not 0) 
						
					}//Fin critical pragma
					
				}//Fin if
				
			}//Fin for 3
			
		}//Fin pragma 1
		
	}//Fin for 2
	//-------- FIN ALGORITMO PARALELO ----------
	
	endtime = omp_get_wtime();
	totaltime = endtime - starttime;
	
	//----- Imprimir resultado ------------
	/*for(int l = 0; l < VERTICES; l++)
	{
		printf("De: %i ir a %i \n", vertexVisitedFrom[l], l);
		
	}//Fin for 1*/
	//---------------------------------------
	printf("\nRESULTADO PARALLEL VERSION MST en %f segundos: \n",totaltime);
	
	//--- Escribir resultados en archivo txt ---
	//Convertir entero de numero de procesadores a cadena
	//Convertir int to char itoa(value,string,base)
	itoa(NUMTHREADS,procesadores,10); //+1 para no usar 0 en calendario y llegar a ultimo dia
	
	//Convert double to string
	sprintf(strtottime,"%f",totaltime);
	
	//Append en archivo de resultados
	writeFile("resultados.txt",procesadores);
	writeFile("resultados.txt"," ");
	writeFile("resultados.txt",strtottime);
	writeFile("resultados.txt","\n");
	
}//Fin paralelPrimMST


//Inicio del progrma/main
int main()
{
	//Declaracion de variables
	
	//Datos de entrada
	NUMTHREADS = 8; 		//Numero de procesos
	
	//Proceso
	paralelPrimMST(14000);	//Numero de vertices grafo
	
	//Datos de salida
	
	//FIn del programa
	puts("Fin del programa v1");
	
	//Detener la pantalla
	scanf("%");
	
	//Indicar al SO que todo termina OK
	return 0;
	
}//Fin main

