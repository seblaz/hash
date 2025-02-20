#include "vector_dinamico.h"
#include "nodo_hash.h"
#include <stdio.h>

typedef struct vector {
    size_t         tam;
    nodo_hash_t*  datos;
} vector_t;

#ifndef NODO_STRUCT // El vector debe conocer la estructura para hacer el malloc.
#define NODO_STRUCT

typedef struct nodo_hash{
  estados estado;
  void * valor;
  char * clave;
}nodo_hash_t;

#endif

vector_t* vector_crear(size_t tam) {
    vector_t* vector = malloc(sizeof(vector_t));

    if (vector == NULL) {
        return NULL;
    }
    vector->datos = calloc(1, tam * sizeof(nodo_hash_t));

    if (tam > 0 && vector->datos == NULL) {
        free(vector);
        return NULL;
    }
    vector->tam = tam;
    return vector;
}

nodo_hash_t* vector_obtener(vector_t* vector, size_t pos){
    if(vector->tam > pos)
        return &vector->datos[pos];
    return NULL;
}

size_t vector_obtener_tamanio(vector_t* vector){
    return vector->tam;
}

bool vector_redimensionar(vector_t* vector, size_t tam_nuevo) {
    nodo_hash_t* datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(nodo_hash_t));

    if (tam_nuevo > 0 && datos_nuevo == NULL)
        return false;

    vector->datos = datos_nuevo;
    vector->tam = tam_nuevo;
    return true;
}

void vector_destruir(vector_t* vector, void destruir_dato(void *)){
  for (size_t i = 0; i < vector_obtener_tamanio(vector); i++) {
    nodo_hash_destruir(vector_obtener(vector, i), destruir_dato);
  }
  free(vector->datos);
  free(vector);
}

void vector_destruir_sin_datos(vector_t* vector){
  free(vector->datos);
  free(vector);
}
