#ifndef join_iterator_h
#define join_iterator_h

#include "BaseDeDatos.h"
#include <list>
class BaseDeDatos::join_iterator{
 public:
  typedef Tabla::const_iterador_registros const_iterador_registros;
  typedef list<Tabla::const_iterador_registros> listaReg;
  //Amigos de la clase
  friend class BaseDeDatos;
 
  //Constructor vacío
  //Constructor por copia
  //Operador =
  //Operador ==
  //Operador !=
  //Operador ++ (prefijo)
  //Operador ++int (postfijo)
  //Operador *
  //Operador ->
 
 private:
 join_iterator(const_iterador_registros it1, const_iterador_registros it2, listaReg::iterator pos): 
  _elem1(it1),_elem2(it2),_pos(pos) {}
  const_iterador_registros _elem1;
  const_iterador_registros _elem2;
  listaReg::iterator _pos;
};
/* #include "BaseDeDatos.h" */
/* #include "const_iterador_registros.h" */
/* #include <iostream> */

/* using namespace std; */
/* typedef Tabla::const_iterador_registros const_iterador_registros;  */

/* class BaseDeDatos::join_iterator { */
/*  public: */
/*   friend class BaseDeDatos; */

/*   join_iterator join(string& t1, string& t2, string campo){ */
/*     Indice &i = _indices[t2][campo]; */
/*     elem1& = t1.begin(); */
/*     for (auto r : t1.registros()){ */
/*       Dato &d = r.dato(campo); */
/*       if (d.esNat()){ */
/*         if(i.count(d.esNat())){ */
/*           elem2 = i._diccInt[d.valorNat()][0]; */
/*           break */
/*         } */
/*       } */
/*     } */
/*     posicion = 0; //creo un iterador que indica en que posicion de la lista estoy. */
/*     return join_iterator(elem1,elem2,posicion) */
/* } */
/*   Registro &operator*(); */

/*   join_iterator &operator++(); */

/*  private: */
/*   friend class BaseDeDatos; */

/*   const_iterador_registros elem1; */
/*   const_iterador_registros elem2; */
/*   int posicion; */

/* }; */



#endif
