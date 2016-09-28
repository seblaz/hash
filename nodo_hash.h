#ifndef NODO_HASH_H
#define NODO_HASH_H

struct nodo_hash;
typedef struct nodo_hash nodo_hash_t;

typedef enum{ VACIO, OCUPADO, BORRADO }estados;

// Crea el nodo del hash con el dato correspondiente.
// En caso de que no pueda crearlo devuelve NULL.
// El valor inicial del estado es OCUPADO.
// Pre: existe espacio en memoria.
// Post: se creo el nodo del hash.
nodo_hash_t * nodo_hash_crear(char clave [], void * dato);

// Devuele la clave del nodo.
// Pre: el nodo fue creado.
// Post: se devolvio la clave del nodo.
char* nodo_hash_obtener_clave(nodo_hash_t * nodo);

// Devuele el valor del nodo.
// Pre: el nodo fue creado.
// Post: se devolvio el valor del nodo.
void* nodo_hash_obtener_valor(nodo_hash_t * nodo);

// Devuele el estado del nodo.
// Pre: el nodo fue creado.
// Post: se devolvio el estado del nodo.
estados nodo_hash_obtener_estado(nodo_hash_t * nodo);

// Cambia el valor del nodo y destruye el anterior
// si se para destruir_dato como parametro.
// Pre: el nodo fue creado.
// Post: se cambio el valor del nodo.
void nodo_hash_cambiar_valor(nodo_hash_t * nodo, void * dato, void (*destruir_dato)(void *));

// Destruye el nodo de hash y devuele el dato almacenado en el nodo.
// Pre: el nodo fue creado.
// Post: se destruyo el nodo.
void* nodo_hash_destruir(nodo_hash_t * nodo);

// Cambia el estado del nodo a BORRADO, borra el valor del nodo
// y devuele el dato almacenado en el nodo.
// Pre: el nodo fue creado.
// Post: se borro el nodo.
void* nodo_hash_borrar(nodo_hash_t * nodo);

// Cambia el estado del nodo.
// Pre: el nodo fue creado.
// Post: se cambio el estado del nodo.
void nodo_hash_cambiar_estado(nodo_hash_t * nodo, estados estado);

#endif
