#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    {
        int posicion = hash(key, map->capacity);
      
        if (map->buckets[posicion] == NULL)
        {
          map->buckets[posicion] = createPair(key, value);
          map->size++;
          map->current = posicion;
        }
        else
        {
          while (map->buckets[posicion] != NULL)
          {
            if (is_equal(map->buckets[posicion]->key, key))
            {
              map->buckets[posicion]->value = value;
              return;
            }
            posicion = (posicion + 1) % map->capacity;
            
          }
          
          map->buckets[posicion] = createPair(key, value);
          map->size++;
          map->current = posicion;
        }
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair ** old_buckets = map->buckets;
    map->buckets = (Pair **) calloc(map->capacity * 2, sizeof(Pair *));
    map->capacity *= 2;
    map->size = 0;
    for(int k = 0; k < map->capacity / 2; k++)
    {
      if (old_buckets[k] != NULL)
      {
        insertMap(map, old_buckets[k]->key, old_buckets[k]->value);
        free(old_buckets[k]);
      }
    }
    free(old_buckets);
}

HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *) malloc(sizeof(HashMap));
    if (map == NULL) {
        printf("Error al crear el mapa\n");
        return NULL;
    }
    map->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    int posicion = hash(key, map->capacity);
    while (map->buckets[posicion] != NULL){
      if (is_equal(map->buckets[posicion]->key, key))
      {
        map->buckets[posicion]->key = NULL;
        map->size--;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    if (map == NULL || key == NULL) return NULL;

    int posicion = hash(key, map->capacity);
    while (map->buckets[posicion] != NULL)
    {
      if (is_equal(map->buckets[posicion]->key, key))
      {
        map->current = posicion;
        return map->buckets[posicion];
      }
      posicion = (posicion + 1) % map->capacity;
    }
    return NULL;  
}

Pair * firstMap(HashMap * map) {
    if (map == NULL) return NULL;

    for (int k= 0; k < map->capacity;k++)
    {
      if (map->buckets[k] != NULL && map->buckets[k]->key != NULL)
      {
        map->current = k;
        return map->buckets[k];
      }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL) return NULL;
    for (int k = map->current + 1; k < map->capacity; k++)
    {
      if (map->buckets[k] != NULL)
      {
        map->current = k;
        return map->buckets[k];
      }
    }
    return NULL;
}
