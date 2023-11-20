#include "pa2m.h"
#include <stdlib.h>


#include "src/hash.h"



void destructor(void *i){
	return;


}


void prueba_crear()
{

	hash_t *hash = hash_crear(3);
	
	int uno = 1;
	int dos = 2;

	int* anterior = NULL;  
	void* puntero_anterior = &anterior;


	const char* clave = "CLAVE 1";

	hash_insertar(hash, clave, &uno, &puntero_anterior );
	hash_insertar(hash, "CLAVE 2", &dos, &puntero_anterior );
	hash_insertar(hash, "CLAVE 3", &dos, &puntero_anterior );
	hash_insertar(hash, "CLAVE 4", &dos, &puntero_anterior );
	hash_insertar(hash, clave, &uno, &puntero_anterior );

	
	pa2m_afirmar(hash_cantidad(hash) == 4, "La cantidad de elementos deberia ser 4");
	pa2m_afirmar(dos ==  *(int *)hash_obtener(hash, "CLAVE 2"), "La clave 2 tiene el valor 2");
	pa2m_afirmar(uno ==  *(int *)hash_obtener(hash, clave), "La clave 1 tiene el valor 1");

	void *value1 = hash_quitar(hash, "CLAVE 1");
	pa2m_afirmar(*(int *)value1 == uno, "Se elimina CLAVE 1");
	pa2m_afirmar(!hash_obtener(hash, clave), "CLAVE 1 no se encuentra en el hash");
	
	pa2m_afirmar(!hash_quitar(hash, "CLAVE 1"), "No se puede eliminar una clave ya eliminada" );



	hash_quitar(hash, "CLAVE 3");
	pa2m_afirmar(hash_cantidad(hash) == 2, "Se elimina un elemento ahora la cantidad es 2");

	hash_quitar(hash, "CLAVE 128930");
	hash_insertar(hash, "CLAVE 2", &uno, &puntero_anterior );
	pa2m_afirmar(uno ==  *(int *)hash_obtener(hash, "CLAVE 2"), "Se actualizo al valor 1");
	hash_insertar(hash, "CLAVE 3", &uno, &puntero_anterior );
	hash_insertar(hash, "CLAVE 1", &dos, &puntero_anterior );
	hash_insertar(hash, "CLAVE 2", &uno, &puntero_anterior );
	hash_insertar(hash, "CLAVE 1", &dos, &puntero_anterior );



	hash_insertar(hash, "CLAVE 2", &dos, &puntero_anterior );
	pa2m_afirmar(dos ==  *(int *)hash_obtener(hash, "CLAVE 2"), "Se volvio a actualizar el valor a 2");
	
	

	hash_quitar(hash, "hola1");

	 
	
	
	hash_destruir(hash);
}


void pruebas_con_null(){
	hash_cantidad(NULL);
	hash_con_cada_clave(NULL, NULL, NULL);
	hash_contiene(NULL, NULL);
	hash_destruir(NULL);
	hash_destruir_todo(NULL, NULL);
	hash_insertar(NULL, NULL, NULL, NULL);
	hash_obtener(NULL, NULL);
	hash_quitar(NULL, NULL);


	hash_t *hash = hash_crear(0);

	hash_contiene(hash, NULL);
	
	hash_insertar(hash, NULL, NULL, NULL);
	hash_obtener(hash, NULL);
	hash_quitar(hash, NULL);


	hash_destruir(hash);

}



void pruebas_rehashing_y_muchos_elementos()
{
	hash_t *hash = hash_crear(0);

	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cuatro = 4;

	hash_insertar(hash, "CLAVE 1", &uno, NULL );
	hash_insertar(hash, "CLAVE 2", &dos, NULL );
	hash_insertar(hash, "CLAVE 3", &tres, NULL );
	hash_insertar(hash, "CLAVE 4", &cuatro, NULL );
	hash_insertar(hash, "CLAVE 5", &uno, NULL );
	hash_insertar(hash, "CLAVE 6", &uno, NULL );
	hash_insertar(hash, "CLAVE 7", &dos, NULL );
	hash_insertar(hash, "CLAVE 8", &uno, NULL );
	hash_insertar(hash, "CLAVE 9", &uno, NULL );
	hash_insertar(hash, "CLAVE 10", &uno, NULL );
	hash_insertar(hash, "CLAVE 11", &dos, NULL );
	hash_insertar(hash, "CLAVE 12", &uno, NULL );
	hash_insertar(hash, "A", &dos, NULL );
	hash_insertar(hash, "BA", &uno, NULL );
	hash_insertar(hash, "C", &uno, NULL );
	hash_insertar(hash, "DS", &uno, NULL );
	hash_insertar(hash, "E", &dos, NULL );
	hash_insertar(hash, "F", &uno, NULL );
	
	

	hash_destruir(hash);
	
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");
	prueba_crear();
	pruebas_con_null();
	pruebas_rehashing_y_muchos_elementos();

	return pa2m_mostrar_reporte();
}
