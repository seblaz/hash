#include "nodo_hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo_hash{
  estados estado;
  void * valor;
  char * clave;
}nodo_hash_t;

nodo_hash_t * nodo_hash_crear(char* clave, void * dato){
  nodo_hash_t * nodo_hash = malloc(sizeof(nodo_hash_t));
  if(nodo_hash == NULL)
    return NULL;

  nodo_hash->estado = OCUPADO;
  nodo_hash->valor  = dato;
  nodo_hash->clave  = malloc(sizeof(char)*(strlen(clave)+1)); //

  if(nodo_hash->clave == NULL){
    free(nodo_hash);
    return NULL;
  }

  strcpy(nodo_hash->clave, clave);
  return nodo_hash;
}

char* nodo_hash_obtener_clave(nodo_hash_t * nodo){
  return nodo->clave;
}

void* nodo_hash_obtener_valor(nodo_hash_t * nodo){
  return nodo->valor;
}

estados nodo_hash_obtener_estado(nodo_hash_t * nodo){
  return nodo->estado;
}

void nodo_hash_cambiar_valor(nodo_hash_t * nodo, void * dato, void (*destruir_dato)(void *)){
  if(destruir_dato)
    destruir_dato(nodo->valor);
  nodo->valor = dato;
}

void* nodo_hash_borrar(nodo_hash_t * nodo){
  void* dato = nodo->valor;
  nodo->valor   = NULL;
  nodo->estado  = BORRADO;
  return dato;
}

void* nodo_hash_destruir(nodo_hash_t * nodo){
  void* dato = nodo->valor;
  free(nodo->clave);
  free(nodo);
  return dato;
}

void nodo_hash_cambiar_estado(nodo_hash_t * nodo, estados estado){
  nodo->estado = estado;
}
