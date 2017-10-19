#ifndef INDICES_H
#define INDICES_H
#include "Tabla.h"
#include "string_map.h"
#include <map>
#include <set>

using namespace std;
class Indice{
 public:
  /* Indice(const string &campo, const bool tipo){ */
  /*   return; */
  /* } */
  bool esNat(){
    return _esNat;
  }

  string campo(){
    return _campo;
  }

  map<int,set<Registro> > diccInt(){
    return _dicc_int;
  }

  string_map< set<Registro> > diccString() {
    return _dicc_string; 
  }

 private:
  friend class BaseDeDatos;
  string _campo;
  bool _esNat;
  map<int,set<Registro> >  _dicc_int;
  string_map< set<Registro> > _dicc_string;
};

#endif
