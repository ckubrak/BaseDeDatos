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


template<typename T>
class string_map {

    // Redeficinión de tipos usados por la clase

    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef size_t size_type;

private:

    struct Nodo {
        key_type clave;
        bool estaDef;
        mapped_type significado;
        Nodo *padre;
        Nodo *hijos[96];  // Son 128 - 32 caracteres
        Nodo(const key_type &key, bool def, Nodo *pad) : clave(key), estaDef(def), padre(pad) {
            for (int i = 0; i < 96; i++) {
                hijos[i] = nullptr;
            }
        };

        value_type *cs = nullptr;

        value_type &claveSignificado() {
            assert(estaDef);
            if (cs == nullptr) cs = new value_type(clave, significado);
            return (*cs);
        }

    };

    size_type nroClaves;
    Nodo *raiz;

    //metodos privados
    Nodo *minimaClave(Nodo *nodo) {
        Nodo *actual = nodo;
        int i;
        while (!actual->estaDef) {
            i = 0;
            while (i < 96) {
                if (actual->hijos[i] != nullptr) {
                    actual = actual->hijos[i];
                    break;
                }
                i++;
            }
        }
        return actual;
    }

    int charToInt(char ch) const {
        return (int) ch - 32;
    }


    int cantHijos(Nodo *nodo) {
        int i = 0;
        int son = 0;
        while (i < 96) {
            if (nodo->hijos[i] != nullptr) {
                son++;
            }
            i++;
        }
        return son;
    }


    size_type borrarRecursivo(/*size_type k,*/ Nodo *aBorrar) {
        size_type i = 1;
        Nodo *hastaDondeBorro = aBorrar->padre;
        //Busco hastaDondeBorro (voy subiendo y borrando hasta llegar a ese, es medio negro.)
        while (cantHijos(hastaDondeBorro) < 2 && !(hastaDondeBorro->estaDef) &&
               !(hastaDondeBorro == raiz)) { // < 2 o == 1?
            delete hastaDondeBorro;
            hastaDondeBorro = hastaDondeBorro->padre;
            i++; //Sumo porque es otro mas que voy a tener que borrar.
        }
        hastaDondeBorro->hijos[charToInt(aBorrar->clave[aBorrar->clave.length() - i])] = nullptr;
        //es -i porque resta uno por cada nodo que borre hasta llegar a hastaDondeBorro.
        //aBorrar->clave.length()-i me dice qué numero de letra tengo que ver en la palabra
        //aBorrar->clave[aBorrar->clave.length()-i] me dice qué letra es.
        //charToInt(aBorrar->clave[aBorrar->clave.length()-i]) me da el numero en el abecedario que representa a esa letra (ej. a = 0).


        delete aBorrar;
        return i;
    }

    std::ostream &mostrarAux(Nodo *nodo,
                             std::ostream &os) { //Le pasariamos raiz? (solo estoy volviendo a la version anterior de mostrar)
        /*int i = 0;
            os << nodo->clave;
            os << std::endl;
        if (nodo->estaDef) {
            os << nodo->significado;
            os << " ";
            os << std::endl;
        }
        while (i < 96){
            if (nodo->hijos[i] != nullptr){
                mostrarAux(nodo->hijos[i], os);
            }
            i++;
        }*/


        os << nodo->clave;
        if (nodo->estaDef) {
            os << ", ";
            os << nodo->significado;
        } else {
            os << ",";
            os << "@";
        }
        os << "[";
        int j = 0;
        while (j < 96) {
            if (nodo->hijos[j] != nullptr) {
                os << nodo->hijos[j]->clave;
                os << ",";
            }
            j++;
        }
        os << "]";
        os << std::endl;
        int i = 0;
        while (i < 96) {
            if (nodo->hijos[i] != nullptr) {
                mostrarAux(nodo->hijos[i], os);
            }
            i++;
        }
    }


public:
    class iterator {
    public:
        typedef string_map<T>::value_type value_type;

        iterator() : nodo(nullptr) {};

        iterator(const iterator &otro) {
            nodo = otro.nodo;
        }

        iterator &operator=(const iterator &otro) {
            nodo = otro.nodo;
        }

        bool operator==(const iterator &otro) const {
            return (nodo == otro.nodo);
        }

        bool operator!=(const iterator &otro) const {
            return (nodo != otro.nodo);
        }

        iterator &operator++() {
            int i = 0;
            while (i < 96) {
                if (nodo->hijos[i] != nullptr) {
                    Nodo *actual = nodo->hijos[i];
                    int k;
                    while (!actual->estaDef) {
                        k = 0;
                        while (k < 96) {
                            if (actual->hijos[k] != nullptr) {
                                actual = actual->hijos[k];
                                break;
                            }
                            k++;
                        }
                    }
                    nodo = actual;
                    return *this;
                    //return iterator(minimaClave(nodo->hijos[i]));
                }
                i++;
            }

            Nodo *actual = nodo->padre;
            int k;
            int j = ((int) (nodo->clave[nodo->clave.length() - 1])) -
                    31;//Quiero ver los hijos del padre del nodo, desde la ultima letra
            // I.e. Si la ultima letra de la clave es B, empiezo a ver los hijos del padre desde la C.
            while (actual != nullptr) {
                while (j < 96) {
                    if (actual->hijos[j] != nullptr) {
                        actual = actual->hijos[j];
                        while (!(actual->estaDef)) {
                            k = 0;
                            while (k < 96) {
                                if (actual->hijos[k] != nullptr) {
                                    actual = actual->hijos[k];
                                    break;
                                }
                                k++;
                            }
                        }
                        nodo = actual;
                        return *this;
                        //return iterator(minimaClave(nodo->hijos[i]));
                    }
                    j++;
                }
                j = ((int) (actual->clave[actual->clave.length() - 1])) - 31;
                actual = actual->padre;
            }
            nodo = nullptr;
            return *this;
        }

        iterator operator++(int) {
            iterator resultado(*this);
            ++(*this);
            return resultado;
        }

        value_type &operator*() const {
            return nodo->claveSignificado();
        }

        value_type *operator->() const {
            return (&operator*());
        }

        friend class string_map;

    private:
        iterator(string_map::Nodo *n) : nodo(n) {};
        Nodo *nodo;
    };

    class const_iterator {
    public:
        typedef string_map<T>::value_type value_type;

        const_iterator() : nodo(nullptr) {};

        const_iterator(const const_iterator &otro) {
            nodo = otro.nodo;
        }

        const_iterator &operator=(const const_iterator &otro) {
            nodo = otro.nodo;
        }

        bool operator==(const const_iterator &otro) const {
            return (nodo == otro.nodo);
        }

        bool operator!=(const const_iterator &otro) const {
            return (nodo != otro.nodo);
        }

        const_iterator &operator++() {
            int i = 0;
            while (i < 96) {
                if (nodo->hijos[i] != nullptr) {
                    Nodo *actual = nodo->hijos[i];
                    int k;
                    while (!actual->estaDef) {
                        k = 0;
                        while (k < 96) {
                            if (actual->hijos[k] != nullptr) {
                                actual = actual->hijos[k];
                                break;
                            }
                            k++;
                        }
                    }
                    nodo = actual;
                    return *this;
                    //return iterator(minimaClave(nodo->hijos[i]));
                }
                i++;
            }

            Nodo *actual = nodo->padre;
            int k;
            int j = ((int) (nodo->clave[nodo->clave.length() - 1])) -
                    31;//Quiero ver los hijos del padre del nodo, desde la ultima letra
            // I.e. Si la ultima letra de la clave es B, empiezo a ver los hijos del padre desde la C.
            while (actual != nullptr) {
                while (j < 96) {
                    if (actual->hijos[j] != nullptr) {
                        actual = actual->hijos[j];
                        while (!(actual->estaDef)) {
                            k = 0;
                            while (k < 96) {
                                if (actual->hijos[k] != nullptr) {
                                    actual = actual->hijos[k];
                                    break;
                                }
                                k++;
                            }
                        }
                        nodo = actual;
                        return *this;
                        //return iterator(minimaClave(nodo->hijos[i]));
                    }
                    j++;
                }
                j = ((int) (actual->clave[actual->clave.length() - 1])) - 31;
                actual = actual->padre;
            }
            nodo = nullptr;
            return *this;
        }

        const_iterator operator++(int) {
            iterator resultado(*this);
            ++(*this);
            return resultado;
        }

        /*value_type& operator*() const{
			return nodo->claveSignificado();
		}
        value_type* operator->() const{
			return (&operator*());
		}*/

        friend class string_map;

    private:
        const_iterator(string_map::Nodo *n) : nodo(n) {}

        const Nodo *nodo;
    };


    /** @brief Construye mapa vacio
     *
     * \complexity{\O(1)}
     */
    string_map() : nroClaves(0) {
        raiz = new Nodo("", false, nullptr);
    };

    /** @brief Destruye mapa
     *
     * \complexity{\O(sn * S)}
     */
    ~string_map() { //TODO
        return;
    }

    /** @brief Constructor por copia
     *
     * \complexity{\O(sn * S)}
     */
    string_map(const string_map &aCopiar) {
        for (auto e : aCopiar) {
            insert(e);
        }
    }

    /** @brief Operador de asignacion
     *
     * \complexity{\O(sn * S)}
     */
    string_map &operator=(const string_map &); //TODO

    /** @brief Operadores de comparacion
     *
     * \complexity{\O(sn * S)}
     */
    bool operator==(const string_map &otro) const; //TODO
    bool operator!=(const string_map &otro) const { return !(*this == otro); }

    /** @brief Cantidad de apariciones de la clave (0 o 1)
     *  @param key clave a buscar
     *
     *  \complexity{\O(S)}
     */
    size_type count(const key_type &key) const {
        if (key == "") {
            return raiz->estaDef;
        }
        Nodo *actual = raiz;
        int i = 0;
        while (i < key.length() - 1) {
            if (actual->hijos[charToInt(key[i])] == nullptr) {
                return 0;
            }
            actual = actual->hijos[charToInt(key[i])];
            i++;
        }
        return (actual->estaDef ? 1 : 0);
    }

    /** @brief Devuelve cantidad de claves definidas */
    size_t size() const {
        return nroClaves;
    }

    /** @brief devuelve true si size() == 0 */
    bool empty() const {
        return nroClaves == 0;
    }


    /** @brief Acceso / definición de pares clave/valor
     *  @param key clave a acceder, si no existe, se crea
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type &operator[](const key_type &key) {
        if (key == "") {
            raiz->estaDef = true;
        }
        //Si no está
        if (count(key) == 0) {
            mapped_type sig;
            value_type claSig(key, sig);
            insert(claSig);
        }

        int i = 0;
        Nodo *actual = raiz;
        while (i < key.length() - 1) {
            actual = actual->hijos[charToInt(key[i])];
            i++;
        }
        return actual->significado;
    }

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia a la definicion.
     *
     *  \complexity{\O(S)}
     */
    mapped_type &at(const key_type &key) {
        Nodo *actual = raiz;
        int i = 0;
        //Busco la clave.
        while (i < key.length()) {
            actual = actual->hijos[charToInt(key[i])];
            i++;
        }
        return actual->significado;
    }

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
     *  \complexity{\O(S)}
     */
    const mapped_type &at(const key_type &key) const {
        Nodo *actual = raiz;
        int i = 0;
        while (i < key.length()) {
            actual = actual->hijos[charToInt(key[i])];
            i++;
        }
        return actual->significado;
    }

    /** @brief Vacia el mapa */
    void clear() {
        //suponiendo que el iterator++ anda bien
        iterator aBorrar = begin();
        while (aBorrar != end()) {
            aBorrar = erase(aBorrar);
        }
    }

    // Accesos con iteradores

    /** @brief iterador al primer par <clave,significado> en orden lexicografico
     *  @returns iterador al elemento o end() si el mapa era vacio
     *
     *  \complexity{\O(S)}
     */
    /*iterator begin(){
        int i = 0;
        while (i<96){
            if (raiz->hijos[i] != nullptr){
                return iterator(minimaClave(raiz->hijos[i]));
            }
            i++;
        }
        return end();
    }*/

    /*  @brief iterador al fin de la coleccion
     *
     *  \complexity{\O(S)}
     */
    iterator end() {
        return iterator(raiz);
    }

    /// Versiones const de begin/end

    const_iterator begin() const {
        int i = 0;
        while (i < 96) {
            if (raiz->hijos[i] != nullptr) {
                return const_iterator(minimaClave(raiz->hijos[i]));
            }
            i++;
        }
        return end();
    }

    const_iterator end() const {
        return const_iterator(raiz);
    }

    const_iterator cbegin() const;

    const_iterator cend() const;

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador al par <clave, significado>
     *
     *  \complexity{\O(S)}
     */
    iterator find(const key_type &key) {
        int i = 0;
        Nodo *actual = raiz;
        while (i < key.length()) {
            if (actual->hijos[i] != nullptr) {
                actual = actual->hijos[i];
            } else {
                break;
            }
            i++;
        }
        if (i = key.length()) {
            return iterator(actual);
        } else {
            return end();
        }
    }

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador const al par <clave, significado>
     *
     *  \complexity{\O(S)}
     */
    const_iterator find(const key_type &key) const;//TODO

    /** @brief insercion
     * @param value par <clave,significado> a insertar, no debia existir la clave
     * @returns un par con el iterador resultante y
     *
     * \complexity{\O(S + copy(value_type))}
     */

    pair<iterator, bool> insert(const value_type &value) {  //OJO!

        if (value.first == "") {
            raiz->significado = value.second;
            raiz->estaDef = true;
        }
        //value_type = pair<string, T>
        int i = 0; //Iterador del while
        int n = value.first.length() - 1; //n es el largo de la clave. //.
        bool inserto = false;

        Nodo *actual = raiz;
        string s = ""; //vamos a ir armando el string clave para cada nodo
        while (i < n) { // Vamos recorriendo el trie letra a letra, viendo los que ya estan.
            if (actual->hijos[charToInt(value.first[i])] != nullptr) {
                s += value.first[i];
                actual = actual->hijos[charToInt(value.first[i])];
                if (i == n - 1) { //este caso es si recorrimos toda la palabra
                    if (!actual->estaDef) { //si no esta definida, le doy un significado.
                        actual->significado = value.second; //.
                        actual->estaDef = true;
                        inserto = true;
                    } //Si tenia significado, este while termina, y no entro al proximo, devolviendo inserto = false y un iterador al nodo.
                }
                i++;
            } else {
                break;
            }
        }

        while (i < n) { //Si no llegue al ultimo en el while anterior, falta agregar!
            s += value.first[i]; //Esto me va a armando la clave de los nodos.
            if (i != n - 1) {
                actual->hijos[charToInt(value.first[i])] = new Nodo(s, false, actual);
            } else { //Al llegar al ultimo, le doy el significado, y cambio inserto = true.
                actual->hijos[charToInt(value.first[i])] = new Nodo(value.first, true, actual);
                actual->hijos[charToInt(value.first[i])]->significado = value.second;
                inserto = true;
            }
            actual = actual->hijos[charToInt(value.first[i])];
            i++;
        }

        if (inserto) {
            nroClaves++;
        }

        return std::make_pair(iterator(actual), inserto);
    }

    /** @brief eliminar una clave
     *  @param key clave a eliminar
     *  @returns cantidad de elementos eliminados
     *
     *  \complexity{\O(S)}
     */
    size_type erase(const key_type &key) {
        //Caso 1: No es hoja: estaDef -> !estaDef.
        //Caso 2: Es hoja: BorrarAux(padres hasta un nodo que tenga hijos u otra clave). Recursivamente?
        nroClaves--;
        int i = 0;
        Nodo *actual = raiz;
        while (i < key.length()) { //Busco letra por letra hasta llegar a la clave a borrar.
            actual = actual->hijos[charToInt(key[i])];
            i++;
        }

        if (cantHijos(actual) > 0) { //Caso 1
            actual->estaDef = false;
            return 0;
        } else { //Caso 2
            size_type k = 1;
            return borrarRecursivo(actual);
        }
    }

    /** @brief eliminar una clave mediante irerador
     *  @param pos iterador apuntando a clave a eliminar
     *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
     *
     *  \complexity{\O(S)}
     */
    iterator erase(iterator pos) { //TODO

    }


//DEBUGGING! //MOSTRAR TIENE QUE TOMAR UN puntero a nodo y un ostream para poder llamarse recursivamente!!!

    std::ostream &mostrar(std::ostream &os) {
        mostrarAux(raiz, os);
    }

};

/*template<class T>
typename string_map<T>::iterator& string_map<T>::iterator::operator++(){
	std::cout<<"entre"<<std::endl;
    int i = 0;
    while(i < 96){
        if (nodo->hijos[i] != nullptr){
			nodo = minimaClave(nodo->hijos[i]);
			std::cout<<"puto"<<std::endl;
			return *this;
            //return iterator(minimaClave(nodo->hijos[i]));
        }
        i++;
    }

    Nodo* actual = nodo->padre;
    int j = charToInt(nodo->clave[nodo->clave.length()-1])+1;//Quiero ver los hijos del padre del nodo, desde la ultima letra
    // I.e. Si la ultima letra de la clave es B, empiezo a ver los hijos del padre desde la C.
    while (actual != nullptr){
        while(j<96){
            if(actual->hijos[j] != nullptr){
                return iterator(minimaClave(actual->hijos[j]));
            }
            j++;
        }
        j = charToInt(actual->clave[actual->clave.length()-1]) + 1;
        actual = actual->padre;
    }
    return end();
}*/




#endif //STRING_MAP_STRING_MAP_H
