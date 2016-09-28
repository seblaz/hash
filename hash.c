#include "hash.h"
#include "vector_dinamico.h"
#include "nodo_hash.h"
#define CAP_INICIAL  10
#define CANT_INICIAL  0

typedef struct hash{
  vector_t * vector_dinamico;
  hash_destruir_dato_t destruir_dato;
  int capacidad;
  int cant_elementos;
}hash_t;

size_t hashing(const char* clave, int maximo);

size_t probing(size_t index);

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
  hash_t* hash = malloc(sizeof(hash_t));
  if(hash == NULL)
    return NULL;

  // hash->capacidad       = CAP_INICIAL;
  hash->destruir_dato   = destruir_dato;
  hash->cant_elementos  = CANT_INICIAL;
  hash->vector_dinamico = vector_crear(CAP_INICIAL);

  if(hash->vector_dinamico != NULL)
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
  nodo_hash_t* nodo = nodo_hash_crear(clave, dato);
  if(nodo == NULL)
    return false;

  size_t index = hashing(clave, hash->capacidad);
  while (vector_obtener(hash->vector_dinamico, index) != NULL)
    index = probing(index);

  vector_guardar(hash->vector_dinamico, index, nodo);
  return true;
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave);

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave);

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave);

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash){
  
}

// Por defecto el minimo es cero.
size_t hashing(const char* clave, int maximo){
  return 1;
}

size_t probing(size_t index){
  return index++;
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
