#include "BaseDeDatos.h"
#include <list>
#include <tuple>
#include <algorithm>

BaseDeDatos::BaseDeDatos(){};

void BaseDeDatos::crearTabla(const string &nombre, 
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Dato> &tipos) {
  string_map<Tabla>::value_type nuevaTabla= make_pair(nombre,Tabla(claves, campos, tipos));
  _tablas.insert(nuevaTabla);
}

void BaseDeDatos::agregarRegistro(const Registro &r, const string &nombre) {
  Tabla &t = _tablas.at(nombre);
 if (_indices.count(nombre)){
   for (auto i : _indices.at(nombre)){
     if (i.esNat()){
       int nuevo = r.dato(i.campo()).valorNat();
       // i._diccInt.(make_pair(nuevo, r));
     }
   }
   //i._diccInt.insert()
   //}
     //}
   //Modificar indices
   // bool esNat = _indices[nombre].tipo();
   // set<Registro> registros;
   // if (esNat){
   //   registros = _dicc_int[]
   // }
   // for (auto ri : registros){
   // }
   t.agregarRegistro(r);
  } else {
    t.agregarRegistro(r);
  }
}

const linear_set<string> &BaseDeDatos::tablas() const {
  linear_set<Tabla> result;
  for (auto t : _tablas){
    result.fast_insert(t.second);
  }
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
  // campos() es O(1)
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
  list<Registro> filtrados(t.registros().begin(), t.registros().end());
  for (auto clave : t.claves()) {
    _filtrar_registros(clave, r.dato(clave), filtrados);
  }
  return filtrados.empty();
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
  while ( iter != registros.end()) {
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

void BaseDeDatos::crearIndice(const string &nombre, const string &campo){
  Tabla &t = _tablas.at(nombre); // O(1)
  bool esNat = t.tipoCampo(campo).esNat(); // O(1)
  linear_set<Indice> indices = {};
  for (auto r : t.registros()){ //O(m)
    Indice indiceNuevo;
    linear_set<Registro> registro = {r};
    Dato d = r.dato(campo);
    bool esNat = d.esNat();
    if (esNat){
      if (indiceNuevo._diccInt.count(d.valorNat())){
        indiceNuevo._diccInt[d.valorNat()].fast_insert(r);
      } else {
        map<int,linear_set<Registro> >::value_type nReg = make_pair(d.valorNat(),registro);
        indiceNuevo._diccInt.insert(nReg);
      }
    } else {
      if (indiceNuevo._diccString.count(d.valorStr())){
        indiceNuevo._diccString.at(d.valorStr()).fast_insert(r);
      } else {
        string_map<linear_set <Registro> >::value_type nReg = make_pair(d.valorStr(),registro);
        indiceNuevo._diccString.insert(nReg);
      }
    }
    indiceNuevo._esNat = esNat;
    indices.insert(indiceNuevo);
  }
  string_map<linear_set<Indice> >::value_type parNuevo= make_pair(nombre,indices);
  _indices.insert(parNuevo); // O(1)
}
