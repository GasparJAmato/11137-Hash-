#include <string.h>
#include <stdlib.h>
#include "LISTA/lista.c"

#include "hash.h"


#define FACTOR_CARGA_MAXIMO 0.7

struct hash {

    size_t capacidad;
	size_t cantElementos;
	lista_t **vector;
	

};

typedef struct keyvalue {

	const char *key;
	void *value;
    
} keyvalue_t;


size_t funcion_hash(const char* key, size_t capacidad){
	size_t sum = 0;

	for(int i = 0; i < strlen(key); i++){
		sum += (size_t)key[i];
	
	}
	
	return sum%capacidad;
}


const char* hash_duplicar_clave(const char *key) {
    
    char *copia = malloc(strlen(key) + 1);
    if (!copia){
        return NULL;
    }
    
    strcpy(copia, key);  
	const char* copy = (const char *)copia;

	

    return copy;
}



/**
 * libera los elementos de tipo keyvalue_t
*/
void funcion(void *elemento){
	keyvalue_t *Adestruir = (keyvalue_t *)elemento;
	
	free((void *)Adestruir->key);
	free(Adestruir);
	
} 


int comparador(void* p1, void* p2){
	if(!p1 || !p2 ){
		
		return -1;
	}
	

	keyvalue_t *uno = (keyvalue_t *)p1;

	keyvalue_t *dos = (keyvalue_t *)p2;


	

	if(!uno->key  || !dos->key)
	
	 	return -1;
 
	size_t compa = (size_t )strcmp(uno->key, dos->key);
	
	return (int)compa;
}


void liberar_vector(lista_t** vector, size_t size){
	for(size_t i = 0; i < size; i++){
		
		lista_destruir(vector[i]);

	}
	free(vector);
}


hash_t *hash_crear(size_t capacidad)
{	

	hash_t *hash = malloc(sizeof(hash_t));
	if(!hash){
        return NULL;

    }

	lista_t **vector = NULL;

	if(capacidad <= 3){
		vector = calloc(3, sizeof(lista_t *));
		hash->capacidad = 3;

	}else{
		vector = calloc(capacidad, sizeof(lista_t *));
		
		hash->capacidad = capacidad;

	}

	if(!vector){
		free(hash);
		return NULL;
	}

    for (size_t i = 0; i < hash->capacidad; i++){
	
		vector[i] = lista_crear();

        if(!vector[i]){
			
            liberar_vector(vector, i);
			return NULL;
        }
	}

	hash->vector = vector;
	hash->cantElementos = 0;

	return hash;
}


/**
 * Crea un elemento del struct keyvalue_t y lo devuelve.
*/
keyvalue_t *creacion_de_keyvalue(const char *key, void *value){
	keyvalue_t* result = calloc(sizeof(keyvalue_t), 1);
	if(result == NULL){
		return NULL;
	}

	result->key = hash_duplicar_clave(key);
	result->value = value;

	

	return result;

}

hash_t *rehash(hash_t *hash){
		
	size_t nuevaCapacidad = hash->capacidad * 4;
	lista_t** nuevoVector = calloc(nuevaCapacidad, sizeof(lista_t*));
	if(!nuevoVector){
		return NULL;
	}

	for(int i = 0; i < nuevaCapacidad; i++ ){
		nuevoVector[i] = lista_crear();
	}


	for(size_t w = 0; w < hash->capacidad; w++ ){
		
		lista_t *listaAnterior = hash->vector[w];
		size_t tamanio = lista_tamanio(listaAnterior);

		
		for(size_t j = 0; j < tamanio; j++ ){
			
			keyvalue_t *par = (keyvalue_t *)lista_elemento_en_posicion(listaAnterior, j);

			size_t nuevoIndice = funcion_hash(par->key, nuevaCapacidad);

		 	lista_t *nuevaLista = nuevoVector[nuevoIndice];


			lista_insertar(nuevaLista, par);
			
			
			
			
			
		}
	}

	liberar_vector(hash->vector, hash->capacidad);

	hash->capacidad = nuevaCapacidad;

	
	hash->vector = nuevoVector;
	

	return hash;
}



hash_t *hash_insertar(hash_t *hash, const char *clave, void *elemento,
		      void **anterior)
{	
	if (!hash || !clave)
		return NULL;

	if((hash->cantElementos / hash->capacidad) >= FACTOR_CARGA_MAXIMO){
		hash = rehash(hash);
	}	

	size_t indice = funcion_hash(clave, hash->capacidad);


	keyvalue_t *toInsert = creacion_de_keyvalue(clave, elemento);
	
	
	lista_t *lista = hash->vector[indice];
	
	
	if(!hash_contiene(hash, clave))
	{

		lista_insertar(lista, toInsert);

		if(anterior){
			*anterior = NULL;
		}
		
		hash->cantElementos += 1;

		
		
	}
	else{

		
		size_t posicion = 0;
		lista_iterador_t *iterador = lista_iterador_crear(lista);
		keyvalue_t *Actual = lista_iterador_elemento_actual(iterador);

		while(comparador(toInsert, lista_iterador_elemento_actual(iterador)) != 0){

			posicion++;
			lista_iterador_avanzar(iterador);
		}

		Actual = lista_iterador_elemento_actual(iterador);

		if(anterior){
			*anterior = Actual->value;

		}
		
		free((void *)toInsert->key);
		free(toInsert);


		Actual->value = elemento;
		lista_iterador_destruir(iterador);
	}

	




	return hash;
}

void *hash_quitar(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return NULL;

	keyvalue_t *claveABuscar = creacion_de_keyvalue(clave, NULL);
	
	size_t indice = funcion_hash(clave, hash->capacidad);
	

	lista_t *listaDondeBuscar = hash->vector[indice];

	
	if(!hash_contiene(hash, clave)){
		free((void *)claveABuscar->key);
		free(claveABuscar);
		return NULL;
	}
	
	

	size_t posicion = lista_buscar_posicion(listaDondeBuscar, comparador, claveABuscar);
	keyvalue_t *elementoEncontrado = (keyvalue_t *)lista_quitar_de_posicion(listaDondeBuscar, posicion);


	void *value  = elementoEncontrado->value;
	
	
	free((void *)claveABuscar->key);
	free(claveABuscar);
	
	free((void *)elementoEncontrado->key);
	free(elementoEncontrado);


	hash->cantElementos--;

	return value;
}

void *hash_obtener(hash_t *hash, const char *clave)
{
	if (!hash || !hash_contiene(hash, clave)){
		
		return NULL;
	}
	keyvalue_t *claveABuscar = creacion_de_keyvalue(clave, NULL);
	

	size_t indice = funcion_hash(clave, hash->capacidad);
	lista_t *listaDondeBuscar = (hash->vector)[indice];
	
	keyvalue_t *elementoAdevolver = lista_buscar_elemento(listaDondeBuscar, comparador, claveABuscar);

	
	free((void *)claveABuscar->key);
	free(claveABuscar);

	
	return elementoAdevolver->value;
	
	
	
}

bool hash_contiene(hash_t *hash, const char *clave)
{
	if (!hash || !clave)
		return false;
	
	keyvalue_t *claveABuscar = creacion_de_keyvalue(clave, NULL);
	

	size_t indice = funcion_hash(clave, hash->capacidad);
	lista_t *listaDondeBuscar = hash->vector[indice];


	
	if(!lista_buscar_elemento(listaDondeBuscar, comparador, claveABuscar)){
		

		free((void *)claveABuscar->key);
		free(claveABuscar);
		return false;
	}


	
	free((void *)claveABuscar->key);
	free(claveABuscar);
	return true;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	
	return hash->cantElementos;
}



void hash_destruir(hash_t *hash)
{	
	if(!hash){
		return;
	}

	for(size_t i = 0; i < hash->capacidad; i++){
		lista_t *lista = hash->vector[i];
		
		lista_destruir_todo(lista, funcion);
		
		
			
	}
	free(hash->vector);
	free(hash);

	
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash || !destructor){
		hash_destruir(hash);
		return;
		
	}
	

	for(size_t i = 0; i < hash->capacidad; i++){
		lista_t *lista = hash->vector[i];
		size_t tamanio = lista_tamanio(lista);

		for(size_t j = 0; j < tamanio; j++){
			keyvalue_t *par = (keyvalue_t *)lista_quitar(lista);
			
			
			destructor(par->value);
			free((void *)par->key);
			free(par);
			
		}
		free(lista);
		
	}
	free(hash->vector);
	free(hash);
}

size_t hash_con_cada_clave(hash_t *hash,
			   bool (*f)(const char *clave, void *valor, void *aux),
			   void *aux)
{	

	size_t n = 0;
	if (!hash || !f)

		return n;

	for(size_t i = 0; i < hash->capacidad; i++ ){
		lista_t *lista = (hash->vector)[i];
		size_t tamanio = lista_tamanio(lista);
		lista_iterador_t *iterador = lista_iterador_crear(lista);
		keyvalue_t *Actual = lista_iterador_elemento_actual(iterador);

		for(size_t j = 0; j < tamanio; j++ ){


			if(!f(Actual->key, Actual->value, aux)){
				n++;
				lista_iterador_destruir(iterador);
				return n;
			}	
			lista_iterador_avanzar(iterador);
			Actual = lista_iterador_elemento_actual(iterador);
			n++;
		}
		lista_iterador_destruir(iterador);
	}

	

	return n;
}
