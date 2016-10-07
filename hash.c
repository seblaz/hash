#include "hash.h"
#include "vector_dinamico.h"
#include "nodo_hash.h"
#include <stdio.h>
#include <string.h>
#define CAP_INICIAL 10
#define CANT_INICIAL 0
#define REDIMENSIONAR_POR_MAX 0.75
#define REDIMENSIONAR_POR_MIN 0.25
#define PORC_REDIMENSIONAR 2

typedef struct hash{
  vector_t * vector;
  hash_destruir_dato_t destruir_dato;
  size_t cant_elementos;
}hash_t;


// Hashea la clave siempre del mismo modo.
size_t hashing(const char* clave, size_t maximo);

// Aumenta index en 1.
size_t probing(size_t index);

// Devuelve un indice en el cual no hay ninguna clave.
size_t obtener_indice_vacio(const char * clave, vector_t* vector);

// Redimensiona el hash a un nuevo tamano de tam_redim.
bool redimensionar(hash_t* hash, size_t tam_redim);

// Busca el nodo con la clave en el hash. Si no lo encuentra devuelve NULL.
nodo_hash_t* buscar_nodo(const hash_t* hash, const char * clave);

// Devuelve el indice del nodo que contiene la clave. Si no lo encuentra devuelve -1.
long buscar_indice(const hash_t *hash, const char *clave);

// Guarda el nodo en una posicion vacia del hash.
bool guadar_nodo(hash_t *hash, const char *clave, void *dato);



hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
  hash_t* hash = malloc(sizeof(hash_t));
  if(hash == NULL)
    return NULL;

  hash->destruir_dato   = destruir_dato;
  hash->cant_elementos  = CANT_INICIAL;
  hash->vector = vector_crear(CAP_INICIAL);

  if(hash->vector != NULL)
    return hash;

  free(hash);
  return NULL;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
  size_t tam_aux = vector_obtener_tamanio(hash->vector); // esta variable auxiliar es necesaria porque los flags con los que se compila no permiten un casteo de lo que devuelve una funcion.
  if((double) hash->cant_elementos / (double) tam_aux > REDIMENSIONAR_POR_MAX){
    if(!redimensionar(hash, vector_obtener_tamanio(hash->vector)*PORC_REDIMENSIONAR))
      return false;
  }

  nodo_hash_t* nodo_existente = buscar_nodo(hash, clave);
  if(nodo_hash_obtener_estado(nodo_existente) == OCUPADO){ // La clave se encuentra en el hash
    nodo_hash_cambiar_valor(nodo_existente, dato, hash->destruir_dato);
    return true;
  }

  hash->cant_elementos++;
  return guadar_nodo(hash, clave, dato);
}

void *hash_borrar(hash_t *hash, const char *clave){
  size_t tam_aux = vector_obtener_tamanio(hash->vector);
  if((double) hash->cant_elementos / (double) tam_aux < REDIMENSIONAR_POR_MIN){
    if(!redimensionar(hash, vector_obtener_tamanio(hash->vector)/PORC_REDIMENSIONAR))
      return false;
  }

  nodo_hash_t* nodo = buscar_nodo(hash, clave);
  if(nodo_hash_obtener_estado(nodo) == VACIO)
    return NULL;

  hash->cant_elementos--;
  return nodo_hash_borrar(nodo);
}

void *hash_obtener(const hash_t *hash, const char *clave){
  nodo_hash_t * nodo = buscar_nodo(hash, clave);
  return nodo_hash_obtener_valor(nodo);
}

bool hash_pertenece(const hash_t *hash, const char *clave){
  return nodo_hash_obtener_estado(buscar_nodo(hash, clave)) == OCUPADO;
}

size_t hash_cantidad(const hash_t *hash){
  return hash->cant_elementos;
}

void hash_destruir(hash_t *hash){
  vector_destruir(hash->vector, hash->destruir_dato);
  free(hash);
}


/************************* FUNCIONES DE HASHING********************************/
size_t hash(const char *str) {

	unsigned int seed = 0 ;
	unsigned int hash = seed;
    while (*str)
    {
        hash = hash * 101  +  *str++;
    }
    return hash;
}

size_t hashing(const char* clave, size_t maximo){
  size_t hash_n = hash(clave) % maximo;
  if(hash_n > maximo || hash_n < 0)
    printf("%s\n", "la funcion de has esta mal");
  return hash_n;
}

size_t probing(size_t index){
  return ++index;
}


/************************* FUNCIONES AUXILIARES********************************/
long buscar_indice(const hash_t *hash, const char *clave){
  size_t index = hashing(clave, vector_obtener_tamanio(hash->vector));
  nodo_hash_t* nodo = vector_obtener(hash->vector, index);

  while (nodo_hash_obtener_estado(nodo) != VACIO){
    if(nodo_hash_obtener_estado(nodo) == OCUPADO && !strcmp(nodo_hash_obtener_clave(nodo), clave)) // strcmp devuelve 0 (false) cuando las cadenas son iguales
      return index;

    index = probing(index);
    if(index == vector_obtener_tamanio(hash->vector))
      index = 0;

    nodo = vector_obtener(hash->vector, index);
  }

  return -1;
}

nodo_hash_t* buscar_nodo(const hash_t* hash, const char * clave){
  long index = buscar_indice(hash, clave);
  if(index == -1)
    return NULL;
  return vector_obtener(hash->vector, index);
}

size_t obtener_indice_vacio(const char * clave, vector_t* vector){
  size_t index = hashing(clave, vector_obtener_tamanio(vector));
  while (nodo_hash_obtener_estado(vector_obtener(vector, index)) == OCUPADO){
    index = probing(index);
    if(index == vector_obtener_tamanio(vector))
      index = 0;
  }
  return index;
}

bool guadar_nodo(hash_t *hash, const char *clave, void *dato){
  size_t index = obtener_indice_vacio(clave, hash->vector);

  nodo_hash_t* nodo = vector_obtener(hash->vector, index);
  nodo_hash_cambiar_valor(nodo, dato, hash->destruir_dato);
  nodo_hash_cambiar_estado(nodo, OCUPADO);
  bool result = nodo_hash_cambiar_clave(nodo, clave);
  return result;
}

bool redimensionar(hash_t* hash, size_t tam_redim){
  vector_t* nuevo_vector = vector_crear(tam_redim);
  if(nuevo_vector == NULL)
    return false;

  vector_t * viejo_vector = hash->vector;
  hash->vector = nuevo_vector;

  for(int i = 0; i < vector_obtener_tamanio(viejo_vector); i++){
    nodo_hash_t* nodo = vector_obtener(viejo_vector, i);
    if(nodo_hash_obtener_estado(nodo) == OCUPADO){
      guadar_nodo(hash, nodo_hash_obtener_clave(nodo), nodo_hash_obtener_valor(nodo));
    }
  }

  vector_destruir(viejo_vector, NULL);
  return true;
}




/************************* Iterador del hash ********************************/

typedef struct hash_iter{
	const hash_t*  hash_iter;
	size_t   actual;
}hash_iter_t;

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(iter == NULL)
		return NULL;

	iter->hash_iter = hash;
	iter->actual    = CANT_INICIAL;

  if(nodo_hash_obtener_estado(vector_obtener(iter->hash_iter->vector, iter->actual)) != OCUPADO) // Debe avanzar al primer nodo ocupado solo si el primero no esta ocupado
    hash_iter_avanzar(iter);

  return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
  if(hash_iter_al_final(iter))
    return false;

  do iter->actual++;
  while(!hash_iter_al_final(iter) && nodo_hash_obtener_estado(vector_obtener(iter->hash_iter->vector, iter->actual)) != OCUPADO);

  return !hash_iter_al_final(iter);
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if(hash_iter_al_final(iter))
		return NULL;

	return nodo_hash_obtener_clave(vector_obtener(iter->hash_iter->vector, iter->actual));
}

bool hash_iter_al_final(const hash_iter_t *iter){
  return iter->actual == vector_obtener_tamanio(iter->hash_iter->vector);
}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}
