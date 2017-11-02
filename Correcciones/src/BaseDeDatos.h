#ifndef _BASEDEDATOS_H
#define _BASEDEDATOS_H

#include "Indice.h"
#include "Registro.h"
#include "Restriccion.h"
#include "Tabla.h"
#include <utility>
#include <list>
#include <string>
#include <map>
#include "const_iterador_registros.h"
#include "linear_map.h"
#include "linear_set.h"
#include "utils.h"

///////////// RESUMEN DE CORRECCIONES DEL TP2 //////////////////////////////

// @corregir agregar documentación doxygen de todas las funciones agregadas en todo el TP
// @corregir completar implementaciones faltantes
// @corregir bugs de join/indices marcados -> probables causantes de SEGFAULT en tests
// @corregir rotura de encapsulamiento entre Tabla y BaseDeDatos
// @corregir TP desactualizdo con respecto a última versión del TP2 base de la cátedra. Revisar.

using namespace std;

/**
 * @brief Una base de datos es un administrador de tablas con funciones de
 * búsqueda.
 *
 * Una base de datos permite administrar tablas identificadas por registro.
 * Permite saber si se puede agegar un registro a una tabla y luego agregarlo.
 * Permite realizar filtros del contenido de tablas mediante criterios de
 * búsqueda. Además mantiene estadísticas del uso de los criterios.
 *
 * **se explica con** TAD BaseDeDatos
 */
class BaseDeDatos {

public:
  /** @brief Criterio de búsqueda para una base de datos */
  typedef linear_set<Restriccion> Criterio;
  typedef Tabla::const_iterador_registros const_iterador_registros; 

  // Forward declarations
  class join_iterator;

  /**
   * @brief Inicializa una base de datos sin tablas.
   *
   * \pre true
   * \post \P{this} = nuevaDB
   *
   * \complexity{\O(1)}
   */
  BaseDeDatos();

  /**
   * @brief Crea una nueva tabla en la base de datos.
   *
   * @param nombre Nombre identificador de la tabla
   * @param claves Claves de la tabla a crear
   * @param campos Campos de la tabla a crear
   * @param tipos  Tipos para los campos de la tabla a crear
   *
   * \pre db = \P{this} \LAND
   *      \LNOT (nombre \IN tablas(\P{this})) \LAND 
   *      \LAND \LNOT \EMPTYSET?(claves) \LAND
   *      \FORALL (c: campo) c \IN claves \IMPLICA c \IN campos \LAND 
   *      long(campos) = long(tipos) \LAND sinRepetidos(campos)
   * \post \P{this} = agregarTabla(nuevaTabla(claves, nuevoRegistro(campos, tipos)), db)
   *
   * \complexity{\O(C)}
   */
  void crearTabla(const string &nombre, const linear_set<string> &claves,
                  const vector<string> &campos, const vector<Dato> &tipos);

  /**
   * @brief Agrega un registro a la tabla parámetro
   *
   * @param r Registro a agregar
   * @param nombre Nombre de la tabla donde se agrega el registro
   *
   * \pre db = \P{this} \LAND nombre \IN tablas(\P{this}) \LAND 
   *      puedoInsertar?(r, dameTabla(\P{this}))
   * \post \P{this} = insertarEntrada(r, nombre, db)
   *
   * \complexity{\O(T + copy(reg))}
   */
  void agregarRegistro(const Registro &r, const string &nombre);

  /**
   * @brief Devuelve el conjunto de tablas existentes en la base.
   *
   * El conjunto de nombres se devuelve por referencia no-modificable.
   *
   * \pre true
   * \post \P{res} = tablas(\P{this})
   *
   * \complexity{\O(1)}
   */
  const linear_set<string> &tablas() const;

  /**
   * @brief Devuelve la tabla asociada al nombre.
   *
   * La tabla se devuelve por referencia no modificable.
   *
   * @param nombre Nombre de la tabla buscada.
   *
   * \pre nombre \IN tablas(\P{this})
   * \post \P{res} = dameTabla(nombre, \P{this})
   *
   * \complexity{O(1)}
   */
  const Tabla &dameTabla(const string &nombre) const;

  /**
   * @brief Devuelve la cantidad de usos que tiene un criterio
   *
   * @param criterio Criterio por el cual se consulta.
   *
   * \pre nombre \IN tablas(\P{this})
   * \post \P{res} = usoCriterio(criterio, \P{this})
   *
   * \complexity{\O(#cs * cmp(Criterio))}
   */
  int uso_criterio(const Criterio &criterio) const;

  /**
   * @brief Evalúa si un registro puede ingresarse en la tabla parámetro.
   *
   * @param r Registro a ingresar en la tabla.
   * @param nombre Nombre de la tabla.
   *
   * \pre nombre \IN tablas(\P{this})
   * \post \P{res} = puedoInsertar?(r, dameTabla(nombre, \P{this}))
   *
   * \complexity{\O(C + (c.n.l))}
   */
  bool registroValido(const Registro &r, const string &nombre) const;
  /**
   * @brief Crea un indice en la base de datos
   * @param nombre de la tabla
   * @param campo es el nombre del campo del indice
   *
   * \pre this = db \LAND nombre pertenece a las tablas \LAND campo pertenece a los campos de nombre
   * \post this = crearIndice(nombre, campo, db)
   *
   * \complexity{\O(m * [L + log(m)])}
   */


  bool criterioValido(const Criterio &c, const string &nombre) const;

  /**
   * @brief Devuelve el resultado de buscar en una tabla con un criterio.
   *
   * @param c Criterio de búsqueda utilizado.
   * @param nombre Nombre de la tabla.
   *
   * \pre nombre \IN tablas(\P{this}) \LAND criterioValido(c, nombre, \P{this}) 
   * \post \P{res} = buscar(c, nombre, \P{this})
   *
   * \complexity{\O(T + cs * cmp(Criterio) + cr * n * (C + L + copy(reg)))}
   */
  Tabla busqueda(const Criterio &c, const string &nombre);

  /**
   * @brief Devuelve los criterios de máximo uso.
   *
   * \pre true
   * \post \FORALL (c : Criterio) [c \IN \P{res} \IFF 
   *       \FORALL (c' : Criterio) usoCriterio(c, db) >= usoCriterio(c', db)]
   *
   * \complexity{\O(cs * copy(Criterio))}
   */
  linear_set<Criterio> top_criterios() const;

  void crearIndice(const string &nombre, const string &campo);

  join_iterator join(const string& tabla1, const string& tabla2,
                     const string& campo);
  join_iterator join_end() const;

  /* join_iterator join_iterator::operator++(); */

  /* Registro join_iterator::operator*(); */

  // @comentario chequeen si hace falta exponer pública completamente la estructura de indices.

  string_map<string_map<Indice> > indices(){
    return _indices;
  }

  string_map<Indice> indices(string nombre){
    return _indices.at(nombre);
  }

private:
  //////////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: basededatos \TO bool\n
     * rep(bd) \EQUIV 
     *  * _nombres_tablas = claves(_tablas) \LAND
     *  * \FORALL (c : Criterio) c \IN claves(_uso_criterios) \IMPLIES 
     *     * (
     *       * \EXISTS (n : string) n \IN _nombres_tablas 
     *       * \LAND criterioValido(c, n, bd)
     *     * ) \LAND
     *     * obtener(c, _uso_criterios) > 0 \LAND
     *     * (\FORALL indicc: dicc(indice)) indicc \IN _indices \IMPLIES ((\FORALL i:indice) i \IN indicc \IMPLIES (\EXISTS t:tabla) (t \IN _tablas \LAND campo(i) \IN campos(t)) \IMPLIES (\FORALL cc : campos(t)) (cc \IGOBS campo(i)) \IMPLIES (tipo(cc) \IGOBS esNat(i)) )

     *
     * abs: basededatos \TO BaseDeDatos\n
     * abs(bd) \EQUIV bd' \|
     *  * _nombres_tablas = tablas(bd') \LAND
     *  * (\FORALL nt : string) nt \IN _nombres_tablas \IMPLIES
     *    * obtener(nt, _tablas) = dameTabla(nt, bd') \LAND
     *  * (\FORALL c : criterio) 
     *    * (usoCriterio(c, bd') == 0 \LAND \LNOT def?(c, _uso_criterios)) \LOR
     *    * (usoCriterio(c, bd') == obtener(c, _uso_criterios)) \LAND
     *    (\FORALL indicc: dicc(indice)) indicc \IN _indices\IMPLIES ((\FORALL i:indice) i \IN indicc \IMPLIES (\EXISTS t:tabla) (t \IN _tablas \LAND tieneIndice(t,campo(i),bd'))
     */
//////////////////////////////////////////////////////////////////////////////////////////////////////


    /** @{ */
    linear_set<string> _nombres_tablas;
  string_map<Tabla> _tablas;
  string_map<string_map<Indice> > _indices;
    linear_map<Criterio, int> _uso_criterios;

    /** @} */

    /** @{ */
    /**
     * @brief Revisa si los campos del registro y la tabla tienen el mismo tipo.
     * @param r es el registro a revisar
     * @param t es la tabla en la que reviso
     *
     * \pre campos(r) == campos(t)
     * \post \P{res} == \FORALL (c : campo) c \IN campos(r) \IMPLIES
     * Nat?(valor(c, r)) == tipoCampo(c, t)
     *
     * \complexity{\O(C)}
     */

    bool _mismos_tipos(const Registro &r, const Tabla &t) const;
    /**
     * @brief Revisa que no haya ningun registro en la tabla que tenga los mismos valores en campos clave que el registro parametro
     * @param r es el registro con el que comparo
     * @param t es la tabla en la que reviso
     *
     * \pre compatible(r, t)
     * \post \P{res} = \FORALL (r' : Registro) r \IN registros(t) \IMPLIES
     *  \EXISTS (c : campo) c \IN claves(t) \LAND valor(c, r') != valor(c, r)
     *
     * \complexity{\O(c*n*l)}
     */

    bool _no_repite(const Registro &r, const Tabla &t) const;
    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     * @param campo es el campo en base al cual quiero filtrar
     * @param valor es el Dato por el que voy a filtrar
     * @param registros es la lista de registros de la que quiero filtrar
     * @param igualdad dice si quiero que esté o no el dato, decide cómo filtro
     *
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, igualdad), registros)
     * \complexity{\O(m)}
     */

    list<Registro> &_filtrar_registros(const string &campo, const Dato &valor,
                                      list<Registro> &registros,
                                      bool igualdad) const;
    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     * @param campo es el campo en base al cual quiero filtrar
     * @param valor es el Dato por el que voy a filtrar
     * @param registros es la lista de registros de la que quiero filtrar
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, true), registros)
     * \complexity{\O(m)}
     */
    list<Registro> &_filtrar_registros(const string &campo, const Dato &valor,
                                       list<Registro> &registros) const;

    /**
     * @brief Obtiene los campos y tipos de una tabla.
     * @param t es la tabla de la que quiero los campos y los tipos
     * \pre true
     * \post (\FORALL (c : Campo) está?(c, \P1(\P{res})) \IFF c \IN campos(t))
     *       \LAND #(campos(t)) = long(\P1(\P{res}))
     *       \LAND \FORALL (i : Nat) 0 \LEQ i < #(campos(t)) \IMPLIES
     *       tipo?(\P2(\P{res})[i]) = tipoCampo(\P1(\P{res})[i], t)
     * \complexity{\O(C)}
     */
	

    pair<vector<string>, vector<Dato> > _tipos_tabla(const Tabla &t);
    /** @} */
};

#include "join_iterator.h"

#endif
