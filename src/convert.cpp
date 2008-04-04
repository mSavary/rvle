/** 
 * @file convert.cpp
 * @author The VLE Development Team
 */

/*
 * VLE Environment - the multimodeling and simulation environment
 * This file is a part of the VLE environment (http://vle.univ-littoral.fr)
 * Copyright (C) 2003 - 2008 The VLE Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vle/manager.hpp>
#include <vle/oov.hpp>
#include <vle/value.hpp>
#include <cassert>
#include <Rdefines.h>
#include "rvle.h"
#include "convert.h"

using namespace vle;


static void rvle_convert_view_matrix(const oov::OutputMatrix& matrix, SEXP out)
{
    value::MatrixFactory::ConstMatrixView view(matrix.values());
    value::MatrixFactory::ConstMatrixView::index i, j;

    for (i = 0; i < view.shape()[0]; ++i) {
        for (j = 0; j < view.shape()[1]; ++j) {
            switch (view[i][j]->getType()) {
            case value::ValueBase::BOOLEAN: {
                REAL(out)[j + i * view.shape()[1]] = (double)
                    value::toBoolean(view[i][j]);
                break;
            }
            case value::ValueBase::DOUBLE: {
                REAL(out)[j + i * view.shape()[1]] = 
                    value::toDouble(view[i][j]);
                break;
            }
            case value::ValueBase::INTEGER: {
                REAL(out)[j + i * view.shape()[1]] = (double)
                    value::toInteger(view[i][j]);
                break;
            }
            default: {
                REAL(out)[j + i * view.shape()[1]] = NA_REAL;
                break;
            }
            }
        }
    }
}

static void rvle_convert_vector_boolean(
                const value::MatrixFactory::ConstVectorView& vec,
                SEXP out)
{
    int i = 0;
    for (value::MatrixFactory::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if ((*it)->getType() == value::ValueBase::BOOLEAN) {
            LOGICAL(out)[i] = value::toBoolean(*it);
        } else {
            LOGICAL(out)[i] = NA_LOGICAL;
        }
        ++i;
    }
}

static void rvle_convert_vector_double(
                const value::MatrixFactory::ConstVectorView& vec,
                SEXP out)
{
    int i = 0;
    for (value::MatrixFactory::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if ((*it)->getType() == value::ValueBase::DOUBLE) {
            REAL(out)[i] = value::toDouble(*it);
        } else {
            REAL(out)[i] = NA_REAL;
        }
        ++i;
    }
}

static void rvle_convert_vector_integer(
                const value::MatrixFactory::ConstVectorView& vec,
                SEXP out)
{
    int i = 0;
    for (value::MatrixFactory::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if ((*it)->getType() == value::ValueBase::INTEGER) {
            INTEGER(out)[i] = value::toInteger(*it);
        } else {
            INTEGER(out)[i] = NA_INTEGER;
        }
        ++i;
    }
}

static void rvle_convert_vector_string(
                const value::MatrixFactory::ConstVectorView& vec,
                SEXP out)
{
    int i = 0;
    for (value::MatrixFactory::ConstVectorView::const_iterator it = vec.begin();
         it != vec.end(); ++it) {
        if ((*it)->getType() == value::ValueBase::STRING) {
            SET_STRING_ELT(out, i, mkChar(value::toString(*it).c_str()));
        } else {
            SET_STRING_ELT(out, i, NA_STRING);
        }
        ++i;
    }
}

static SEXP rvle_build_data_frame(const oov::OutputMatrix& matrix)
{
    value::MatrixFactory::ConstMatrixView view(matrix.values());
    SEXP ret, names, value;

    PROTECT(ret = NEW_LIST(view.shape()[0]));
    PROTECT(names = NEW_CHARACTER(view.shape()[0]));

    PROTECT(value = NEW_NUMERIC(view.shape()[1]));
    rvle_convert_vector_double(matrix.getValue(0), value);
    SET_VECTOR_ELT(ret, 0, value);
    SET_STRING_ELT(names, 0, mkChar("time"));
    UNPROTECT(1); // unprotect value

    const oov::OutputMatrix::MapPairIndex& index(matrix.index());
    for (oov::OutputMatrix::MapPairIndex::const_iterator it = index.begin();
         it != index.end(); ++it) {
        SET_STRING_ELT(names, it->second,
                       mkChar(boost::str(boost::format("%1%.%2%") %
                                         it->first.first %
                              it->first.second).c_str()));
        switch(view[0][it->second]->getType()) {
        case value::ValueBase::BOOLEAN:
            PROTECT(value = NEW_LOGICAL(view.shape()[1]));
            rvle_convert_vector_boolean(matrix.getValue(it->second),
                                        value);
            break;
        case value::ValueBase::DOUBLE:
            PROTECT(value = NEW_NUMERIC(view.shape()[1]));
            rvle_convert_vector_double(matrix.getValue(it->second),
                                       value);
            break;
        case value::ValueBase::INTEGER:
            PROTECT(value = NEW_INTEGER(view.shape()[1]));
            rvle_convert_vector_integer(matrix.getValue(it->second),
                                        value);
            break;
        case value::ValueBase::STRING:
            PROTECT(value = NEW_CHARACTER(view.shape()[1]));
            rvle_convert_vector_string(matrix.getValue(it->second),
                                       value);
            break;
        default:
            UNPROTECT(2); // unprotect ret and names
            error("not suppored type for (%s, %s), column (%d)",
                  it->first.first.c_str(), it->first.second.c_str(),
                  it->second);
        }

        SET_VECTOR_ELT(ret, it->second, value);
        UNPROTECT(1); // unprotect value
    }

    /* set the first column name's */
    PROTECT(value = NEW_CHARACTER(view.shape()[1]));
    for (int i = 0; i < view.shape()[1]; ++i) {
        SET_STRING_ELT(value, i, mkChar(boost::str(boost::format(
                        "%1%") % i ).c_str()));
    }
    setAttrib(ret, R_RowNamesSymbol, value);
    UNPROTECT(1);

    SET_NAMES(ret, names);
    SET_CLASS(ret, mkString("data.frame"));
    UNPROTECT(1); // unprotect names
    return ret;
}

//
// public R part
//

SEXP rvle_convert_simulation_matrix(rvle_output_t out)
{
    SEXP r;

    manager::OutputSimulationMatrix* matrix(
        reinterpret_cast < manager::OutputSimulationMatrix* >(out));

    PROTECT(r = allocMatrix(VECSXP, matrix->shape()[0], matrix->shape()[1]));

    manager::OutputSimulationMatrix::index i, j;
    for (i = 0; i < matrix->shape()[0]; ++i) {
        for (j = 0; j < matrix->shape()[1]; ++j) {
            const oov::OutputMatrixViewList& lst((*matrix)[i][j]);

            SEXP sexplst;
            PROTECT(sexplst = allocVector(VECSXP, lst.size()));
            SET_VECTOR_ELT(r, i + j * matrix->shape()[1], sexplst);

            oov::OutputMatrixViewList::const_iterator it;
            int n;
            for (it = lst.begin(), n = 0; it != lst.end(); ++it, ++n) {
                SEXP sexpdata;
                PROTECT(sexpdata = allocMatrix(REALSXP,
                                               it->second.values().shape()[1],
                                               it->second.values().shape()[0]));
                rvle_convert_view_matrix(it->second, sexpdata);

                SET_VECTOR_ELT(sexplst, n, sexpdata);
            }
            UNPROTECT(lst.size());
        }
        UNPROTECT(matrix->shape()[0] * matrix->shape()[1]);
    }
    UNPROTECT(1);
    return r;
}

SEXP rvle_convert_simulation_dataframe(rvle_output_t out)
{
    SEXP r;

    manager::OutputSimulationMatrix* matrix(
        reinterpret_cast < manager::OutputSimulationMatrix* >(out));

    PROTECT(r = allocMatrix(VECSXP, matrix->shape()[0], matrix->shape()[1]));

    manager::OutputSimulationMatrix::index i, j;
    for (i = 0; i < matrix->shape()[0]; ++i) {
        for (j = 0; j < matrix->shape()[1]; ++j) {
            const oov::OutputMatrixViewList& lst((*matrix)[i][j]);

            SEXP sexplst;
            PROTECT(sexplst = allocVector(VECSXP, lst.size()));
            SET_VECTOR_ELT(r, i + j * matrix->shape()[1], sexplst);

            oov::OutputMatrixViewList::const_iterator it;
            int n;
            for (it = lst.begin(), n = 0; it != lst.end(); ++it, ++n) {
                SEXP sexpdata = rvle_build_data_frame(it->second);
                SET_VECTOR_ELT(sexplst, n, sexpdata);
            }
            UNPROTECT(lst.size());
        }
        UNPROTECT(matrix->shape()[0] * matrix->shape()[1]);
    }
    UNPROTECT(1);
    return r;
}

