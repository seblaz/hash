#include "nodo_hash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef NODO_STRUCT // Como el struct se define en vector_dinamico hay que fijarse que no halla sido definido previamente
#define NODO_STRUCT

typedef struct nodo_hash{
  estados estado;
  void * valor;
  char * clave;
}nodo_hash_t;

#endif



char* nodo_hash_obtener_clave(nodo_hash_t * nodo){
  return nodo == NULL ? NULL : nodo->clave;
}

void* nodo_hash_obtener_valor(nodo_hash_t * nodo){
  return nodo == NULL ? NULL : nodo->valor;
}

estados nodo_hash_obtener_estado(nodo_hash_t * nodo){
  return nodo == NULL ? VACIO : nodo->estado;
}

void nodo_hash_cambiar_valor(nodo_hash_t * nodo, void * dato, void (*destruir_dato)(void *)){
  if(destruir_dato)
    destruir_dato(nodo->valor);
  nodo->valor = dato;
}

bool nodo_hash_cambiar_clave(nodo_hash_t * nodo, const char *clave){
  nodo->clave = malloc(sizeof(char)*(strlen(clave)+1));
  if(nodo->clave == NULL)
    return false;
  strcpy(nodo->clave, clave);
  return true;
}

void nodo_hash_cambiar_estado(nodo_hash_t * nodo, estados estado){
  nodo->estado = estado;
}

void* nodo_hash_borrar(nodo_hash_t * nodo){
  void* dato = nodo->valor;
  nodo->valor   = NULL;
  nodo->estado  = BORRADO;
  free(nodo->clave);
  return dato;
}

void nodo_hash_destruir(nodo_hash_t * nodo, void (*destruir_dato_t)(void *)){
  if(destruir_dato_t)
    destruir_dato_t(nodo->valor);
  if(nodo_hash_obtener_estado(nodo) == OCUPADO)
    free(nodo->clave);
}
