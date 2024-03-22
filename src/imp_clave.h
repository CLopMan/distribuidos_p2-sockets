#ifndef IMP_H

#define IMP_H


/**
 * Inicializa el servicio borrando todos los datos anteriores
 * 
 * @return devuelve 0 en caso de éxito y -1 en cualquier otro caso
*/
int init();

/**
 * En la tupla de clave *key* se guardan los valores value1 (str) y 
 * value2 (vector de N_value2 elementos de tipo double)
 * 
 * @param key clave qeu identifica la tupla
 * @param value1 string que para el primer valor de la tupla
 * @param N_value2 longitud del vector 
 * @param V_value2 vector de N_value2 doubles 
 * 
 * @return 0 en caso de éxito, -1 en cualquier otro caso
*/
int set_value(int key, char *value1, int N_value2, double *V_value2);


/**
 * Devuelve en value1, N_value2 y V_value2 los valores de la tupla caracterizada
 * por key
 * 
 * @param key clave que identifica la tupla
 * @param value1 dirección del contenedor para la string
 * @param N_value2 dirección del contenedor para la longitud del vector
 * @param V_value2 dirección del contenedor para el vector
 * 
 * @return 0 en caso de éxito, -1 en cualquier otro caso
*/
int get_value(int key, char *value1, int *N_value2, double *V_value2);

/**
 * modifica el primer valor y un valor del vector de una tupla
 * 
 * @param key clave que identifica la tupla
 * @param value1 string para el primer valor de la tupla
 * @param N_value2 índice
 * @param V_value2 valor a insertar en el vector
 * 
 * @return 0 en caso de éxito, -1 en cualquier otro caso
*/
int modify_value(int key, char *value1, int N_value2, double *V_value2);

/**
 * Elimina una tupla
 * 
 * @param key clave que identifica a la tupla
 * 
 * @return 0 en caso de éxito, -1 en cualquier otro caso
*/
int delete_key(int key);

/**
 * Comprueba la existencia de una tupla. 
 * 
 * @param key clave que identifica a la tupla
 * 
 * @return 1 en caso de que exista, 0 en caso contrario; -1 en caso de error
*/
int exist(int key); 


#endif