#include "hash.h"
#include "vector_dinamico.h"
#include "nodo_hash.h"
#include <stdio.h>
#include <string.h>
#define CAP_INICIAL 10
#define CANT_INICIAL 0
#define REDIMENSIONAR_POR_MAX 0.7
#define REDIMENSIONAR_POR_MIN 0.2
#define PORC_REDIMENSIONAR 2

typedef struct hash{
  vector_t * vector;
  hash_destruir_dato_t destruir_dato;
  // int capacidad;
  size_t cant_elementos;
}hash_t;

// Devuelve el indice en el cual se debe guardar la clave.
// El indice devuelto esta entre cero y maximo.
size_t hashing(const char* clave, size_t maximo);

// Devuelve un nuevo indice.
size_t probing(size_t index);

// Devuelve el indice en el cual se debe guardar la clave.
size_t obtener_indice_vacio(const char * clave, vector_t* vector);

// Redimensiona el hash y rehashea las claves.
bool redimensionar(hash_t* hash);

// Busca el nodo que tiene la clave
nodo_hash_t* buscar_nodo(const hash_t* hash, const char * clave);

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
  hash_t* hash = malloc(sizeof(hash_t));
  if(hash == NULL)
    return NULL;

  // hash->capacidad       = CAP_INICIAL;
  hash->destruir_dato   = destruir_dato;
  hash->cant_elementos  = CANT_INICIAL;
  hash->vector = vector_crear(CAP_INICIAL);

  if(hash->vector != NULL)
    return hash;

  free(hash);
  return NULL;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
  if(hash->cant_elementos / vector_obtener_tamanio(hash->vector) > REDIMENSIONAR_POR_MAX){
    if(!redimensionar(hash))
      return false;
  }

  nodo_hash_t* nodo = nodo_hash_crear(clave, dato);
  if(nodo == NULL)
    return false;

  size_t index = obtener_indice_vacio(clave, hash->vector);
  hash->cant_elementos++;
  return vector_guardar(hash->vector, index, nodo);
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave){
  nodo_hash_t* nodo = buscar_nodo(hash, clave);
  hash->cant_elementos--;
  return nodo == NULL ? NULL : nodo_hash_borrar(nodo);
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave){
  nodo_hash_t * nodo = buscar_nodo(hash, clave);
  return nodo_hash_obtener_valor(nodo);
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave){
  return buscar_nodo(hash, clave) != NULL;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash){
  return hash->cant_elementos;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash){
  vector_destruir(hash->vector, hash->destruir_dato);
  free(hash);
}

// Por defecto el minimo es cero.
size_t hashing(const char* clave, size_t maximo){
  return 0;
}

size_t probing(size_t index){
  return ++index;
}

size_t obtener_indice_vacio(const char * clave, vector_t* vector){
  size_t index = hashing(clave, vector_obtener_tamanio(vector));
  while (vector_obtener(vector, index) != NULL)
    index = probing(index);
  return index;
}

nodo_hash_t* buscar_nodo(const hash_t* hash, const char * clave){
  size_t index = hashing(clave, vector_obtener_tamanio(hash->vector));
  while (vector_obtener(hash->vector, index) != NULL && strcmp(nodo_hash_obtener_clave(vector_obtener(hash->vector, index)), clave) && nodo_hash_obtener_estado(vector_obtener(hash->vector, index)) != VACIO)
    index = probing(index);

  return vector_obtener(hash->vector, index) == NULL ? NULL : vector_obtener(hash->vector, index);
}

bool redimensionar(hash_t* hash){
  vector_t* nuevo_vector = vector_crear(vector_obtener_tamanio(hash->vector)*PORC_REDIMENSIONAR);
  if(nuevo_vector == NULL)
    return false;

  for(int i = 0; vector_obtener_tamanio(hash->vector); i++){
    nodo_hash_t* nodo = vector_obtener(hash->vector, i);
    if(nodo != NULL){
      size_t index = obtener_indice_vacio(nodo_hash_obtener_clave(nodo), hash->vector);
      vector_guardar(hash->vector, index, nodo);
    }
  }

  vector_destruir(hash->vector, hash->destruir_dato);
  hash->vector = nuevo_vector;
  return true;
}

/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter);
