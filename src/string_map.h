#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;

// Nota de Doxygen

//falta una descripcion de la clase y el "se explica con". mirar Tabla.h//TODO



/**
 * Implementacion de map<string,T> sobre Trie
 * Asume de T:
 * - tiene constructor por copia
 * - tiene operador ==
 * - solo permite utilizar el operator[] si T tiene constructor por defecto
 */

template<typename T>
class string_map {
public:
    // Redeficinión de tipos usados por la clase
    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef size_t size_type;
public:
    //Subclases y estructuras públicas
    class iterator;
    class const_iterator;

    //Métodos públicos de la clase
  /** @brief Construye un string_map vacio
   *
   *\pre true
   *\post \P{res} \IGOBS vacio
   *
   *\complexity{\O(1)}
   */

    string_map(){};
    /** @brief Destruye mapa
     *
     *\complexity{\O(sn * S)}
     */
    ~string_map();
	/**
	 * @brief Constructor por copia de string_map
	 * @param str es el string_map que quiero copiar
	 *\pre str =  str0
	 *\post \P{res} \IGOBS str0
	 *
	 *\complexity{\O(sn * S)}
	 */

    string_map(const string_map &aCopiar);
    /** @brief Operador de asignacion
     *
     * \complexity{\O(sn * S)}
     */
    string_map &operator=(const string_map &otro){
        copiar(otro);
    }
    /**
     * @brief reviso si dos string_maps son iguales 
     * @param otro string_map
     * \pre  true
     * \post \P{res} \IGOBS  (this \IGOBS otro)
     *
     * \complexity{\O(sn * S)}
     */

    bool operator==(const string_map &otro) const {
        return revisarIgualdad(otro);
    }
    bool operator!=(const string_map &otro) const {
        return !revisarIgualdad(otro);
    }
    /**
     * @brief Count revisa si está o no la clave en el string_map.
     * @param key es el string clave del cual quiero ver la cantidad de apariciones (1 o 0)
     * \pre  true
     * \post \P{res} \IGOBS  beta(def?(key, this))
     *
     * \complexity{\O(S)}
     */

    size_type count(const key_type &key) const;
    /**
     *@brief Size devuelve el tamaño del string_map.
     *
     *\pre  true
     *\post \P{res} \IGOBS #claves(this)
     *
     *\complexity{\O(1)}
     */

    size_t size() const;
    /**
     *@brief Empty revisa si el string_map está vacio.
     *
     *\pre  true
     *\post \P{res} \IGOBS (#claves(this) \IGOBS 0)
     *
     *\complexity{\O(1)}
     */

    bool empty() const;
     /**
     * @brief Acceso/definición de pares clave/valor
     * @param key clave a acceder, si no existe, se crea
     *
     *\pre  true
     *\post {def?(key,this) \LAND   \P{res} \IGOBS obtener(key, this)
     *
     *\complexity{\O(S)}
     */

    mapped_type &operator[](const key_type &key) {
        Nodo *nodo = buscarNodo(key);
        if (nodo->significado==nullptr) {
            tamano++;
            nodo->significado = new contenedorSignificado();
        }
        return nodo->significado->valor;
    }
    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
	 *  @returns una referencia a la definicion
     *
     * \pre  \def?(key,this)
     * \post \P{res} \IGOBS obtener(key, this)

     *  \complexity{\O(S)}
     */
    string_map<T>::mapped_type &at(const key_type &key);
    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
	 *\pre def?(key,this)
     *\post \P{res} \IGOBS obtener(key, this)
     *
     *  \complexity{\O(S)}
     */
    const string_map<T>::mapped_type &at(const key_type &key) const;
    /**
     * @brief Limpia el mapa, lo deja solo con la raiz.
     * 
     *\pre  true
     *\post this \IGOBS vacio
     *
     *  \complexity{//TODO}
     */

    void clear();
    /** @brief Iterador al primer par <clave,significado> en orden lexicografico
     *\pre  true
   	 *\post Devuelve un iterador a la primera clave o end() si el mapa era vacio.
     *
     *  \complexity{\O(S)}
     */
    string_map<T>::iterator begin();
    /*@brief iterador al fin de la coleccion
     *
	 * \pre  true
     * \post @brief Devuelve un iterador al siguiente a la ultima clave
     *  \complexity{\O(1)}
     */
    iterator end();
	/**
     * 
     * \pre  true
     * \post Devuelve un iterador const a la primera clave.
     *
     * \complexity{\O(S)}
     */

    string_map<T>::const_iterator begin() const;
	/**
     * 
     *\pre  true
     *\post Devuelve un iterador const al siguiente a la ultima clave
     *
     *\complexity{\O(1)}
     */

    string_map<T>::const_iterator end() const;

	/**
	 * @brief 
	 * 
	 * \pre  true
	 * \post Devuelve un iterador const a la primera clave
	 *
	 * \complexity{\O(S)}
   	 */
    string_map<T>::const_iterator cbegin() const;
   /**
    * 
    * \pre  true
    * \post Devuelve un iterador que no modifica la estructura al siguiente a la ultima clave.
    *
    * \complexity{\O(1)}
    */
    string_map<T>::const_iterator cend() const;
    /**
     *@brief Busca una clave, devolviendo un iterador a esta.
     *@param key string es la clave a buscar
     *\pre  def?(key, this)
     *\post Devuelve un iterador a la clave parámetro.
     *
     *  \complexity{\O(S)}
     */

    string_map<T>::iterator find(const key_type &key);
	/**
     * @brief Busca una clave, devolviendo un iterador const a esta.
     * @param key string es la clave a buscar
     *\pre def?(key, this)
     *\post Devuelve un iterador que no modifica la estructura a la clave parámetro.
	 *
     *  \complexity{\O(S)}
     */
    string_map<T>::const_iterator find(const key_type &key) const;
    /**
     * @brief Define el string de la tupla parametro como clave y T como significado
     * @param value es tupla (string,T) , string siendo la clave y T el significado 
     *\pre  this = this0
     *\post def?(pi1(t), this) \LAND  pi2(res) \IGOBS \beta(NO(def?(pi1(t),this0))) y el pi1(res) es un iterador que apunta a la tupla parametro
     *
     *  \complexity{\O(S)}
     */

    pair<string_map<T>::iterator, bool> insert(const value_type &value);
    /**
     * @brief Busca y borra la clave pasada por parametro, devuelve si borro o no
     * @param key string es la clave a borrar
     *\pre  this = this0
     *\post \P{res} \IGOBS &beta (def?(key,this0)) \LAND  if (def?(key,this0)) then (this \IGOBS borrar(key,this0)) else this \IGOBS this0) //TODO arreglar ese if then else
     *
     *  \complexity{\O(S)}
     */

    string_map<T>::size_type erase(const key_type &key);
    /**
     * @brief Busca y borra la clave desde el iterador pasado por parámetro devolviendo un iterador al siguiente
     * @param pos es un iterador a la clave que quiero borrar
     * \pre  def?(pi1(actual(pos)),this)
     * \post Devuelve un iterador a la clave siguiente del iterador pasado por parametro.
	 *
     *  \complexity{\O(S)}
     */

    string_map<T>::iterator erase(iterator pos);

private:

	
   /** \name Representación
   * rep: string_map \TO bool\n
   * rep(e) \EQUIV 
   *  * La cantidad de nodos cuyo significado no es NULL es igual a tamaño
   *  * raiz != NULL
   *  * raiz->clave \IGOBS <>
   *  * (\FORALL Nodo n que pertenece al arbol) (n.clave \IGOBS <> \IMPLIES n.padre \IGOBS NULL) \LAND (\FORALL 0<=i<=95) n.hijos[i] != NULL \IMPLIES (n.hijos[i]->clave \IGOBS n.clave O intToChar(i))
   *  * (\FORALL Nodo n que pertenece al arbol) (\FORALL 0<=i<=95) n[i] \IGOBS NULL \IMPLIES n.sig != NULL
     

   Aclaracion: suponemos que existe el TAD Char. La funcion intToChar recibe un int y devuelve un char correspondiente (con i= 0 retorna el carácter numero 32 de ASCII, hasta el 95 que retorna el carácter 127).

Decidimos describir algunas partes del Invariante de Representación en lenguaje natural ya que para poder utilizar Existenciales o Para Todos sobre los nodos ibamos a requerir funciones del lenguaje de especificación. El problema con esto es que, para poder utilizarlas con los nodos, tambien ibamos a requerir una funcion de Abstracción, un invariante de Representación y una especificacion de nuestro TAD Nodo. Consideramos que esto escapaba a la consigna del trabajo Práctico.

Nota: Puede pasar que significado y cs no esten sincronizados.

Abs: string_map \TO Dicc(string,alfa)        {Rep(e)}\n 
   * (\FORALL e:estr(alfa)) Abs(e)\IGOBS d: Dicc(string,alfa) |\n (\FORALL s: string) def?(s,d) \EQUIV ((Existe un unico Nodo n) n pertenece al string map \LAND n.clave \IGOBS s \LAND n->significado != NULL \LAND ((n->significado).valor) \IGOBS obtener(s,d)
   *
   
   

   **/




    //Subclases y estructuras privadas
    struct Nodo;
    class contenedorSignificado;
    class claveSignificado;

    //Variables privadas de la clase
    Nodo *raiz = new Nodo("", nullptr);
    size_t tamano = 0;

    //Métodos privados de la clase
    int charToInt(char ch) const;
    string_map<T>::Nodo *buscarNodo(const key_type &key) const; //O(S)
    int medirRama(Nodo *parent) const;
    string_map<T>::Nodo *minimaClave(Nodo *nodo) const;
    bool revisarIgualdad(const string_map<T> &b) const;
    void borrarTodo(Nodo *desde);
    void copiar(const string_map<T> &aCopiar);
    void borrarRecursivo(Nodo *aBorrar);
    int cantHijos(Nodo *nodo);
};

#include "../src/string_map.hpp"

#endif //STRING_MAP_STRING_MAP_H
