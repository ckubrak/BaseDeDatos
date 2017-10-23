#ifndef join_iterator_h
#define join_iterator_h

#include "BaseDeDatos.h"
#include <list>

class BaseDeDatos::join_iterator {
public:
    typedef Tabla::const_iterador_registros const_iterador_registros;
    typedef list<Tabla::const_iterador_registros> listaReg;

    //Amigos de la clase
    friend class BaseDeDatos;

    bool operator==(const join_iterator &otro) const {
        return (_elem1 == otro._elem1 && _elem2 == otro._elem2 && _pos == otro._pos);
    }

    bool operator!=(const join_iterator &otro) const {
        return not(*this == otro);
    }


    join_iterator operator++() {
        if (true) {
            listaReg::const_iterator *nuevoPos = &(*(_pos++));
            const const_iterador_registros *nuevoElem2 = &(*(*(nuevoPos)));

            return join_iterator(_elem1, nuevoElem2, nuevoPos);
        } else {
            for (auto r = *(_elem1++); r != t1.registros_end(); ++r) {
                Dato d = r->dato(campo);
                if (d.esNat()) {
                    if (i._diccInt.count(d.valorNat())) {
                        pos = i._diccInt[d.valorNat()].begin();
                        elem2 = *(pos);
                        const_iterador_registros *join1 = &r;
                        const_iterador_registros *join2 = &elem2;
                        listaReg::iterator *join3 = &pos;
                        return join_iterator(join1, join2, join3);
                    }
                } else {
                    if (i._diccString.count(d.valorStr())) {
                        pos = i._diccString[d.valorStr()].begin();
                        elem2 = *(pos);
                        const_iterador_registros *join1 = &r;
                        const_iterador_registros *join2 = &elem2;
                        listaReg::iterator *join3 = &pos;
                        return join_iterator(join1, join2, join3);
                    }
                }
            }
            return join_end() //si salgo del for y no retorne el siguiente elemento, entonces estoy en end.
        }
    }

private:
    join_iterator(const const_iterador_registros *it1, const const_iterador_registros *it2,
                  const listaReg::const_iterator *pos) : _elem1(it1), _elem2(it2), _pos(pos) {}

    const_iterador_registros *_elem1;
    const_iterador_registros *_elem2;
    listaReg::const_iterator *_pos;
};

#endif
