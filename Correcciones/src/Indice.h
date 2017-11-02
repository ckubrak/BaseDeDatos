#ifndef INDICES_H
#define INDICES_H
#include "Tabla.h"
#include "string_map.h"
#include "linear_set.h"
#include "const_iterador_registros.h"
#include <map>
#include <set>
#include <list>


// @corregir documentar esta clase con todas sus operaciones

/** @comentario La idea de esta clase está muy buena, dado que agrupa en una clase las dos opciones posibles de indices
 *  El problema que tiene es que no encapsula la existencia del string_map y map subyacentes.
 *  Esto se refleja en que la BD tiene que preguntar si el indice es Nat o String y hacer cosas diferentes.
 *
 *  Quedaría mucho mejor si:
 *  - Tuviera un único generador que toma un dato e inicializa internamente un indice de Nat / String según corresponda
 *  - Tuviera sus diccionarios internos privados
 *  - Implementara las operaciones que necesiten de map
 *    - usando Dato como clave
 *    - usando internamente el diccioanrio que corresponda
 * En particular este último punto permitiría a la BD / join iterator usar los Indices indistintamente sin tener que preguntar su tipo interno
 */

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
  /* friend class BaseDeDatos::join_iterator; */
  string _campo;
  bool _esNat;
  diccInt _diccInt;
  diccStr _diccString;
};

#endif
