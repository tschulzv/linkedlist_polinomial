/* Universidad Nacional de Itapua.
   Proyecto 2 - Algoritmos y Estructuras de Datos I
   Listas y Polinomios
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* TRUE/OK  o FALSE/ERROR */
typedef char BOOLEAN;

#define TRUE 1
#define FALSE 0

#define ERROR FALSE
#define OK TRUE

#define CONFIRM_TRUE(v, retval) \
    if (!(v)) { \
        printf("Error en linea %u de archivo %s\n", __LINE__, __FILE__); \
        system("pause"); \
        return (retval); \
    }

#define CONFIRM_NOTNULL(v, retval) \
    if (NULL == v) { \
        printf("Error en linea %u de archivo %s\n", __LINE__, __FILE__); \
        system("pause"); \
        return (retval); \
    }

/************************************************************
        L I S T A   C O N   L I S T A S   L I G A D A S
/***********************************************************/

/* La estructura de la lista */
typedef struct _LList {
    void* value; // almacenará un monomio
    struct _LList* sig;
} LList;

typedef struct _Monomial {
    int degree; // grado del monomio
    int coef;   // coeficiente del monomio
} Monomial;


/* Crea la lista 
    Retorna NULL si hubo algun error (ej: no hay suficiente memoria)
    Inicializar la lista aqui
*/
LList* LList_create() {
    LList* cabecera;
    cabecera = (LList*)malloc(sizeof(LList));
    if (cabecera != NULL) {
        cabecera->sig = NULL;
        // almacenar numero de elementos en el nodo cabecera
        cabecera->value = (int*)malloc(sizeof(int));
        *(int*)(cabecera->value) = 0;

        return cabecera;
    }
    return NULL; 
}

/* Destruye la lista. No se debe utilizar mas la lista despues de destruirla.
    Retorna OK si no hay errores, sino ERROR
*/
BOOLEAN LList_destroy(LList* lista) {
    LList *cabecera, *aux;
    CONFIRM_NOTNULL(lista, ERROR);
    cabecera = (LList*)lista;
    aux = (LList*)malloc(sizeof(LList));
    if (aux == NULL) { return ERROR; }
    while (cabecera != NULL) {
        // guardar una referencia del sigte en el auxiliar 
        aux = cabecera->sig;
        free(cabecera);
        cabecera = aux;
    }
    free(aux);
    return OK; 
}

/* Retorna el tamano (la cantidad de elementos dentro de la lista)
    Retorna -1 si hubo un error
*/
int LList_size(LList* lista) {
    CONFIRM_NOTNULL(lista, -1);
    int size = *(int*)(lista->value);
    return size;
}

/* Agrega un valor al final de la lista
    Retorna ERROR si hay algun error (ej: argumento invalido)
*/
BOOLEAN LList_add(LList* lista, void* valor) {
    LList *cabecera, *new;
    CONFIRM_NOTNULL(lista, ERROR);
    CONFIRM_NOTNULL(valor, ERROR);
    cabecera = (LList*)lista;
    // aumentar conteo de elementos
    int* size = (int*)(cabecera->value);
    (*size)++;

    // recorrer hasta hallar el ultimo nodo (su sigte es NULL) 
    while (cabecera->sig != NULL) {
        cabecera = cabecera->sig;
    }
    // la cabecera ahora es el ultimo nodo, su siguiente ahora sera el nuevo nodo
    new = (LList*)malloc(sizeof(LList));
    CONFIRM_NOTNULL(new, ERROR);
    new->value = valor;
    new->sig = NULL;
    cabecera->sig = new;

    return OK;
}

/* Inserta un valor antes de la posicion dada 
    Retorna OK si no hay errores, sino ERROR.
    Ej index = 0 primera posicion
    index = 1 segunda posicion..
    index = n (n+1) posicion.
*/
BOOLEAN LList_addPos(LList* lista, int index, void* valor) {
    LList* head, * new;
    int* currentSize;
    CONFIRM_NOTNULL(lista, ERROR);
    head = (LList*)lista;
    // usar el tamano de lista como referencia 
    currentSize = (int*)(head->value);
    if (index < 0 || index > *currentSize) { // verificar si index esta en el rango
        return ERROR; // no se pudo inseRtar
    }

    (*currentSize)++;
    //*(int*)(head->value) = currentSize++; // porque agregaremos un nuevo elemento
    while (index > 0) {
        head = head->sig;
        index--;
    }
    // ahora el siguiente es el indice donde se quiere insertar
    new = (LList*)malloc(sizeof(LList));
    CONFIRM_NOTNULL(new, ERROR);
    new->value = valor;
    new->sig = head->sig;
    head->sig = new;
    return OK;
}


/* Carga el parametro pResultado con el valor 
   en la posicion dada de la lista
    Retorna OK si no hay errores, sino ERROR
    Retorna ERROR si no hay mas
    Si hay error pResultado no cambia
*/
BOOLEAN LList_get(LList* lista, int index, void** pResultado) {
    LList* head;
    CONFIRM_NOTNULL(lista, ERROR);
    CONFIRM_NOTNULL(pResultado, ERROR);
    head = lista;

    int* currentSize = (int*)(head->value);
    if (index < 0 || index >= *currentSize) { // verificar si está dentro del rango
        return ERROR; // No se pudo obtener
    }
    while (index > 0) {
        head = head->sig;
        index--;
    }
    *pResultado = head->sig->value; // asignar el valor al puntero de resultado
    return OK;
}

/* Borra el dato de la lista especificado por la posicion dada
    Retorna OK si no hay errores, sino ERROR
*/
BOOLEAN LList_remove(LList* lista, int index) {
    LList* head, * prev;
    CONFIRM_NOTNULL(lista, ERROR);
    head = lista->sig;

    // usar el tamaño de la lista como referencia 
    int* currentSize = (int*)lista->value; // Aquí usamos el tamaño de la lista desde el nodo cabecera
    if (index < 0 || index >= *currentSize) { // verificar si index está en el rango
        return ERROR; // no se pudo borrar
    }
    (*currentSize)--; // reducimos el tamaño de la lista porque borraremos un elemento

    if (index == 0) { // Si se va a eliminar el primer elemento
        lista->sig = head->sig; // Actualizamos el inicio de la lista
        free(head); // Liberamos la memoria del nodo eliminado
        return OK;
    }

    prev = head;
    while (index > 1) { // Iteramos hasta llegar al nodo anterior al que se eliminará
        prev = prev->sig;
        index--;
    }

    // Ahora prev apunta al nodo anterior al que se eliminará
    head = prev->sig; // head apunta al nodo que se eliminará
    prev->sig = head->sig; // El nodo anterior ahora apunta al siguiente del nodo que se eliminará
    free(head); // Liberamos la memoria del nodo eliminado
    return OK;
}

/* Borra el valor de la lista.
    Retorna OK si no hay errores, sino ERROR
*/
BOOLEAN LList_removeValue(LList* lista, void* valor) {
    LList *head, *prev;
    CONFIRM_NOTNULL(lista, ERROR);
    head = (LList*)lista;
    prev = (LList*)malloc(sizeof(LList));
    CONFIRM_NOTNULL(prev, ERROR);

    // EMPEZAR A RECORRER DESDE 1er ELEMENTO
    prev = head;
    head = head->sig;
    while (head != NULL) {
        if (head->value == valor) {
            prev->sig = head->sig; // si se encuentra enlazar el nodo anterior con el siguiente del actual
            free(head);
            return OK;
        }
        else { // si no se encuentra hacer correr los nodos
            prev = head;
            head = head->sig;
        }
    }
    return ERROR; // no se pudo borrar
}

/************************************************************
        M A I N
/***********************************************************/



BOOLEAN testLList() {

    /* AGREGUE CODIGO AQUI
        que ejercite la lista basada en listas ligadas LList
        de tal forma que cualquier error en la implementacion 
        sera visible. 
        No se olvide de probar las funciones con valores no validos
        para asegurar que estas manejan estos casos bien.
        
        Por ejemplo hacer algo similar a esto: (pero obviamente probando todos las funciones
		implementadas y los casos limites)
			LList* l = LList_create();
			char* value = "";
			int pos = 0;
			CONFIRM_NOTNULL(l,ERROR);
			CONFIRM_TRUE(LList_add(l,"papa"),ERROR);
			CONFIRM_TRUE(LList_get(l,pos,(void**)&value),ERROR);
			CONFIRM_TRUE(strcmp("papa", value), ERROR);	
			CONFIRM_TRUE(LList_destroy(l),ERROR);	
    */
   return FALSE;
}

/**

Esta seccion contiene las funciones relacionadas con polinomios.

* /

/************************************************************
                    P O L I N O M I O S
/***********************************************************/

/* Crea un Monomio */
Monomial* createMonomial(int coef, int degree) {
    Monomial* mono = (Monomial*)malloc(sizeof(Monomial));
    CONFIRM_NOTNULL(mono, NULL);
    
    if (coef == 0) { return NULL; }
    mono->coef = coef;
    mono->degree = degree;

    return mono;
}

BOOLEAN getPolinomial(LList* poli) {
    CONFIRM_NOTNULL(poli, ERROR);
    int coef, degree;
    char option;
    printf("- MONOMIOS - \nPara finalizar, ingrese Coeficiente = 0 o Grado negativo");
    do {
        // obtener grado y coeficiente
        printf("\nIngrese coeficiente = ");
        scanf("%d", &coef);
        if (coef == 0) {
            printf("\nCoef. 0, fin del polinomio");
            break;
        }
        printf("\nIngrese grado = ");
        scanf("%d", &degree);
        if (degree < 0) {
            printf("\n Grado negativo, fin del polinomio");
            break;
        }
        // crear monomio y agregar a la lista
        Monomial* newMono = createMonomial(coef, degree);
        LList_add(poli, newMono);
    } while (coef != 0);
    
    printf("\nPolinomio agregado con exito: ");
    return OK;
}

BOOLEAN printPolinomial(LList* poli) {
    LList* head;
    //int i, size;
    CONFIRM_NOTNULL(poli, ERROR);

    head = (LList*)poli;
    head = head->sig; // empezar a imprimir desde el primer monomio
    
    while (head != NULL) {
        Monomial* mono = (Monomial*)head->value;
        if (mono->coef > 0) { // si coeficiente es positivo, imprimir signo +
            printf("+%d", mono->coef);
        }
        else {
            printf("%d", mono->coef);
        }

        if (mono->degree != 0) { // si el grado no es 0, imprimir incognita y exponente
            printf("x^%d ", mono->degree);
        }
        head = head->sig;
    }
    
    return OK;
}

// ordenar el polinomio segun el grado, de mayor a menor
// para ello, se usa el algoritmo SELECTION SORT
// fuente consultada https://www.geeksforgeeks.org/iterative-selection-sort-for-linked-list/
BOOLEAN sort_Polinomial(LList* poli) {
    CONFIRM_NOTNULL(poli, ERROR);
    LList *head, *temp, *max;
    head = poli->sig;
    
    while (head != NULL) {
        max = head; // iniciar asumiendo que el grado del primer monomio es el mayor
        temp = (LList*)head->sig;
        // recorrer la parte desordenada de la lista
        while (temp != NULL) {
            if (((Monomial*)(temp->value))->degree > ((Monomial*)(max->value))->degree) {
                max = temp;
            }
            temp = temp->sig;
        }
        // intercambiar los monomios 
        Monomial* tempMono = (Monomial*)(head->value);
        head->value = max->value;
        max->value = tempMono;

        head = head->sig;
    }
    return OK;
}

BOOLEAN reducePolinomial(LList* poli) {
    CONFIRM_NOTNULL(poli, ERROR);
    sort_Polinomial(poli); // se ordena el polinomio
    LList *head, *temp, *sig, *prev;
    int sum;
    head = (LList*)poli->sig;
    prev = (LList*)poli;

    while (head != NULL) {
        temp = head;
        sig = head->sig;
        sum = ((Monomial*)(head->value))->coef; // inicializar suma
        // si el monomio es de igual grado que el siguiente, se suman los coeficientes y se elimina el siguiente
        while (sig != NULL && ((Monomial*)(head->value))->degree == ((Monomial*)(sig->value))->degree) {
            // sumar los coeficientes
            ((Monomial*)(head->value))->coef += ((Monomial*)(sig->value))->coef;
            
            // eliminar el siguiente monomio
            temp->sig = sig->sig;
            free(sig);
            sig = temp->sig; 
        }

        // si el monomio queda con coeficiente 0 se elimina
        if (((Monomial*)(head->value))->coef == 0) {
            prev->sig = head->sig;
            free(head);
            head = prev->sig;
        }
        else {
            prev = head;
            head = head->sig;
        }
    }
    return OK;
}

// evaluar el polinomio dado un valor de x
double evaluate(LList* poli, int x) {
    CONFIRM_NOTNULL(poli, ERROR);
    LList* head = (LList*)poli->sig;
    double result = 0.0;

    while (head != NULL) {
        Monomial* mono = (Monomial*)head->value;
        result += (pow(x, mono->degree) * mono->coef);
        head = head->sig;
    }
    return result;
}

int main(int argc, char *argv[]){
    testLList();
    int option = -1;
    LList* polinomial;
    polinomial = LList_create();
    int x;

  // imprimir menu
    while (option != 3) {
        printf("\n-SELECCIONE:-\n1. Ingresar polinomio\n2. Evaluar polinomio\n3. Salir\n");
        scanf("%d", &option);

        switch (option) {
            case 1:
                getPolinomial(polinomial);
                reducePolinomial(polinomial);
                printPolinomial(polinomial);
                break;
            case 2:
                printf("\n Ingrese valor de x: ");
                scanf("%d", &x);
                double res = evaluate(polinomial, x);
                printf("RESULTADO: %f", res);
                break;
            case 3:
                printf("Fin del programa.");
                break;
            }
    }
  /*AGREGUE CODIGO AQUI 
  implemente la funcion para ingresar polinomios y almacenarlos dentro de la lista, 
  imprimir el polinomio reducido 
  y evaluar el polinomio en base a un valor
  */
    system("pause");
    return 0;
}
