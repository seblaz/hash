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

typedef struct hash_iter{
	hash_t* hash_iter;
	bool iter_fin;
	size_t actual;
}hash_iter_t;

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

// Busca el indice del nodo correspondiente a la clave. Si no lo encuentra devuelve -1.
size_t buscar_indice(const hash_t *hash, const char *clave);

// Guarda el nodo en el indice especificado.
// Pre: el indice no existe en el hash.
// Post: se guardo el nodo en el hash.
bool guadar_nodo(hash_t *hash, nodo_hash_t *nodo);

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

  nodo_hash_t* nodo_existente = buscar_nodo(hash, clave);
  if(nodo_existente != NULL){ // La clave se encuentra en el hash
    nodo_hash_cambiar_valor(nodo_existente, dato, hash->destruir_dato);
    return true;
  }

  nodo_hash_t* nodo = nodo_hash_crear(clave, dato);
  if(nodo == NULL)
    return false;

  hash->cant_elementos++;
  return guadar_nodo(hash, nodo);
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave){
  nodo_hash_t* nodo = buscar_nodo(hash, clave);
  if(nodo == NULL)
    return NULL;

  hash->cant_elementos--;
  return nodo_hash_borrar(nodo);
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

size_t buscar_indice(const hash_t *hash, const char *clave){
  size_t index = hashing(clave, vector_obtener_tamanio(hash->vector));
  nodo_hash_t* nodo = vector_obtener(hash->vector, index);

  while (nodo != NULL && strcmp(nodo_hash_obtener_clave(nodo), clave) && nodo_hash_obtener_estado(nodo) != VACIO){
    index = probing(index);
    nodo = vector_obtener(hash->vector, index);
  }

  return nodo == NULL || nodo_hash_obtener_estado(nodo) == BORRADO ? -1 : index;
}

nodo_hash_t* buscar_nodo(const hash_t* hash, const char * clave){
  return vector_obtener(hash->vector, buscar_indice(hash, clave));
}

bool guadar_nodo(hash_t *hash, nodo_hash_t *nodo){
  size_t index = obtener_indice_vacio(nodo_hash_obtener_clave(nodo), hash->vector);
  return vector_guardar(hash->vector, index, nodo);
}

bool redimensionar(hash_t* hash){
  vector_t* nuevo_vector = vector_crear(vector_obtener_tamanio(hash->vector)*PORC_REDIMENSIONAR);
  if(nuevo_vector == NULL)
    return false;

  vector_t * viejo_vector = hash->vector;
  hash->vector = nuevo_vector;

  for(int i = 0; i < vector_obtener_tamanio(viejo_vector); i++){
    nodo_hash_t* nodo = vector_obtener(viejo_vector, i);
    if(nodo != NULL){
      guadar_nodo(hash, nodo);
    }
  }

  vector_destruir_sin_datos(viejo_vector);
  return true;
}

/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(iter == NULL){
		return NULL;
	} 
	iter->hash_iter = hash;
	iter->actual = CANT_INICIAL;
	iter->iter_fin = false;
	return iter;
	
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter){//necesito tener bien la estructura del vector para manejar la manera en que va a avanzar, segun los cambios que me dijiste deberia andar bien asi
	size_t total_pos = vector_obtener_tamanio(iter->hash_iter->vector);
	
	//esto explota si la primera condicion no se cumple e igualmente intenta ejecutar la segunda condicion, ya que tama;o empieza por 1 y pos por 0
	while(iter->actual != total_pos || nodo_hash_obtener_estado(vector_obtener(iter->hash_iter->vector,iter->actual))!= 1 ){
	
		iter->actual++;	
	}
		
	if(iter->actual == total_pos){
		iter->iter_fin = true; //para manejar mas facil el iter_al_final
		return false;
	}
	
	return true;
	

}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter){
	
	if(hash_iter_al_final(iter)){
		return NULL;
	}
	
	return nodo_hash_obtener_clave(vector_obtener(iter->hash_iter->vector,iter->actual));
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter){
	if(iter->iter_fin){
		return true;
	}
	return false;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter){
	free(iter);

}
