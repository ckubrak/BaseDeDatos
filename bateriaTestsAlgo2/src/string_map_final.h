#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>
#include "string_map.h"

using std::string;
using std::vector;
using std::pair;

// Nota de Doxygen
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
        /** @brief Construye mapa vacio
         *
         * \complexity{\O(1)}
         */
        string_map();
        /** @brief Destruye mapa
         *
         * \complexity{\O(sn * S)}
         */
        ~string_map(); //TODO
        /** @brief Constructor por copia
         *
         * \complexity{\O(sn * S)}
         */
        string_map(const string_map &aCopiar); //TODO
        /** @brief Operador de asignacion
         *
         * \complexity{\O(sn * S)}
         */
        string_map &operator=(const string_map &); //TODO
        /** @brief Operadores de comparacion
         *
         * \complexity{\O(sn * S)}
         */
        bool operator==(const string_map &otro) const{
            const_iterator c1 = begin();
            const_iterator c2 = otro.begin();
            if(size() != otro.size()) {
                return false;
            }
            while(c1.nodo!=nullptr && c2.nodo!=nullptr){
                if(c1.nodo->clave != c2.nodo->clave){
                    return false;
                }
                ++c1;
                ++c2;
            }
            return true;
        }
        bool operator!=(const string_map &otro) const{
            //TODO ver cómo hacer para reusar el operador == sin segfaults
            const_iterator c1 = begin();
            const_iterator c2 = otro.begin();
            if(size() != otro.size()) {
                return true;
            }
            while(c1.nodo!=nullptr && c2.nodo!=nullptr){
                if(c1.nodo->clave != c2.nodo->clave){
                    return true;
                }
                ++c1;
                ++c2;
            }
            return false;
        }
        /** @brief Cantidad de apariciones de la clave (0 o 1)
         *  @param key clave a buscar
         *
         *  \complexity{\O(S)}
         */
        size_type count(const key_type &key) const; //TODO
        /** @brief Devuelve cantidad de claves definidas */
        size_t size() const;
        /** @brief devuelve true si size() == 0 */
        bool empty() const;
        /** @brief Acceso / definición de pares clave/valor
         *  @param key clave a acceder, si no existe, se crea
         *  @returns una referencia a la definicion.
         *
         *  \complexity{\O(S)}
         */
        mapped_type &operator[](const key_type &key){
            Nodo* nodo = buscarNodo(key);
            if(!nodo->estaDef){
                tamano++;
                nodo->estaDef=true;
            }
            return nodo->significado;
        }
        /** @brief Acceso a una clave sin modificar mapa
         *  @param key clave a acceder que debe existir previamente
         *  @returns una referencia a la definicion.
         *
         *  \complexity{\O(S)}
         */
        mapped_type &at(const key_type &key);
        /** @brief Acceso a una clave sin modificar mapa
         *  @param key clave a acceder que debe existir previamente
         *  @returns una referencia const a la definicion.
         *
         *  \complexity{\O(S)}
         */
        const mapped_type &at(const key_type &key) const;
        /** @brief Vacia el mapa */
        void clear();
        /** @brief iterador al primer par <clave,significado> en orden lexicografico
         *  @returns iterador al elemento o end() si el mapa era vacio
         *
         *  \complexity{\O(S)}
         */
        iterator begin(); //TODO
        /*  @brief iterador al fin de la coleccion
         *
         *  \complexity{\O(S)}
         */
        iterator end(); //TODO
        string_map<T>::const_iterator begin() const;
        string_map<T>::const_iterator end() const; //TODO
        const_iterator cbegin() const; //TODO
        const_iterator cend() const; //TODO
        /** @brief busca una clave
         *  @param key clave a buscar
         *  @returns un iterador al par <clave, significado>
         *
         *  \complexity{\O(S)}
         */
        iterator find(const key_type &key); //TODO
        /** @brief busca una clave
         *  @param key clave a buscar
         *  @returns un iterador const al par <clave, significado>
         *
         *  \complexity{\O(S)}
         */
        const_iterator find(const key_type &key) const; //TODO
        /** @brief insercion
        * @param value par <clave,significado> a insertar, no debia existir la clave
        * @returns un par con el iterador resultante y
        *
        * \complexity{\O(S + copy(value_type))}
        */
        pair<string_map<T>::iterator, bool> insert(const value_type &value); //TODO (Ojo?)
        /** @brief eliminar una clave
         *  @param key clave a eliminar
         *  @returns cantidad de elementos eliminados
         *
         *  \complexity{\O(S)}
         */
        size_type erase(const key_type &key); //TODO
        /** @brief eliminar una clave mediante irerador
        *  @param pos iterador apuntando a clave a eliminar
        *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
        *
        *  \complexity{\O(S)}
        */
        iterator erase(iterator pos); //TODO

    private:
        //Subclases y estructuras privadas
        struct Nodo;

        //Variables privadas de la clase
        Nodo *raiz;
        size_t tamano = 0;

        //Métodos privados de la clase
        int charToInt(char ch) const;
        string_map<T>::Nodo* buscarNodo(const key_type &key);
        int medirRama(Nodo* parent) const;
        string_map<T>::Nodo* minimaClave(Nodo *nodo) const;
};

#include "../src/string_map_final.hpp"

#endif //STRING_MAP_STRING_MAP_H
