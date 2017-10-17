#include "string_map_final.h"

// ========= Struct Nodo de String_Map =========
template<typename T>
struct string_map<T>::Nodo {
    //Variables internas
    key_type clave;
    mapped_type significado = NULL;
    Nodo *padre;
    Nodo *hijos[96];  // Son 128 - 32 caracteres
    value_type *cs = nullptr;

    //Constructor del Nodo
    Nodo(const key_type &key, bool def, Nodo *pad) : clave(key), estaDef(def), padre(pad) {
        for (int i = 0; i < 96; i++) {
            hijos[i] = nullptr;
        }
    };

    //Dice si el nodo está definido o no
    bool estaDef(){
        return significado == NULL;
    }

    //Operación de asignado de significado al nodo
    bool definir(mapped_type _sig) {
        bool acabaDeDefinirlo = estaDef();
        significado = _sig;
        if (cs != nullptr) delete cs;
        cs = new value_type(clave, significado);
        return acabaDeDefinirlo;
    }

    //Operación de retorno del par <clave, significado>. Requiere que esté definido el nodo.
    value_type &claveSignificado() {
        assert(estaDef());
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
        //TODO
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
    medirRama(raiz);
}

template<typename T>
int string_map<T>::medirRama(Nodo *parent) const {
    int resultado = 0;
    std::cout << "Parent: " << parent->clave << " def?" << parent->estaDef() << std::endl;
    if (parent->estaDef()) resultado++;
    for (int a = 0; a < 96; a++) {
        if (parent->hijos[a] != nullptr) {
            std::cout << parent->hijos[a]->clave << std::endl;
            resultado += medirRama(parent->hijos[a]);
        }
    }
    return resultado;
}

template<typename T>
bool string_map<T>::empty() const {
    return medirRama(raiz) == 0;
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