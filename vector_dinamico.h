#ifndef VECTOR_DINAMICO_H
#define VECTOR_DINAMICO_H

#include <stdlib.h>
#include <stdbool.h>
#include "nodo_hash.h"

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

struct vector;
typedef struct vector vector_t;


/*******************************************************************
 *                    PRIMITIVAS DEL VECTOR
 ******************************************************************/

// Crea un vector de tamaño tam.
// Post: vector es una vector vacío de tamaño tam.
vector_t* vector_crear(size_t tam);

// Almacena el valor en la posición pos. Devuelve verdadero o falso
// dependiedo si se pudo guardar el valor en la posicion.
// Pre: el vector fue creado
// Post: se almacenó el valor en la posición pos.
bool vector_guardar(vector_t* vector, size_t pos, void* valor);

// Devuelve el dato guardado en la posición pos del vector.
// Si la posicion pos es invalida devuelve NULL.
// Pre: el vector fue creado.
// Post: se devolvio el valor guardado en pos.
void* vector_obtener(vector_t* vector, size_t pos);

// Devuelve el tamaño del vector.
// Pre: el vector fue creado.
// Post: se devolvio el tamaño del vector
size_t vector_obtener_tamanio(vector_t* vector);

// Cambia el tamaño del vector.
// Pre: el vector fue creado.
// Post: el vector cambió de tamaño a nuevo_tam y devuelve true o el vector
// queda intacto y devuelve false si no se pudo cambiar el tamaño a nuevo_tam.
bool vector_redimensionar(vector_t* vector, size_t nuevo_tam);

// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vector_destruir(vector_t* vector);

#endif // VECTOR_DINAMICO_H
