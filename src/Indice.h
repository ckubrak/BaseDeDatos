#ifndef INDICES_H
#define INDICES_H
#include "Tabla.h"
#include "string_map.h"
#include "linear_set.h"
#include "const_iterador_registros.h"
#include <map>
#include <set>
#include <list>


using namespace std;
class Indice{
 public:
  typedef string_map< list<Tabla::const_iterador_registros> > diccStr;
  typedef map<int,list<Tabla::const_iterador_registros> > diccInt;
  bool esNat() const{
    return _esNat;
  }

  string campo() const{
    return _campo;
  }

  diccInt diccionarioInt() const{
    return _diccInt;
  }

  diccStr diccionarioString() const{
    return _diccString;
  }
  bool operator==(const Indice& otro) const{
    bool mismoTipo = this->esNat() == otro.esNat();
    bool mismoCampo = this->campo() == otro.campo();
    bool mismoDiccInt = this->diccionarioInt() == otro.diccionarioInt();
    bool mismoDiccStr = this->diccionarioString() == otro.diccionarioString();
    return (mismoTipo && mismoCampo && mismoDiccInt && mismoDiccStr);
  }
  bool operator!=(const Indice& otro) const{
    return not (*this == otro);
  }

 private:
  friend class BaseDeDatos;
  string _campo;
  bool _esNat;
  diccInt _diccInt;
  diccStr _diccString;
};

#endif
