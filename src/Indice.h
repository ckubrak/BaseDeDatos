#ifndef INDICES_H
#define INDICES_H
#include "Tabla.h"
#include "string_map.h"
#include "linear_set.h"
#include <map>
#include <set>

using namespace std;
class Indice{
 public:
  /* Indice(const string &campo, const bool tipo){ */
  /*   return; */
  /* } */
  bool esNat() const{
    return _esNat;
  }

  string campo() const{
    return _campo;
  }

  map<int,linear_set<Registro> > diccInt() const{
    return _diccInt;
  }

  string_map< linear_set<Registro> > diccString() const{
    return _diccString; 
  }
  bool operator==(const Indice& otro) const{
    bool mismoTipo = this->esNat() == otro.esNat();
    bool mismoCampo = this->campo() == otro.campo();
    bool mismoDiccInt = this->diccInt() == otro.diccInt();
    bool mismoDiccStr = this->diccString() == otro.diccString();
    return (mismoTipo && mismoCampo && mismoDiccInt && mismoDiccStr);
  }
  bool operator!=(const Indice& otro) const{
    return not (*this == otro);
  }

 private:
  friend class BaseDeDatos;
  string _campo;
  bool _esNat;
  map<int,linear_set<Registro> >  _diccInt;
  string_map< linear_set<Registro> > _diccString;
};

#endif
