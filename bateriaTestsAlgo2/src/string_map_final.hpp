#include "string_map_final.h"

template<typename T>
class string_map<T>::contenedorSignificado{
public:
    T valor;

    T &operator=(const T &a){
        valor = a;
    }

    contenedorSignificado(){}

    contenedorSignificado(T &a):valor(a){}
};

// ========= Struct Nodo de String_Map =========
template<typename T>
struct string_map<T>::Nodo {
    //Variables internas
    bool estaDef = false;
    key_type clave;
    //mapped_type significado;
    contenedorSignificado* significado = nullptr;
    Nodo *padre;
    Nodo *hijos[96];  // Son 128 - 32 caracteres
    value_type *cs = nullptr;

    //Constructor del Nodo indefinido
    Nodo(const key_type &key, bool def, Nodo *pad) : clave(key), estaDef(def), padre(pad) {
        for (int i = 0; i < 96; i++) {
            hijos[i] = nullptr;
        }
    };

    //Operación de asignado de significado al nodo
    bool definir(mapped_type _sig) {
        bool acabaDeDefinirlo = !estaDef;
        estaDef = true;
        if(significado == nullptr){
            significado = new contenedorSignificado(_sig);
        }else{
            significado->valor = _sig;
        }
        if (cs != nullptr) delete cs;
        cs = new value_type(clave, significado->valor);
        return acabaDeDefinirlo;
    }

    //TODO el problema de nova es que se cagan los punteros D:
    //TODO NO PORQUE NO TENGO PUNTEROS A COSAS QUE ESTUVIERAN SIN DEFINIR :D

    //Operación de retorno del par <clave, significado>. Requiere que esté definido el nodo.
    value_type &claveSignificado() {
        assert(estaDef);
        return (*cs);
    }
};

// ========= Clase Iterator de String_Map =========
template<typename T>
class string_map<T>::iterator {
public:
    //Amigos de la clase
    friend class string_map;

    //Constructor vacío
    iterator() : nodo(nullptr) {};

    //Constructor por copia
    iterator(const iterator &otro) {
        nodo = otro.nodo;
    }

    //Operador =
    iterator &operator=(const iterator &otro) {
        nodo = otro.nodo;
    }

    //Operador ==
    bool operator==(const iterator &otro) const {
        return (nodo == otro.nodo);
    }

    //Operador !=
    bool operator!=(const iterator &otro) const {
        return (nodo != otro.nodo);
    }

    //Operador ++
    iterator &operator++() {
        //TODO
    }

    //Operador ++int
    iterator operator++(int) {
        iterator resultado(*this);
        ++(*this);
        return resultado;
    }

    //Operador *
    value_type &operator*() const {
        //TODO
    }

    //Operador ->
    value_type *operator->() const {
        //TODO
    }

private:
    //Variables privadas
    Nodo *nodo;

    //Constructor por parámetro
    iterator(Nodo *n) : nodo(n) {};
};

// ========= Clase Const_Iterator de String_Map =========
template<typename T>
class string_map<T>::const_iterator {
public:
    //Amigos de la clase
    friend class string_map;

    //Constructor vacío
    const_iterator() : nodo(nullptr) {};

    //Constructor por copia
    const_iterator(const const_iterator &otro) {
        nodo = otro.nodo;
    }

    //Operador =
    const_iterator &operator=(const const_iterator &otro) {
        nodo = otro.nodo;
    }

    //Operador ==
    bool operator==(const const_iterator &otro) const {
        return (nodo == otro.nodo);
    }

    //Operador !=
    bool operator!=(const const_iterator &otro) const {
        return (nodo != otro.nodo);
    }

    //Operador ++
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
        int j = ((int) (nodo->clave[nodo->clave.length() - 1])) - 31;
        //Quiero ver los hijos del padre del nodo, desde la ultima letra
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
                }
                j++;
            }
            j = ((int) (actual->clave[actual->clave.length() - 1])) - 31;
            actual = actual->padre;
        }
        nodo = nullptr;
        return *this;
    }

    //Operador ++int
    const_iterator operator++(int) {
        //TODO esto debería dejar de marcar error al hacer el operator ++
    }

    //Operador *
    value_type &operator*() const {
        //TODO
    }

    //Operador ->
    value_type *operator->() const {
        //TODO
    }

private:
    //Variables de la clase
    const Nodo *nodo;

    //Constructor por parámetro
    const_iterator(Nodo *n) : nodo(n) {}
};

// ========= Métodos públicos de String_Map =========
template<typename T>
string_map<T>::string_map() {
    //El string_map se crea con un nodo de string vacío, sin padre e indefinido.
    raiz = new Nodo("", false, nullptr);
}

template<typename T>
string_map<T>::~string_map() {
    //TODO debería ser una función recursiva que itere por los hijos borrando todo
}

template<typename T>
string_map<T>::string_map(const string_map &aCopiar) {
    //TODO debería ir avanzando con un iterador copiando todo
}

template<typename T>
int string_map<T>::charToInt(char ch) const {
    return (int) ch - 32;
}

template<typename T>
pair<typename string_map<T>::iterator, bool> string_map<T>::insert(const value_type &value) {
    Nodo *actual = buscarNodo(value.first);
    bool loAcabaDeDefinir = actual->definir(value.second);
    if (loAcabaDeDefinir) tamano++;
    return std::make_pair(iterator(actual), loAcabaDeDefinir);
};

template<typename T>
typename string_map<T>::size_type string_map<T>::count(const typename string_map<T>::key_type &key) const {
    Nodo *actual = raiz;
    //Voy viajando a donde hay que ubicarlo sino
    int posicionClave = 0; //Próxima letra a buscar
    string claveNueva = ""; //Este va a ser el string clave de cada nodo recorrido
    while (claveNueva != key) {
        //Actualizo la claveNueva
        claveNueva += key[posicionClave];
        //Próximo hijo a recorrer
        int posHijo = charToInt(key[posicionClave]);
        if (actual->hijos[posHijo] == nullptr) return 0;
        actual = actual->hijos[posHijo];
        posicionClave++;
    }
    return (actual->estaDef ? 1 : 0);
}

template<typename T>
size_t string_map<T>::size() const {
    return tamano;
}

template<typename T>
bool string_map<T>::empty() const {
    return size() == 0;
}

template<typename T>
typename string_map<T>::Nodo *string_map<T>::buscarNodo(const key_type &key) {
    //Busca el nodo que contenga la key. Si no está, lo crea.
    Nodo *actual = raiz;
    //Voy viajando a donde hay que ubicarlo sino
    string clave = key;
    int posicionClave = 0; //Próxima letra a buscar
    string claveNueva = ""; //Este va a ser el string clave de cada nodo recorrido
    while (claveNueva != clave) {
        //Actualizo la claveNueva
        claveNueva += clave[posicionClave];
        //Próximo hijo a recorrer
        int posHijo = charToInt(clave[posicionClave]);
        //Si este hijo está indefinido, lo defino
        if (actual->hijos[posHijo] == nullptr) actual->hijos[posHijo] = new Nodo(claveNueva, false, actual);
        //Me muevo a actual, completo claveNueva y aumento posicionClave
        posicionClave++;
        actual = actual->hijos[posHijo];
    }
    return actual;
}

template<typename T>
typename string_map<T>::Nodo *string_map<T>::minimaClave(Nodo *nodo) const {
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

template<typename T>
typename string_map<T>::const_iterator string_map<T>::begin() const {
    int i = 0;
    while (i < 96) {
        if (raiz->hijos[i] != nullptr) {
            return const_iterator(minimaClave(raiz->hijos[i]));
        }
        i++;
    }
    return end();
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::end() const {
    return nullptr;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) {
    Nodo *nodo = buscarNodo(key);
    return nodo->significado->valor;
}

template<typename T>
bool string_map<T>::revisarIgualdad(const string_map<T> &otro) const{
    const_iterator c1 = begin();
    const_iterator c2 = otro.begin();
    if (size() != otro.size()) return false;
    while (c1.nodo != nullptr && c2.nodo != nullptr) {
        if (c1.nodo->clave != c2.nodo->clave) {
            return false;
        }else if (c1.nodo->estaDef != c2.nodo->estaDef) {
            return false;
        }else if (c1.nodo->estaDef){
            if(c1.nodo->significado->valor != c2.nodo->significado->valor) return false;
        }
        ++c1;
        ++c2;
    }
    return true;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const key_type &key){
    Nodo *nodo = buscarNodo(key);
    tamano--;
    return nodo->estaDef = false;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::end(){
    return nullptr;
}