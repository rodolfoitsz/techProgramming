#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
 
//#define MAP "NNNBBBNN;NBNNNNBB;NBBBBNBB;NNNNBNBN;NBBNBNNN;NBBNNNBN"
#define nrOfConnections 500
#define START 0
#define DESTINATION 47

int showConnections = 1;
int showSteps = 1;
int nrOfNodes;

int ROWS;
int COLUMNS;

char  *MAP ;

// ------ MATH STUFF  ------  //

int ABS(int x) { if (x < 0) return x*(-1); else return x; }
int MAX(int x, int y) { if (x>y) return x; else return y; }
int MIN(int x, int y) { if (x<y) return x; else return y; }

//  ------  END MATH  ------  //


// ------ GRAPH IMPLEMENTATION  ------  //


typedef struct node {
    struct node *next;
    int vertex;
    int weight;
}node;
 
 node *G[300];  
 
 
void insert(int vi,int vj, int w) {
    node *p,*q;
    
    //acquire memory for the new node
    q=(node*)malloc(sizeof(node));
    q->vertex=vj;
    q->weight=w;
    q->next=NULL;
 
    //insert the node in the linked list number vi
    if(G[vi]==NULL)
        G[vi]=q;
    else
    {
        //go to end of the linked list
        p=G[vi];
       
        while(p->next!=NULL)
            p=p->next;
        p->next=q;
    }
}
 
int randomInRange(int minimum_number, int max_number) {	
	return rand() % (max_number - minimum_number) + minimum_number; 
}

int existsConnection(int vi, int vj) {
	if (G[vi] == NULL) 
		return 0;
	else{
		node *p,*q;
        int i;
		
		//put all the connections in a vector
        p=G[vi];
		int nodesConnectedWithVI[1000];		
		int count = 1;
       
        nodesConnectedWithVI[0] = p->vertex;
        while(p->next!=NULL) {
			p=p->next;
			nodesConnectedWithVI[count] = p->vertex;
			count++;
		}
        /*
        printf("node %d has %d sons: ", vi, count);
        for (i=0;i<count;i++)
			printf("%d ", nodesConnectedWithVI[i]);
		printf("\n");
        */

        int found = 0;
        for (i=0;i<count;i++)
			if(nodesConnectedWithVI[i]==vj) 
				found = 1;
	
		return found;
	}
}
 
 
 int countSquaresOnMap(char* map) {
	 int i = 0;
	 int counter = 0;
	 while (map[i] != 0) {
		if (map[i] == 'B' || map[i] == 'N')
			counter++;
		i++;		 
	 }	 
	 return counter;
 }
 int lengthOfASingleLine(char* map) {
	 int i = 0;
	 while (map[i] != ';') { i++; }	 
	 return i;
 }
 int isInsideRange(int nr, int min, int max) { return nr >= min && nr < max; }
 
 void printMatrix(char** matrix, int numberOfLines, int numberColumns) {
	int row, columns;
	for (int row=0; row<numberOfLines; row++)
	{
		for(int columns=0; columns<numberColumns; columns++)
			 printf("%c  ", matrix[row][columns]);
		printf("\n");
	}
 }
 int getNodeID(int row, int column) { return column + row * COLUMNS; }
 void idToPosition(int ID, int *r, int *c) {
	 int R = 0;
	 int C = 0;
	 for (R = 0; R < ROWS; R++)
		for (C = 0; C < COLUMNS; C++)
			if (getNodeID(R,C) == ID) {
				*r = R;
				*c = C;
			}	 
 }
 int evaluate(int r, int c, int destR, int destC) { return MAX(ABS(r - destR), ABS(c - destC)); }
 
 int evaluateNode(int n1, int n2) { 
	int n1R;
	int n1C;
	int n2R;
	int n2C; 
	 
	idToPosition(n1, &n1R, &n1C);
	idToPosition(n2, &n2R, &n2C);
	 return evaluate(n1R, n1C, n2R, n2C);
 }
 
 /*
	El programa recibe una cadena del tipo:
	* 	BBBNBBN;NBBBNBN;..
	* donde cada segmento entre punto y comma es una linea del mapa.
	* El algoritmo transforma esta entrada en un grafo utilizable por el algoritmo
*/
void create_graph(char* map) {
	nrOfNodes = countSquaresOnMap(map);	
    COLUMNS = lengthOfASingleLine(MAP);
    ROWS = nrOfNodes / COLUMNS;
        
    int i,j,k,vi,vj;
    int r,c,r2,c2;
    
    //initialise G[] with a null   
    for(i=0;i<nrOfNodes;i++)
        G[i]=NULL;
    
    printf("creating MAP..\n\n");
        
    // create matrix that will be used to check the neighbourhood
    char **MapMatrix = (char **)malloc(ROWS * sizeof(char*));
	for(char i = 0; i < ROWS; i++) MapMatrix[i] = (char *)malloc(COLUMNS * sizeof(char));    
    
    i = 0;
    for (r = 0; r < ROWS; r++)
		for (c = 0; c < COLUMNS; c++) {
			MapMatrix[r][c] = map[i];
			i++;
			if (map[i] == ';') i++;
		}
        
    printMatrix(MapMatrix, ROWS, COLUMNS);
    
    
    printf("\ncreating connections..\n");
	for (r = 0; r < ROWS; r++)
		for (c = 0; c < COLUMNS; c++)			
			if (MapMatrix[r][c] == 'N')
				for (r2 = r-1; r2 <= r+1; r2++)
					for (c2 = c-1; c2 <= c+1; c2++)												
						if (isInsideRange(r2, 0, ROWS) && isInsideRange(c2, 0, COLUMNS) && !(r2==r && c2==c) && MapMatrix[r2][c2] == 'N') {
							insert(getNodeID(r,c), getNodeID(r2,c2), 1);
							if (showConnections)
								printf("[%d,%d]; ", getNodeID(r,c), getNodeID(r2,c2));
						}			
    
    for(char i = 0; i < ROWS; i++) 
		free(MapMatrix[i]);
	free(MapMatrix);
} 
 
 //  ------  END GRAPH  ------  //
 
 
 
 // ------ LIST IMPLEMENTATION  ------  //
 
 typedef struct nn {
    int val;
    struct nn *next;
    struct nn *prevNodeInPath;
    int costToGetHere;
} node_t;
 
 void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%d ", current->val);
        node_t * prev = current->prevNodeInPath;
        while(prev!=NULL){
			printf("-> %d ", prev->val);
			prev = prev->prevNodeInPath;
		}
		printf("\n");
        current = current->next;
    }
	printf("\n");
}
 
 void push(node_t ** head, node_t *new_node) {
    new_node->next = *head;
    *head = new_node;
}

void pushEnd(node_t **head, node_t *new_node) {    
    if (*head==NULL) 
		push(head, new_node);
    else {
		node_t *current = *head;
		while (current->next != NULL)
			current = current->next;
		
		new_node->next = NULL;
		current->next = new_node;
	}
		
}
 
node_t *pop(node_t ** head) {
    node_t * returningNode;
    node_t * next_node = NULL;
	
    if (*head == NULL) {
        return NULL;
    }

    next_node = (*head)->next;
    returningNode = *head;
    returningNode->next = NULL;
    *head = next_node;

    return returningNode;
}

void freeList(node_t **list) {
	while(*list != NULL){
		node_t *tmp = pop(list);
		free(tmp);
	}
}

int count(node_t **list) {
	 if (*list==NULL) 
		return 0;
    else {
		int counter = 1;
		node_t *current = *list;
		while (current != NULL){
			current = current->next;	
			counter++;
		}	
		return counter - 1;
	}
}
 
 //  ------  END LIST  ------  //
 
 
int DFS(int, int, int*, node_t**);
int BFS(int, int, int*, node_t**);
int BestFS(int, int, int*, node_t**);
 
void beFirstSearch(char *MapArray)
{    	


    MAP=MapArray;

	printf("Want to show the connections? (1:yes / 0:no)");   
    scanf("%d",&showConnections);
	
	printf("Want to algorithms steps? (1:yes / 0:no)");   
    scanf("%d",&showSteps);
	
    int i;
    create_graph(MAP);
	
	
    // ---- DFS ----
    int totWeight = 0;
    node_t *finalList;
    if (DFS(START, DESTINATION, &totWeight, &finalList)) {
		printf("path finded used DFS:\n");    
		print_list(finalList);
		printf("\nTotal weight DFS:%d\n\n", totWeight); 
		freeList(&finalList);
    }
    else
		printf("No path found\n\n");
    
    // --- BFS ----
    totWeight = 0;    
    if (BFS(START, DESTINATION, &totWeight, &finalList)) {
		printf("path finded used BFS:\n");    
		print_list(finalList);
		printf("\nTotal weight BFS:%d\n\n", totWeight); 
		freeList(&finalList);
    }
    else
		printf("No path found\n\n");       
    
    // ---- BestFS ----
    totWeight = 0;    
    if (BestFS(START, DESTINATION, &totWeight, &finalList)) {
		printf("path finded used BestFS:\n");    
		print_list(finalList);
		printf("\nTotal weight BestFS:%d\n\n", totWeight); 
		freeList(&finalList);
    }
    else
		printf("No path found\n\n");
    
    
    printf("\n");
}
 
 int existsInTheArray(int val, int *array, int dim) {
	 int found = 0;
	 int i;
	 for(i = 0; i < dim; i++)
		if (array[i] == val)
			found = 1;
	 return found;	 
 }
 
int BestFS(int initial, int final, int* weight, node_t** finalList) {

 /*
	procedure Búsqueda_en_profundidad {
	  open  [estado_inicial]
	  closed  {}
	  while (open no está vacía) {
	    remover el primer estado X de la lista open
	    if (X es un estado objetivo) return éxito
	    else {
	      generar el conjunto de sucesores del estado X
	      agregar el estado X al conjunto closed
	      eliminar sucesores que ya están en open o en closed
	      agregar el resto de los sucesores a open poniendo el MEJOR al principio de la lista
	    }
	  }
	  return fracaso
	}
 */
	printf("\n\n --- BestFS START --- \n\n");
	printf("Searching path from %d to %d\n", initial, final);
	
	//open = [estado_inicial]
	node_t * initialNode = malloc(sizeof(node_t));	
	initialNode->val = initial;
	initialNode->next = NULL;
	initialNode->prevNodeInPath = NULL;
	initialNode->costToGetHere = 0;	
	node_t *open = NULL;
	push(&open, initialNode);
	
	//closed  {}
	node_t* closed = NULL;	
		
	while(open != NULL) {
		
		//remover el primer estado X de la lista open
		int totW = open->costToGetHere;
		node_t *actual = pop(&open);		
		if (showSteps){
			printf("\nManaging %d\n", actual->val);
		}
		//if (X es un estado objetivo) return éxito
		if (actual->val == final) {
			printf("DESTINATION FOUND!!\n\n");
			*finalList = NULL;	
			push(finalList, actual);
			actual = actual->prevNodeInPath;
			while(actual!=NULL) {				
				push(finalList, actual);				
				actual = actual->prevNodeInPath;
			}
			*weight = totW;
			return 1;
		}			
		else {
			
			//agregar el estado X al conjunto closed
			push(&closed, actual);	
			if (showSteps){
				printf("\nCLOSED:\n");    
				print_list(closed);			
			}
			//generar el conjunto de sucesores del estado X			
			node_t* tmpList = NULL;
			int counter = 0;	

			node *p = G[actual->val];
			while(p!=NULL) {				
				node_t * new_node = malloc(sizeof(node_t));				
				new_node->val = p->vertex;
				new_node->next = tmpList;
				new_node->prevNodeInPath = closed;
				new_node->costToGetHere = closed->costToGetHere + p->weight;
				tmpList = new_node;
										
				p=p->next;
				counter++;
			}	
			if (showSteps){			
				printf("TMP:\n");    
				print_list(tmpList);
			}
			//eliminar sucesores que ya están en open o en closed
			int valuesAlreadyListed[nrOfNodes];	
			int k;					
			for (k = 0; k < nrOfNodes; k++) valuesAlreadyListed[k] = -1;								
			k = 0;
			
			node_t *nodeAlreadyInTheList = open;
			while(nodeAlreadyInTheList != NULL) {
				valuesAlreadyListed[k] = nodeAlreadyInTheList->val;
				//printf("val: %d\n", nodeAlreadyInTheList->val);
				nodeAlreadyInTheList = nodeAlreadyInTheList->next;
				k++;				
			}
			nodeAlreadyInTheList = closed;
			while(nodeAlreadyInTheList != NULL) {
				valuesAlreadyListed[k] = nodeAlreadyInTheList->val;
				//printf("val: %d\n", nodeAlreadyInTheList->val);
				nodeAlreadyInTheList = nodeAlreadyInTheList->next;
				k++;				
			}
			
			/*int u;
			printf("values already presents in the lists OPEN and CLOSED:\n");
			for (u=0; u< k; u++)
				printf("%d,", valuesAlreadyListed[u]);
			printf("\n");
			//sleep(1);	*/
			
			node_t *cleanList = NULL;
			while(tmpList!=NULL) {
				node_t *bufferNode = pop(&tmpList);
				if (!existsInTheArray(bufferNode->val, valuesAlreadyListed, k))
					pushEnd(&cleanList, bufferNode);
				else
					free(bufferNode);
			}
				

			tmpList = cleanList;
			if (showSteps){
				printf("CLEAN LIST:\n");    
				print_list(tmpList);
			}
			//sleep(1);
			

			
			//agregar el resto de los sucesores a open poniendo el MEJOR al principio de la lista

			int totSize = count(&tmpList) + count(&open);
			node_t **NodeArray = (node_t**)malloc(totSize * sizeof(node_t*));
			int indx = 0;
			
				//filling tmpArray
			while(tmpList!=NULL)			
				NodeArray[indx++] = pop(&tmpList);
			while(open!=NULL)			
				NodeArray[indx++] = pop(&open);
			
				
				// sorting array
			node_t* swap;
			int w,z;
			for (w = 0 ; w < totSize - 1; w++)
				for (z = 0 ; z < totSize - w - 1; z++){
					int valN1 = evaluateNode((NodeArray[z])->val, DESTINATION);
					int valN2 = evaluateNode((NodeArray[z+1])->val, DESTINATION);					
					if (valN1 > valN2) 
					{
						swap       = NodeArray[z];
						NodeArray[z]   = NodeArray[z+1];
						NodeArray[z+1] = swap;
					}
				}				
				
				// placing in open list
			for (indx = 0; indx < totSize; indx++) 
				pushEnd(&open, NodeArray[indx]);			
				
			if (showSteps){
				printf("OPEN:\n");    
				print_list(open);				
			}
		}		
		
	}
	return 0;
}

int DFS(int initial, int final, int* weight, node_t** finalList) {

 /*
	procedure Búsqueda_en_profundidad {
	  open  [estado_inicial]
	  closed  {}
	  while (open no está vacía) {
	    remover el primer estado X de la lista open
	    if (X es un estado objetivo) return éxito
	    else {
	      generar el conjunto de sucesores del estado X
	      agregar el estado X al conjunto closed
	      eliminar sucesores que ya están en open o en closed
	      agregar el resto de los sucesores al principio de open
	    }
	  }
	  return fracaso
	}
 */
	printf("\n\n --- DFS START --- \n\n");

	//open = [estado_inicial]
	node_t * initialNode = malloc(sizeof(node_t));	
	initialNode->val = initial;
	initialNode->next = NULL;
	initialNode->prevNodeInPath = NULL;
	initialNode->costToGetHere = 0;	
	node_t *open = NULL;
	push(&open, initialNode);
	
	//closed  {}
	node_t* closed = NULL;	
		
	while(open != NULL) {
		
		//remover el primer estado X de la lista open
		int totW = open->costToGetHere;
		node_t *actual = pop(&open);		
		if (showSteps){
			printf("\nManaging %d\n", actual->val);
		}
		//if (X es un estado objetivo) return éxito
		if (actual->val == final) {
			printf("DESTINATION FOUND!!\n\n");
			*finalList = NULL;	
			push(finalList, actual);
			actual = actual->prevNodeInPath;
			while(actual!=NULL) {				
				push(finalList, actual);				
				actual = actual->prevNodeInPath;
			}
			*weight = totW;
			return 1;
		}			
		else {
			
			//agregar el estado X al conjunto closed
			push(&closed, actual);	
			if (showSteps){
				printf("\nCLOSED:\n");    
				print_list(closed);			
			}
			//generar el conjunto de sucesores del estado X			
			node_t* tmpList = NULL;
			int counter = 0;	

			node *p = G[actual->val];
			while(p!=NULL) {				
				node_t * new_node = malloc(sizeof(node_t));				
				new_node->val = p->vertex;
				new_node->next = tmpList;
				new_node->prevNodeInPath = closed;
				new_node->costToGetHere = closed->costToGetHere + p->weight;
				tmpList = new_node;
										
				p=p->next;
				counter++;
			}	
			if (showSteps){			
				printf("TMP:\n");    
				print_list(tmpList);
			}
			//eliminar sucesores que ya están en open o en closed
			int valuesAlreadyListed[nrOfNodes];	
			int k;					
			for (k = 0; k < nrOfNodes; k++) valuesAlreadyListed[k] = -1;								
			k = 0;
			
			node_t *nodeAlreadyInTheList = open;
			while(nodeAlreadyInTheList != NULL) {
				valuesAlreadyListed[k] = nodeAlreadyInTheList->val;
				//printf("val: %d\n", nodeAlreadyInTheList->val);
				nodeAlreadyInTheList = nodeAlreadyInTheList->next;
				k++;				
			}
			nodeAlreadyInTheList = closed;
			while(nodeAlreadyInTheList != NULL) {
				valuesAlreadyListed[k] = nodeAlreadyInTheList->val;
				//printf("val: %d\n", nodeAlreadyInTheList->val);
				nodeAlreadyInTheList = nodeAlreadyInTheList->next;
				k++;				
			}
			
			/*int u;
			printf("values already presents in the lists OPEN and CLOSED:\n");
			for (u=0; u< k; u++)
				printf("%d,", valuesAlreadyListed[u]);
			printf("\n");
			//sleep(1);	*/
			
			node_t *cleanList = NULL;
			while(tmpList!=NULL) {
				node_t *bufferNode = pop(&tmpList);
				if (!existsInTheArray(bufferNode->val, valuesAlreadyListed, k))
					pushEnd(&cleanList, bufferNode);
				else
					free(bufferNode);
			}
				

			tmpList = cleanList;
			if (showSteps){
				printf("CLEAN LIST:\n");    
				print_list(tmpList);
			}
			//sleep(1);
			
			//agregar el resto de los sucesores al principio de open
			while(tmpList!=NULL)			
				push(&open, pop(&tmpList));			
			if (showSteps){
				printf("OPEN:\n");    
				print_list(open);				
			}
		}		
		
	}
	return 0;
}

int BFS(int initial, int final, int* weight, node_t** finalList) {

 /*
	procedure Búsqueda_en_profundidad {
	  open  [estado_inicial]
	  closed  {}
	  while (open no está vacía) {
	    remover el primer estado X de la lista open
	    if (X es un estado objetivo) return éxito
	    else {
	      generar el conjunto de sucesores del estado X
	      agregar el estado X al conjunto closed
	      eliminar sucesores que ya están en open o en closed
	      agregar el resto de los sucesores al final de open
	    }
	  }
	  return fracaso
	}
 */
	printf("\n\n --- BFS START --- \n\n");

	//open = [estado_inicial]
	node_t * initialNode = malloc(sizeof(node_t));	
	initialNode->val = initial;
	initialNode->next = NULL;
	initialNode->prevNodeInPath = NULL;
	initialNode->costToGetHere = 0;	
	node_t *open = NULL;
	push(&open, initialNode);
	
	//closed  {}
	node_t* closed = NULL;	
		
	while(open != NULL) {
		
		//remover el primer estado X de la lista open
		int totW = open->costToGetHere;
		node_t *actual = pop(&open);		
		if (showSteps){
			printf("\nManaging %d\n", actual->val);
		}
		//if (X es un estado objetivo) return éxito
		if (actual->val == final) {
			printf("DESTINATION FOUND!!\n\n");
			*finalList = NULL;	
			push(finalList, actual);
			actual = actual->prevNodeInPath;
			while(actual!=NULL) {				
				push(finalList, actual);				
				actual = actual->prevNodeInPath;
			}
			*weight = totW;
			return 1;
		}			
		else {
			
			//agregar el estado X al conjunto closed
			push(&closed, actual);	
			if (showSteps){
				printf("\nCLOSED:\n");    
				print_list(closed);			
			}
			//generar el conjunto de sucesores del estado X			
			node_t* tmpList = NULL;
			int counter = 0;	

			node *p = G[actual->val];
			while(p!=NULL) {				
				node_t * new_node = malloc(sizeof(node_t));				
				new_node->val = p->vertex;
				new_node->next = tmpList;
				new_node->prevNodeInPath = closed;
				new_node->costToGetHere = closed->costToGetHere + p->weight;
				tmpList = new_node;
										
				p=p->next;
				counter++;
			}		
			if (showSteps){		
				printf("TMP:\n");    
				print_list(tmpList);
			}
			//eliminar sucesores que ya están en open o en closed
			int valuesAlreadyListed[nrOfNodes];	
			int k;					
			for (k = 0; k < nrOfNodes; k++) valuesAlreadyListed[k] = -1;								
			k = 0;
			
			node_t *nodeAlreadyInTheList = open;
			while(nodeAlreadyInTheList != NULL) {
				valuesAlreadyListed[k] = nodeAlreadyInTheList->val;
				//printf("val: %d\n", nodeAlreadyInTheList->val);
				nodeAlreadyInTheList = nodeAlreadyInTheList->next;
				k++;				
			}
			nodeAlreadyInTheList = closed;
			while(nodeAlreadyInTheList != NULL) {
				valuesAlreadyListed[k] = nodeAlreadyInTheList->val;
				//printf("val: %d\n", nodeAlreadyInTheList->val);
				nodeAlreadyInTheList = nodeAlreadyInTheList->next;
				k++;				
			}
			
			/*int u;
			printf("values already presents in the lists OPEN and CLOSED:\n");
			for (u=0; u< k; u++)
				printf("%d,", valuesAlreadyListed[u]);
			printf("\n");
			//sleep(1);	*/
			
			node_t *cleanList = NULL;
			while(tmpList!=NULL) {
				node_t *bufferNode = pop(&tmpList);
				if (!existsInTheArray(bufferNode->val, valuesAlreadyListed, k))
					pushEnd(&cleanList, bufferNode);
				else
					free(bufferNode);
			}
				

			tmpList = cleanList;
			if (showSteps){
				printf("CLEAN LIST:\n");    
				print_list(tmpList);
			}
			//sleep(1);
			
			//agregar el resto de los sucesores al final de open
			while(tmpList!=NULL)			
				pushEnd(&open, pop(&tmpList));			

			if (showSteps){
				printf("OPEN:\n");    
				print_list(open);				
			}			
		}		
		
	}
	return 0;
}
