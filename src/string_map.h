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
        Nodo(key_type& key,mapped_type* sig, Nodo* pad/*,Nodo* prox*/) : clave(key),significado(sig), padre(pad)/*,proximo(prox)*/{
            for(int i = 0; i < 27; i++){
                hijos[i] = nullptr;
            }
        };
        key_type clave;
        mapped_type* significado;
        Nodo* padre;
        //Nodo* proximo;
        Nodo* hijos[27];  // Pusimos 27 porque suponemos que las claves solo usan caracteres tradicionales (letras en espaniol).
    };

    size_type nroClaves;
    //Nodo* iniciosClaves[27] ;
    Nodo* raiz;

    //metodos privados
    Nodo* minimaClave(Nodo* nodo){
        Nodo* actual = nodo;
        int i;
        while(actual->significado == nullptr){
            i = 0;
            while(i<27){
                if(actual->hijos[i] != nullptr){
                    actual = actual->hijos[i];
                    break;
                }
                i++;
            }
        }
        return actual;
    }

    int charToInt(char ch){
        switch(ch){
            case 'a':
                return 0;
                break;
            case 'b':
                return 1;
                break;
            case 'c':
                return 2;
                break;
            case 'd':
                return 3;
                break;
            case 'e':
                return 4;
                break;
            case 'f':
                return 5;
                break;
            case 'g':
                return 6;
                break;
            case 'h':
                return 7;
                break;
            case 'i':
                return 8;
                break;
            case 'j':
                return 9;
                break;
            case 'k':
                return 10;
                break;
            case 'l':
                return 11;
                break;
            case 'm':
                return 12;
                break;
            case 'n':
                return 13;
                break;
            case 'q':
                return 14;
                break;
            case 'o':
                return 15;
                break;
            case 'p':
                return 16;
                break;
            case '@':
                return 17;
                break;
            case 'r':
                return 18;
                break;
            case 's':
                return 19;
                break;
            case 't':
                return 20;
                break;
            case 'u':
                return 21;
                break;
            case 'v':
                return 22;
                break;
            case 'w':
                return 23;
                break;
            case 'x':
                return 24;
                break;
            case 'y':
                return 25;
                break;
            case 'z':
                return 26;
                break;
        }
    }

    std::ostream& mostrarAux(Nodo* nodo, std::ostream &os){ //Le pasariamos raiz? (solo estoy volviendo a la version anterior de mostrar)
        int i = 0;
        if (nodo->significado != nullptr) {
            os << nodo->significado;
            os << " ";
            os << std::endl;
        }
        while (i < 27){
            if (nodo->hijos[i] != nullptr){
                mostrarAux(nodo->hijos[i], os);
            }
            i++;
        }
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

        //  value_type& operator*() const;
        value_type /* * */ operator->() const;

        friend class string_map;

    private:
        iterator(string_map::Nodo* n): valor(std::make_pair(n->clave,n->significado)), nodo(n){}
        Nodo* nodo;
        string_map<T>::value_type valor;

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
        //   value_type* operator->() const;

        friend class string_map;

    private:
        const_iterator(string_map::Nodo* n): nodo(n){}
        Nodo* nodo;
    };


    /** @brief Construye mapa vacio
     *
     * \complexity{\O(1)}
     */
    string_map(): nroClaves(0){
        raiz = new Nodo(nullptr,nullptr);
    }; //hace falta pasar el vector vacio o
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
        int i = 0;
        while (i<27){
            if (raiz->hijos[i] != nullptr){
                return iterator(minimaClave(raiz->hijos[i]));
            }
            i++;
        }
        return end();
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
    iterator find(const key_type &key){
        int i = 0;
        Nodo* actual = raiz;
        while (i < key.length()){
            if (actual->hijos[i] != nullptr){
                actual = actual->hijos[i];
            }else{
                break;
            }
            i++;
        }
        if (i = key.length()){
            return iterator(actual);
        }else{
            return end();
        }
    }

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
    pair<iterator,bool> insert(/*const*/ value_type* /*&*/value){  //OJO!

        //value_type = pair<string, T>
        int i = 0;
        int n = value->first.length(); //.
        bool inserto = false;

        Nodo* actual = raiz;
        while(i<n){ // Vamos recorriendo el trie letra a letra, viendo los que ya estan.
            if(actual->hijos[charToInt(value->first[i])] != nullptr){
                actual = actual->hijos[charToInt(value->first[i])];
                if (i == n-1){ //este caso es si recorrimos toda la palabra
                    if (actual->significado == nullptr){ //si ya existia y no tenia significado, le doy uno.
                        actual->significado = &value->second; //.
                        inserto = true;
                    } //Si tenia significado, este while termina, y no entro al proximo, devolviendo inserto = false y un iterador al nodo.
                }
                i++;
            }else{
                break;
            }
        }
        while(i<n){ //Si no llegue al ultimo en el while anterior, falta agregar!
            if (i != n-1){
                actual->hijos[charToInt(value->first[i])] = new Nodo/* * */(nullptr, actual);
            }else{ //Al llegar al ultimo, le doy el significado, y cambio inserto = true.
                actual->hijos[charToInt(value->first[i])] = new Nodo/* * */(&value->second, actual);
                inserto = true;
            }
            actual = actual->hijos[charToInt(value->first[i])];
            i++;
        }

        if (inserto) {
            nroClaves++;
        }

        return std::make_pair(iterator(actual), inserto); //ARREGLAR
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


//DEBUGGING! //MOSTRAR TIENE QUE TOMAR UN puntero a nodo y un ostream para poder llamarse recursivamente!!!

    std::ostream& mostrar(std::ostream &os){
        mostrarAux(raiz, os);
    }

};

template<class T>
typename string_map<T>::iterator& string_map<T>::iterator::operator++(){
    int i = 0;
    while(i < 27){
        if (nodo->hijos[i] != nullptr){
            return iterator(minimaClave(nodo->hijos[i]));
        }
        i++;
    }

    if(nodo->proximo != nullptr){
        return iterator(minimaClave(nodo->proximo));
    }else{
        Nodo* actual = nodo;
        while(actual->proximo == nullptr){
            if(nodo->padre != nullptr){
                actual = actual->padre;
            }else{
                return end();
            }
        }
        return iterator(minimaClave(actual->proximo));
    }
}

template<class T>
typename string_map<T>::iterator::value_type string_map<T>::iterator::operator->() const{
    return *nodo;
}



#endif //STRING_MAP_STRING_MAP_H