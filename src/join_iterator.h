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
    //elem1 = elem
    bool operator==(const join_iterator& otro) const{
        return esEnd == otro.esEnd && _tab == otro._tab
               && _lista==otro._lista && _elem1==otro._elem1
               && _elem2 == otro._elem2 && pos == otro.pos && _campo == otro._campo
               && _indice == otro._indice;
    }
    bool operator!=(const join_iterator& otro) const{
        return not(*this == otro);
    }

    join_iterator& operator++() {
        assert(!esEnd);
        if(pos != _lista->end()){
            ++pos;
            _elem2 = &(*pos);
        }else{
            while(++(*_elem1) != _tab->registros_end()){
                Dato d = (*(*_elem1)).dato(*_campo);
                Indice i = *_indice;
                if(d.esNat()){
                    if (i._diccInt.count(d.valorNat())){
                        pos = i._diccInt[d.valorNat()].begin();
                        _elem2 = &(*pos);
                        _lista = &i._diccInt[d.valorNat()];
                        return *this;
                    }
                }else{
                    if (i._diccString.count(d.valorStr())){
                        pos = i._diccString[d.valorStr()].begin();
                        _elem2 = &(*pos);
                        _lista = &i._diccString[d.valorStr()];
                        return *this;
                    }
                }
            }
            //Sino retorno end
            mandarAEnd();
            return *this;
        }
    }

    join_iterator& operator++(int){
        return (++*this);
    }

    Registro operator*(){
        //TODO
    }

private:
    bool esEnd = false;
    //Constructor para end
    join_iterator() : esEnd(true){};

    join_iterator(const Tabla *t, const_iterador_registros* it1 /*sin indice*/, const_iterador_registros* it2/*con indice*/, listaReg *list, const string* camp, Indice* index):
            _tab(t),_elem1(it1),_elem2(it2),_lista(list), _campo(camp), _indice(index){
        pos = _lista->begin();
    }
    const Tabla* _tab = nullptr;
    listaReg* _lista = nullptr;
    const_iterador_registros* _elem1 = nullptr;  //de la tabla que no tiene indice
    const_iterador_registros* _elem2 = nullptr;; //del indice
    listaReg::iterator pos;
    const string* _campo = nullptr; //Campo
    Indice* _indice;

    void mandarAEnd(){
        _elem1 = nullptr;
        _elem2 = nullptr;
        esEnd = true;
    }
};

#endif