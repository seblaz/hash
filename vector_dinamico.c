#include "vector_dinamico.h"

typedef struct vector {
    size_t  tam;
    void **  datos;
} vector_t;

vector_t* vector_crear(size_t tam) {
    vector_t* vector = malloc(sizeof(vector_t));

    if (vector == NULL) {
        return NULL;
    }
    vector->datos = calloc(1, tam * sizeof(void*));

    if (tam > 0 && vector->datos == NULL) {
        free(vector);
        return NULL;
    }
    vector->tam = tam;
    return vector;
}

bool vector_guardar(vector_t* vector, size_t pos, void* valor){
    if(vector->tam > pos){
        vector->datos[pos] = valor;
        return true;
    }
    return false;
}

void* vector_obtener(vector_t* vector, size_t pos){
    if(vector->tam > pos)
        return vector->datos[pos];
    return NULL;
}

size_t vector_obtener_tamanio(vector_t* vector){
    return vector->tam;
}

bool vector_redimensionar(vector_t* vector, size_t tam_nuevo) {
    void** datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(int));

    // Cuando tam_nuevo es 0, es correcto si se devuelve NULL.
    // En toda otra situación significa que falló el realloc.
    if (tam_nuevo > 0 && datos_nuevo == NULL)
        return false;

    vector->datos = datos_nuevo;
    vector->tam = tam_nuevo;
    return true;
}

void vector_destruir(vector_t* vector){
    free(vector->datos);
    free(vector);
}
