#include "BaseDeDatos.h"
#include <list>
#include <tuple>
#include <algorithm>

BaseDeDatos::BaseDeDatos() {};

void BaseDeDatos::crearTabla(const string &nombre,
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Dato> &tipos) {
    string_map<Tabla>::value_type nuevaTabla = make_pair(nombre, Tabla(claves, campos, tipos));
    _tablas.insert(nuevaTabla);
    _nombres_tablas.fast_insert(nombre);
};

void BaseDeDatos::agregarRegistro(const Registro &r, const string &nombre) {
    Tabla &t = _tablas.at(nombre);
    const_iterador_registros ultimo = t.registros_end();
    t.agregarRegistro(r);
    if (_indices.count(nombre)) {
        for (auto c : t.campos()) {
            bool esNat = t.tipoCampo(c).esNat();
            if (_indices.at(nombre).count(c)) {
                if (esNat) {
                    _indices[nombre][c]._diccInt[r.dato(c).valorNat()].push_back(ultimo);
                } else {
                    _indices[nombre][c]._diccString[r.dato(c).valorStr()].push_back(ultimo);
                }
            }
        }
    }
}

const linear_set<string> &BaseDeDatos::tablas() const {
    return _nombres_tablas;
}

const Tabla &BaseDeDatos::dameTabla(const string &nombre) const {
    return _tablas.at(nombre);
}

int BaseDeDatos::uso_criterio(const BaseDeDatos::Criterio &criterio) const {
    if (_uso_criterios.count(criterio)) {
        return _uso_criterios.at(criterio);
    } else {
        return 0;
    }
}

bool BaseDeDatos::registroValido(const Registro &r,
                                 const string &nombre) const {
    const Tabla &t = _tablas.at(nombre); // O(1)
    // t.campos() y r.campos son O(1)
    // _mismos_tipos es O(C)
    // _no_repite es O(c*n)
    return (t.campos() == r.campos() and _mismos_tipos(r, t) and
            _no_repite(r, t));
}

bool BaseDeDatos::_mismos_tipos(const Registro &r, const Tabla &t) const {
    for (auto c : t.campos()) { // O(C)
        // el if es O(1)
        if (r.dato(c).esNat() != t.tipoCampo(c).esNat()) {
            return false;
        }
    }
    return true;
}

bool BaseDeDatos::_no_repite(const Registro &r, const Tabla &t) const {

    for (auto reg : t.registros()) {
        int cant = 0;
        for (auto cc : t.claves()) {
            if (reg.dato(cc) != r.dato(cc))
                break;
            cant++;
            if (reg.dato(cc) == r.dato(cc) && cant == t.claves().size())
                return false;
        }
    }
    return true;
}

list<Registro> &
BaseDeDatos::_filtrar_registros(const string &campo, const Dato &valor,
                                list<Registro> &registros) const {
    return _filtrar_registros(campo, valor, registros, true);
}

list<Registro> &BaseDeDatos::_filtrar_registros(const string &campo,
                                                const Dato &valor,
                                                list<Registro> &registros,
                                                bool igualdad) const {
    auto iter = registros.begin();
    while (iter != registros.end()) { // O(n)
        auto now = iter;
        iter++;
        if ((not igualdad) xor now->dato(campo) != valor) {
            registros.erase(now);
        }
    }

    return registros;
}

pair<vector<string>, vector<Dato> > BaseDeDatos::_tipos_tabla(const Tabla &t) {
    vector<string> res_campos;
    vector<Dato> res_tipos;
    for (auto c : t.campos()) {
        res_campos.push_back(c);
        res_tipos.push_back(t.tipoCampo(c));
    }
    return make_pair(res_campos, res_tipos);
}

bool BaseDeDatos::criterioValido(const Criterio &c,
                                 const string &nombre) const {
    const Tabla &t = _tablas.at(nombre);
    for (auto restriccion : c) {
        if (not t.campos().count(restriccion.campo())) {
            return false;
        }
        if (t.tipoCampo(restriccion.campo()).esNat() !=
            restriccion.dato().esNat()) {
            return false;
        }
    }
    return true;
}

Tabla BaseDeDatos::busqueda(const BaseDeDatos::Criterio &c,
                            const string &nombre) {
    if (_uso_criterios.count(c)) {
        _uso_criterios.at(c)++;
    } else {
        _uso_criterios.fast_insert(make_pair(c, 1));
    }

    const Tabla &ref = dameTabla(nombre);
    auto campos_datos = _tipos_tabla(ref);
    Tabla res(ref.claves(), campos_datos.first, campos_datos.second);
    list<Registro> regs(ref.registros().begin(), ref.registros().end());
    for (auto restriccion : c) {
        _filtrar_registros(restriccion.campo(), restriccion.dato(),
                           regs, restriccion.igual());
    }
    for (auto r : regs) {
        res.agregarRegistro(r);
    }
    return res;
}

linear_set<BaseDeDatos::Criterio> BaseDeDatos::top_criterios() const {
    linear_set<Criterio> ret;
    int max = 0;
    for (auto crit_count : _uso_criterios) {
        if (crit_count.second >= max) {
            if (crit_count.second > max) {
                ret = linear_set<Criterio>();
                max = crit_count.second;
            }
            ret.fast_insert(crit_count.first);
        }
    }
    return ret;
}

void BaseDeDatos::crearIndice(const string &nombre, const string &campo) {
    Tabla &t = _tablas.at(nombre); // O(1)
    bool esNat = t.tipoCampo(campo).esNat(); // O(1)
    Indice indiceNuevo;
    for (auto r = t.registros_begin(); r != t.registros_end(); ++r) {
        list<const_iterador_registros> registro = {r};
        Dato d = r->dato(campo);
        bool esNat = d.esNat();
        if (esNat) {
            if (indiceNuevo._diccInt.count(d.valorNat())) {
                indiceNuevo._diccInt.at(d.valorNat()).push_back(r);
            } else {
                map<int, list<const_iterador_registros> >::value_type nReg = make_pair(d.valorNat(), registro);
                indiceNuevo._diccInt.insert(nReg);
            }
        } else {
            if (indiceNuevo._diccString.count(d.valorStr())) {
                indiceNuevo._diccString.at(d.valorStr()).push_back(r);
            } else {
                string_map<list<const_iterador_registros> >::value_type nReg = make_pair(d.valorStr(), registro);
                indiceNuevo._diccString.insert(nReg);
            }
        }
    }
    indiceNuevo._esNat = esNat;
    indiceNuevo._campo = campo;
    string_map<Indice>::value_type indices = make_pair(campo, indiceNuevo);
    if (_indices.count(nombre)) {
        _indices.at(nombre).insert(indices);
    } else {
        string_map<Indice> nuevoIndice;
        nuevoIndice.insert(indices);
        string_map<string_map<Indice> >::value_type parNuevo = make_pair(nombre, nuevoIndice);
        _indices.insert(parNuevo);
    }
}


BaseDeDatos::join_iterator BaseDeDatos::join(const string &tabla1, const string &tabla2, const string &campo) {
    typedef list<const_iterador_registros> listaReg;
    Indice i = _indices[tabla2][campo];
    Tabla t1 = dameTabla(tabla1);
    Tabla t2 = dameTabla(tabla1);
    const_iterador_registros elem2(t1.registros_begin()); // Le ponemos basura para despues reescribirlo
    listaReg::iterator pos;
    for (auto r = t1.registros_begin(); r != t1.registros_end(); ++r) {
        Dato d = r->dato(campo);
        if (d.esNat()) {
            if (i._diccInt.count(d.valorNat())) {
                pos = i._diccInt[d.valorNat()].begin();
                elem2 = *(pos);
                const_iterador_registros *join1 = &r;
                const_iterador_registros *join2 = &elem2;
                Tabla* tRes = &t1;
                listaReg* lRes = &i._diccInt[d.valorNat()];
                const string* cRes = &campo;
                Indice* iRes = &i;
                return join_iterator(tRes,join1, join2, lRes,cRes,iRes);
            }
        } else {
            if (i._diccString.count(d.valorStr())) {
                pos = i._diccString[d.valorStr()].begin();
                elem2 = *(pos);
                const_iterador_registros *join1 = &r;
                const_iterador_registros *join2 = &elem2;
                Tabla* tRes = &t1;
                listaReg* lRes = &i._diccString[d.valorStr()];
                const string* cRes = &campo;
                Indice* iRes = &i;
                return join_iterator(tRes,join1, join2, lRes,cRes,iRes);
            }
        }
    }
    return join_end();
}

BaseDeDatos::join_iterator BaseDeDatos::join_end() const {
    return join_iterator();
}

// BaseDeDatos::join_iterator BaseDeDatos::join_iterator::operator++(){
//   // if(si el siguiente a _pos != linear_set.end()){
// }
