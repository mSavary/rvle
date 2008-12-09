/**
 * @file rvle.cpp
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



#include "rvle.h"
#include <vle/vpz.hpp>
#include <vle/manager.hpp>
#include <vle/value.hpp>
#include <vle/oov.hpp>
#include <cassert>

using namespace vle;

//
// C++ utilities
//

static void rvle_build_matrix(const oov::OutputMatrixViewList& view,
                              manager::OutputSimulationMatrix& matrix)
{
    manager::OutputSimulationMatrix::extent_gen extent;
    matrix.resize(extent[1][1]);
    matrix[0][0] = view;
}

//
// R interface
//

rvle_t rvle_open(const char* filename)
{
    assert(filename);

    vpz::Vpz*  file = 0;

    try {
        file = new vpz::Vpz(filename);
        return file;
    } catch(const std::exception& e) {
        return NULL;
    }
}

rvle_output_t rvle_run(rvle_t handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    try {
        manager::RunVerbose jrm(std::cerr);
        jrm.start(*file);
        const oov::OutputMatrixViewList& result(jrm.outputs());
        return new oov::OutputMatrixViewList(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

rvle_output_t rvle_manager(rvle_t handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    try {
        manager::ManagerRunMono jrm(std::cerr, false);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
        return new manager::OutputSimulationMatrix(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

rvle_output_t rvle_manager_thread(rvle_t handle, int th)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    try {
        manager::ManagerRunThread jrm(std::cerr, false, th);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
        return new manager::OutputSimulationMatrix(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}

rvle_output_t rvle_manager_cluster(rvle_t handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    try {
        manager::ManagerRunDistant jrm(std::cerr, false);
        jrm.start(*file);
        const manager::OutputSimulationMatrix& result(
            jrm.outputSimulationMatrix());
        return new manager::OutputSimulationMatrix(result);
    } catch(const std::exception& e) {
        return NULL;
    }
    return NULL;
}


void rvle_delete(rvle_t handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    delete file;
}

char** rvle_condition_list(rvle_t handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    std::list < std::string > lst;

    file->project().experiment().conditions().conditionnames(lst);
    char** result = NULL;

    if (lst.size()) {
        result = (char**)malloc(lst.size() * sizeof(char*));
        std::list < std::string >::iterator it = lst.begin();

        for (size_t i = 0; i < lst.size(); ++i) {
            result[i] = (char*)malloc((*it).size() + 1);
            strcpy(result[i], (*it).c_str());
            it++;
        }
    }

    return result;
}

char** rvle_condition_port_list(rvle_t handle, const char* conditionname)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    std::list < std::string > lst;
    char** result = NULL;

    try {
        const vpz::Condition& cnd(
            file->project().experiment().conditions().get(conditionname));

        cnd.portnames(lst);

        result = (char**)malloc(lst.size() * sizeof(char*));
        std::list < std::string >::iterator it = lst.begin();

        for (size_t i = 0; i < lst.size(); ++i) {
            result[i] = (char*)malloc((*it).size() + 1);
            strcpy(result[i], (*it).c_str());
            it++;
        }
    } catch(const std::exception& e) {
        return NULL;
    }

    return result;
}

int rvle_condition_port_list_size(rvle_t handle, const char* conditionname)
{
    assert(handle);

    int result;

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    try {
        vpz::Condition& cnd(
            file->project().experiment().conditions().get(conditionname));
        result = cnd.conditionvalues().size();
    } catch(const std::exception& e) {
        result = -1;
    }

    return result;
}

int rvle_condition_size(rvle_t handle)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    return file->project().experiment().conditions().conditionlist().size();
}

int rvle_condition_clear(rvle_t handle,
                         const char* conditionname,
                         const char* portname)
{
    assert(handle && conditionname && portname);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        vpz::Condition& cnd(file->project().experiment().
                            conditions().get(conditionname));

        cnd.clearValueOfPort(portname);
        return -1;
    } catch(const std::exception& e) {
        return 0;
    }
}

rvle_output_t rvle_condition_show(rvle_t handle,
		                  const char* conditionname,
				  const char* portname)
{
    assert(handle && conditionname && portname);

    try {
	vpz::Vpz* file(reinterpret_cast < vpz::Vpz* >(handle));
	vpz::Condition& cnd(file->project().experiment().
		conditions().get(conditionname));
	return new value::VectorValue(cnd.getSetValues(portname).value());
    } catch (const std::exception& e) {
	return 0;
    }
}

int rvle_condition_add_real(rvle_t handle,
                            const char* conditionname,
                            const char* portname,
                            double value)
{
    assert(handle && conditionname && portname);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        vpz::Condition& cnd(file->project().experiment().
                            conditions().get(conditionname));

        cnd.addValueToPort(portname, value::Double::create(value));
        return -1;
    } catch(const std::exception& e) {
        return 0;
    }
}

int rvle_condition_add_integer(rvle_t handle,
                               const char* conditionname,
                               const char* portname,
                               long value)
{
    assert(handle && conditionname && portname);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        vpz::Condition& cnd(file->project().experiment().
                            conditions().get(conditionname));

        cnd.addValueToPort(portname, value::Integer::create(value));
        return -1;
    } catch(const std::exception& e) {
        return 0;
    }
}

int rvle_condition_add_string(rvle_t handle,
                              const char* conditionname,
                              const char* portname,
                              const char* value)
{
    assert(handle && conditionname && portname);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        vpz::Condition& cnd(file->project().experiment().
                            conditions().get(conditionname));

        cnd.addValueToPort(portname, value::String::create(value));
        return -1;
    } catch(const std::exception& e) {
        return 0;
    }
}

int rvle_experiment_set_duration(rvle_t handle, double value)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    file->project().experiment().setDuration(value);

    return -1;
}

double rvle_experiment_get_duration(rvle_t handle)
{
    assert(handle);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));

        return file->project().experiment().duration();
    } catch(const std::exception& e) {
        return 0.0;
    }
}

int rvle_experiment_set_seed(rvle_t handle, uint32_t value)
{
    assert(handle);

    vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
    file->project().experiment().setSeed(value);

    return -1;
}

uint32_t rvle_experiment_get_seed(rvle_t handle)
{
    assert(handle);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));

        return file->project().experiment().seed();
    } catch(const std::exception& e) {
        return 0;
    }
}

int rvle_experiment_linear_combination(rvle_t handle, uint32_t seed,
                                       uint32_t replicas)
{
    assert(handle);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        file->project().experiment().setCombination("linear");
        file->project().experiment().replicas().setSeed(seed);
        file->project().experiment().replicas().setNumber(replicas);
    } catch(const std::exception& e) {
        return 0;
    }
}

int rvle_experiment_total_combination(rvle_t handle, uint32_t seed,
                                      uint32_t replicas)
{
    assert(handle);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        file->project().experiment().setCombination("total");
        file->project().experiment().replicas().setSeed(seed);
        file->project().experiment().replicas().setNumber(replicas);
    } catch(const std::exception& e) {
        return 0;
    }
}

int rvle_save(rvle_t handle, const char* filename)
{
    assert(handle and filename);

    try {
        vpz::Vpz*  file(reinterpret_cast < vpz::Vpz* >(handle));
        file->write(filename);
        return -1;
    } catch(const std::exception& e) {
        return 0;
    }
}

void rvle_clear_vectorvalue(rvle_output_t out)
{
    value::VectorValue* vect(reinterpret_cast < value::VectorValue* >(out));
    delete vect;
}

void rvle_clear_matrix(rvle_output_t out)
{
    manager::OutputSimulationMatrix* matrix(
        reinterpret_cast < manager::OutputSimulationMatrix* >(out));

    delete matrix;
}

void rvle_clear(rvle_output_t out)
{
    oov::OutputMatrixViewList* lst(
        reinterpret_cast < oov::OutputMatrixViewList* >(out));

    delete lst;
}
