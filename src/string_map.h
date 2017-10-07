#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;

/**
 * Implementacion de map<string,T> sobre Trie
 * Asume de T:
 * - tiene constructor por copia
 * - tiene operador ==
 * - solo permite utilizar el operator[] si T tiene constructor por defecto
 */
template < typename T >
class string_map {

    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;  //para que usamos value_type?
    typedef size_t size_type;

private:

    struct Nodo{
        Nodo(key_type id, mapped_type* sig, Nodo* pad,Nodo* prox) :  id(id), significado(sig), padre(pad),proximo(prox){};
        key_type id;
        mapped_type* significado;
        Nodo* padre;
        Nodo* proximo;
        Nodo* hijos[27];  // Pusimos 27 porque suponemos que las claves solo usan caracteres tradicionales (letras en espaniol).
    };

    size_type nroClaves;
    Nodo* iniciosClaves[27] ;

    //metodos privados
    Nodo* minimaClave(Nodo* nodo){
        /*Nodo* actual = nodo;
        while(actual->significado == nullptr){
            actual = actual.hijos[0];
        }
        return actual;*/

        //arreglarlo porque ahora hijos es un array!
    }


public:


    class iterator{
    public:
        typedef T value_type;
        iterator(const iterator&){};
        iterator& operator=(const iterator&);

        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;

        iterator& operator++();
        iterator operator++(int);

        value_type& operator*() const;
        value_type /* * */ operator->() const;

        friend class string_map;

    private:
        iterator(string_map::Nodo* n): nodo(n){}
        Nodo* nodo;

    };

    class const_iterator{
    public:
        typedef T value_type;
        const_iterator(const const_iterator&){};
        const_iterator& operator=(const const_iterator&);

        bool operator==(const const_iterator&) const;
        bool operator!=(const const_iterator&) const;

        const_iterator& operator++();
        const_iterator operator++(int);

        value_type& operator*() const;
        value_type* operator->() const;

        friend class string_map;

    private:
        const_iterator(string_map::Nodo* n): nodo(n){}
        Nodo* nodo;
    };


    /** @brief Construye mapa vacio
     *
     * \complexity{\O(1)}
     */
    string_map(): nroClaves(0){}; //hace falta pasar el vector vacio o
    //se inicializa solo?

    /** @brief Destruye mapa
     *
     * \complexity{\O(sn * S)}
     */
    ~string_map(){
        return;
    }

    /** @brief Constructor por copia
     *
     * \complexity{\O(sn * S)}
     */
    string_map(const string_map &);

    /** @brief Operador de asignacion
     *
     * \complexity{\O(sn * S)}
     */
    string_map& operator=(const string_map &);

    /** @brief Operadores de comparacion
     *
     * \complexity{\O(sn * S)}
     */
    bool operator==(const string_map& otro) const;
    bool operator!=(const string_map& otro) const {return !(*this == otro);}

    /** @brief Cantidad de apariciones de la clave (0 o 1)
     *  @param key clave a buscar
     *
     *  \complexity{\O(S)}
     */
    size_type count(const key_type &key) const;

    /** @brief Devuelve cantidad de claves definidas */
    size_t size() const{
        return nroClaves;
    }

    /** @brief devuelve true si size() == 0 */
    bool empty() const{
        return nroClaves == 0;
    }


    /** @brief Acceso / definición de pares clave/valor
     *  @param key clave a acceder, si no existe, se crea
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type &operator[](const key_type &key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type& at(const key_type& key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
     *  \complexity{\O(S)}
     */
    const mapped_type& at(const key_type& key) const;

    /** @brief Vacia el mapa */
    void clear();

    // Accesos con iteradores

    /** @brief iterador al primer par <clave,significado> en orden lexicografico
     *  @returns iterador al elemento o end() si el mapa era vacio
     *
     *  \complexity{\O(S)}
     */
    iterator begin(){
        if(nroClaves == 0){
            return end();
        }
        else{
            Nodo* actual = iniciosClaves[0];
            while(actual->significado == nullptr){
                actual = actual->hijos[0];
            }
            return iterator(actual);
        }
    }

    /*  @brief iterador al fin de la coleccion
     *
     *  \complexity{\O(S)}
     */
    iterator end(){
        return iterator(nullptr);
    }

    /// Versiones const de begin/end
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador al par <clave, significado>
     *
     *  \complexity{\O(S)}
     */
    iterator find(const key_type &key);

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador const al par <clave, significado>
     *
     *  \complexity{\O(S)}
     */
    const_iterator find(const key_type &key) const;

    /** @brief insercion
     * @param value par <clave,significado> a insertar, no debia existir la clave
     * @returns un par con el iterador resultante y
     *
     * \complexity{\O(S + copy(value_type))}
     */
    pair<iterator,bool> insert(const value_type &value){

    }

    /** @brief eliminar una clave
     *  @param key clave a eliminar
     *  @returns cantidad de elementos eliminados
     *
     *  \complexity{\O(S)}
     */
    size_type erase(const key_type& key);

    /** @brief eliminar una clave mediante irerador
     *  @param pos iterador apuntando a clave a eliminar
     *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
     *
     *  \complexity{\O(S)}
     */
    iterator erase(iterator pos);


};

template<class T>
typename string_map<T>::iterator& string_map<T>::iterator::operator++(){
    /*if (nodo.hijos.size() > 0) {
        return iterator(minimaClave(nodo.hijos[0]));
    }else if(nodo.proximo != nullptr){
        return iterator(minimaClave(nodo.proximo));
    }else{
        Nodo* actual = nodo;
        while(actual->proximo == nullptr){
            if(nodo.padre != nullptr){
                actual = actual->padre;
            }else{
                return end();
            }
        }
        return iterator(minimaClave(actual->proximo));
    }

     arreglarlo porque ahora hijos es un array!
    */
}

template<class T>
typename string_map<T>::iterator::value_type string_map<T>::iterator::operator->() const{
    return *nodo;
}


#endif //STRING_MAP_STRING_MAP_H
