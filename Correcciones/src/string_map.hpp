#include "string_map.h"

// @corregir Tener este contenedorSignificado y una tupla aparte que puede quedar desincronizada es muy peligroso e innecesario.
// @corregir Tampoco los salva realmente de los T que no tengan constructor por defecto. CHARLARLO conmigo.
// @comentario Está perfectamente OK que un string_map<Dato> (u otro T sin constructor default) no compile si se usa el operator[]. En esos casos basta con usar solamente at(...) e insert(..) para obtener y definir tuplas en el map. Si ustedes proveen un string_map que asegura esto, va a estar bien.

// ========= Clase contenedorSignificado de String_Map =========
// Esta clase almacena los valores de los nodos, cosa de no tener que usar el constructor default de los tipos.
template<typename T>
class string_map<T>::contenedorSignificado {
public:
    T valor;

    T &operator=(const T &a) {
        valor = a;
    }

    contenedorSignificado() {} //Existe por si se usa el operador [] con una clave que no existe

    contenedorSignificado(T &a) : valor(a) {}
};

// ========= Clase claveSignificado de String_Map =========
// Esta clase almacena los pair clave - significado para los operatores * y -> de iterador
template<typename T>
class string_map<T>::claveSignificado {
public:
    Nodo *miNodo;
    value_type *cs = nullptr;

    claveSignificado(Nodo *n) : miNodo(n) {}

    ~claveSignificado() {
        if (cs != nullptr) delete cs;
    }

    void refrescarClaveSignificado() {
        if (cs == nullptr) {
            cs = new value_type(miNodo->clave, miNodo->significado->valor);
        } else {
            cs->second = miNodo->significado->valor;
        }
    }
};

// ========= Struct Nodo de String_Map =========
template<typename T>
struct string_map<T>::Nodo {
    //Variables internas
    key_type clave;
    //mapped_type significado;
    contenedorSignificado *significado = nullptr;
    Nodo *padre;
    Nodo *hijos[96];  // Son 128 - 32 caracteres
    claveSignificado *cs = nullptr;

    //Constructor del Nodo indefinido
    Nodo(const key_type &key, Nodo *pad) : clave(key), padre(pad) {
        for (int i = 0; i < 96; i++) {
            hijos[i] = nullptr;
        }
        cs = new claveSignificado(this);
    };

    //Destructor de nodo
    ~Nodo() {
        if (significado != nullptr) delete significado;
        if (cs != nullptr) delete cs;
    }

    //Operación de asignado de significado al nodo
    bool definir(mapped_type _sig) {
        bool acabaDeDefinirlo = !estaDef();
        if (significado == nullptr) {
            significado = new contenedorSignificado(_sig);
        } else {
            significado->valor = _sig;
        }
        return acabaDeDefinirlo;
    }

    bool estaDef() const {
        return !(significado == nullptr);
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
        int i = 0;
        while (i < 96) {
            if (nodo->hijos[i] != nullptr) {
                Nodo *actual = nodo->hijos[i];
                int k;
                while (!actual->estaDef()) {
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
            i++;
        }

        // @corregir el código de abajo tiene *4* whiles anidados. Comenten más en detalle el funcionamiento.

        Nodo *actual = nodo->padre;
        int k;
        int j = ((int) (nodo->clave[nodo->clave.length() - 1])) - 31;
        //Quiero ver los hijos del padre del nodo, desde la ultima letra
        // I.e. Si la ultima letra de la clave es B, empiezo a ver los hijos del padre desde la C.
        while (actual != nullptr) {
            while (j < 96) {
                if (actual->hijos[j] != nullptr) {
                    actual = actual->hijos[j];
                    while (!(actual->estaDef())) {
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
    iterator operator++(int) {
        iterator resultado(*this);
        ++(*this);
        return resultado;
    }

    //Operador *
    value_type &operator*() const {
        nodo->cs->refrescarClaveSignificado();
        return *nodo->cs->cs;
    }

    //Operador ->
    value_type *operator->() const {
        nodo->cs->refrescarClaveSignificado();
        return nodo->cs->cs;
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
                while (!actual->estaDef()) {
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
                    while (!(actual->estaDef())) {
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
    const_iterator operator++(int n) {
        for (int a = 0; a < n; a++) {
            ++this;
        }
    }

    //Operador *
    value_type &operator*() const {
        nodo->cs->refrescarClaveSignificado();
        return *nodo->cs->cs;
    }

    //Operador ->
    value_type *operator->() const {
        nodo->cs->refrescarClaveSignificado();
        return *nodo->cs->cs;
    }

private:
    //Variables de la clase
    const Nodo *nodo;

    //Constructor por parámetro
    const_iterator(Nodo *n) : nodo(n) {}
};

// ========= Métodos de String_Map =========

template<typename T>
string_map<T>::~string_map() {
    borrarTodo(raiz);
}

template<typename T>
string_map<T>::string_map(const string_map &aCopiar) {
    copiar(aCopiar);
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
    return (actual->estaDef() ? 1 : 0);
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
typename string_map<T>::Nodo *string_map<T>::buscarNodo(const key_type &key) const {
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
        if (actual->hijos[posHijo] == nullptr) actual->hijos[posHijo] = new Nodo(claveNueva, actual);
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
    while (!actual->estaDef()) {
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
    return const_iterator(nullptr);
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) {
    Nodo *nodo = buscarNodo(key);
    return nodo->significado->valor;
}

template<typename T>
bool string_map<T>::revisarIgualdad(const string_map<T> &otro) const {
    const_iterator c1 = begin();
    const_iterator c2 = otro.begin();
    if (size() != otro.size()) return false;
    while (c1.nodo != nullptr && c2.nodo != nullptr) {
        if (c1.nodo->clave != c2.nodo->clave) {
            return false;
        } else if (c1.nodo->estaDef() != c2.nodo->estaDef()) {
            return false;
        } else if (c1.nodo->estaDef()) {
            if (!(c1.nodo->significado->valor == c2.nodo->significado->valor)) return false;
        }
        ++c1;
        ++c2;
    }
    return true;
}

template<typename T>
int string_map<T>::cantHijos(Nodo *nodo){
    int cantidad = 0;
    for (int a = 0; a < 96; a++) {
        if (nodo->hijos[a] != nullptr) {
            cantidad++;
        }
    }
    return cantidad;
}

template<typename T>
void string_map<T>::borrarRecursivo(Nodo *aBorrar) {
    size_type i = 1;
    Nodo *hastaDondeBorro = aBorrar->padre;
    while (cantHijos(hastaDondeBorro)< 2 && !(hastaDondeBorro->estaDef()) && !(hastaDondeBorro == raiz)) { // < 2 o == 1
        Nodo* temp = hastaDondeBorro->padre;
        delete hastaDondeBorro;
        hastaDondeBorro = temp;
        i++;
    }
    hastaDondeBorro->hijos[charToInt(aBorrar->clave[aBorrar->clave.length()- i])] = nullptr;
    delete aBorrar;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const key_type &key) {
    Nodo *nodo = buscarNodo(key);
    int borrados = 0;
    if (nodo->estaDef()) {
        //Si tiene hijos, borro solo su significado
        if (cantHijos(nodo)>0) {
            delete nodo->significado;
            nodo->significado = nullptr;
        } else {
            borrarRecursivo(nodo);
        }

        tamano--;
        borrados++;
    }
    return borrados;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::end() {
    return iterator(nullptr);
}

template<typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) const {
    Nodo *nodo = buscarNodo(key);
    assert(nodo->estaDef());
    return nodo->significado->valor;
}

template<typename T>
void string_map<T>::borrarTodo(Nodo *desde) {
    for (int a = 0; a < 96; a++) {
        if (desde->hijos[a] != nullptr) borrarTodo(desde->hijos[a]);
    }
    if(desde->estaDef()) tamano--;
    delete desde;
}

template<typename T>
void string_map<T>::clear() {
    borrarTodo(raiz);
    raiz = new Nodo("", nullptr);
}

template<typename T>
void string_map<T>::copiar(const string_map<T> &aCopiar) {
    clear();
    const_iterator it = aCopiar.begin();
    while (it != aCopiar.end()) {
        key_type clave = it.nodo->clave;
        mapped_type significado = it.nodo->significado->valor;
        insert(make_pair(clave, significado));
        ++it;
    }
}

template<typename T>
typename string_map<T>::iterator string_map<T>::find(const key_type &key) {
    return iterator(buscarNodo(key));
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::find(const key_type &key) const {
    return const_iterator(buscarNodo(key));
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin() {
    int i = 0;
    while (i < 96) {
        if (raiz->hijos[i] != nullptr) {
            return iterator(minimaClave(raiz->hijos[i]));
        }
        i++;
    }
    return end();
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(iterator pos) {
    Nodo *nodo = pos.nodo;
    pos++;
    erase(nodo->clave);
    return pos;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::cbegin() const {
    const_iterator(begin());
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::cend() const {
    const_iterator(end());
}
