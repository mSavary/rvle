#
# @file rvle.R
# @author The VLE Development Team
#

#
# VLE Environment - the multimodeling and simulation environment
# This file is a part of the VLE environment (http://vle.univ-littoral.fr)
# Copyright (C) 2003 - 2012 The VLE Development Team
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


.onLoad <- function(lib, pkg)
{
    if (.Platform$OS.type == "windows") {
      library(utils)
      registryKey = readRegistry("SOFTWARE\\VLE Development Team\\VLE 1.1.0");
      stopifnot(is.list(registryKey))
      stopifnot(length(registryKey) > 0)
      pathAdd = file.path(registryKey[[1]], "bin");
      Sys.setenv(PATH=paste(pathAdd, Sys.getenv("PATH"), sep=";"));
    }
    library.dynam("rvle", pkg, lib)
    x = .Call("__rvle_onload", PACKAGE="rvle")
}

.onUnload <- function(libpath)
{
    library.dynam.unload("rvle", libpath)
    x = .Call("__rvle_onunload", PACKAGE="rvle")
}

.rvle.compile_vle_output = function()
{
    x = .Call("__compile_vle_output", PACKAGE="rvle")
}

.rvle.compile_test_port = function()
{
    x = .Call("__compile_test_port", PACKAGE="rvle")
}

rvle.open <- function(file, pkg = "")
{
    stopifnot(is.character(file))
    stopifnot(is.character(pkg))

    if (pkg == "")
      x <- .Call("open", file, PACKAGE="rvle")
    else
      x <- .Call("open_pkg", file, pkg, PACKAGE="rvle")

    stopifnot(!is.null(x))
    class(x) <- 'rvle'
    return(x)
}

rvle.runPoly <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))
    
    .Call("run_poly", rvleHandle, PACKAGE="rvle")
}

rvle.run <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("run", rvleHandle, PACKAGE="rvle")
}

rvle.runMatrix <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("run_matrix", rvleHandle, PACKAGE="rvle")
}

rvle.runManagerPoly <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))
    
    .Call("run_manager_poly", rvleHandle, PACKAGE="rvle")
}


rvle.runManager <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("run_manager", rvleHandle, PACKAGE="rvle")
}

rvle.runManagerMatrix <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("run_manager_matrix", rvleHandle, PACKAGE="rvle")
}

rvle.runManagerThreadPoly <- function(rvleHandle, th)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("run_manager_thread_poly", rvleHandle, as.integer(th),
            PACKAGE="rvle")
}

rvle.runManagerThread <- function(rvleHandle, th)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("run_manager_thread", rvleHandle, as.integer(th),
			PACKAGE="rvle")
}

rvle.runManagerThreadMatrix <- function(rvleHandle, th)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("run_manager_thread_matrix", rvleHandle, as.integer(th), 
            PACKAGE="rvle")
}

is.rvle <- function(object)
{
    inherits(object, "rvle")
}

rvle.listConditions <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("condition_list", rvleHandle, PACKAGE="rvle")
}

rvle.listConditionPorts <- function(rvleHandle, condition)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))

    .Call("condition_port_list", rvleHandle, condition, PACKAGE="rvle")
}

rvle.getConditionsSize <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("condition_size", rvleHandle, PACKAGE="rvle")
}

rvle.getConditionPortsSize <- function(rvleHandle, condition)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))

    .Call("condition_port_list_size", rvleHandle, condition, PACKAGE="rvle")
}

rvle.clearConditionPort <- function(rvleHandle, condition, port)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))

    .Call("condition_clear", rvleHandle, condition, port, PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.getConditionPortValues <- function(rvleHandle, condition, port)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))

    .Call("condition_show", rvleHandle, condition, port, PACKAGE="rvle")
}

rvle.setDuration <- function(rvleHandle, value)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("experiment_set_duration", rvleHandle, as.numeric(value),
                                     PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.getDuration <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("experiment_get_duration", rvleHandle, PACKAGE="rvle")
}

rvle.setSeed <- function(rvleHandle, value)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("experiment_set_seed", rvleHandle, as.integer(value), PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.getSeed <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("experiment_get_seed", rvleHandle, PACKAGE="rvle")
}

rvle.setBegin <- function(rvleHandle, value)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("experiment_set_begin", rvleHandle, as.numeric(value), PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.getBegin <- function(rvleHandle)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("experiment_get_begin", rvleHandle, PACKAGE="rvle")
}

rvle.setLinearCombination <- function(rvleHandle, seed, repliquas)
{
    stopifnot(is.rvle(rvleHandle))

    .Call("experiment_linear_combination", rvleHandle, as.integer(seed),
	    as.integer(repliquas), PACKAGE="rvle")

    return (invisible(NULL))
}

rvle.listViews <- function(rvleHandle)
{
	stopifnot(is.rvle(rvleHandle))

	.Call("view_list", rvleHandle, PACKAGE="rvle")
}

rvle.getViewsSize <- function(rvleHandle)
{
	stopifnot(is.rvle(rvleHandle))

	.Call("view_size", rvleHandle, PACKAGE="rvle")
}

rvle.setOutputPlugin <- function(rvleHandle, viewname, pluginname)
{
	stopifnot(is.rvle(rvleHandle))
	stopifnot(is.character(viewname))
	stopifnot(is.character(pluginname))
    
    resplit = strsplit(pluginname,"/")[[1]]
    if (length(resplit) == 2){
        .Call("set_output_plugin", rvleHandle, viewname,
                resplit[2], resplit[1], PACKAGE="rvle")
    } else {
        
        .Call("set_output_plugin", rvleHandle, viewname, 
                pluginname, "vle.output" , PACKAGE="rvle")
    }
	return (invisible(NULL))
}

rvle.getOutputPlugin <- function(rvleHandle, view)
{
	stopifnot(is.rvle(rvleHandle))
	stopifnot(is.character(view))

	.Call("get_output_plugin", rvleHandle, view, PACKAGE="rvle")
}

rvle.save <- function(rvleHandle, file)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(file))

    .Call("save", rvleHandle, file, PACKAGE="rvle")

    return (invisible(NULL))
}

##NEW

rvle.addValueCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    .Call("rvle_addValueCondition", rvleHandle, condition, port, value,
            PACKAGE="rvle")
    
    return (invisible(NULL))
}

rvle.setValueCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    rvle.clearConditionPort(rvleHandle, condition, port)
    rvle.addValueCondition(rvleHandle, condition, port, value)
    
    return (invisible(NULL))
}

##DEPRECATED
rvle.addRealCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    .Call("condition_add_real", rvleHandle, condition, port, as.numeric(value),
            PACKAGE="rvle")
    
    return (invisible(NULL))
}

rvle.setRealCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    rvle.clearConditionPort(rvleHandle, condition, port)
    rvle.addRealCondition(rvleHandle, condition, port, value)
    
    return (invisible(NULL))
}

rvle.addTupleCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    .Call("condition_add_tuple", rvleHandle, condition, port, as.list(value),
            PACKAGE="rvle")
    
    return (invisible(NULL))
}

rvle.setTupleCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    rvle.clearConditionPort(rvleHandle, condition, port)
    rvle.addTupleCondition(rvleHandle, condition, port, as.list(value))
    
    return (invisible(NULL))
}

rvle.setStringCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    stopifnot(is.character(value))
    
    rvle.clearConditionPort(rvleHandle, condition, port)
    rvle.addStringCondition(rvleHandle, condition, port, value)
    
    return (invisible(NULL))
}

rvle.addStringCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    stopifnot(is.character(value))
    
    .Call("condition_add_string", rvleHandle, condition, port, value, PACKAGE="rvle")
    
    return (invisible(NULL))
}

rvle.addBooleanCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    .Call("condition_add_boolean", rvleHandle, condition, port, as.logical(value),
            PACKAGE="rvle")
    
    return (invisible(NULL))
}

rvle.setBooleanCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    rvle.clearConditionPort(rvleHandle, condition, port)
    rvle.addBooleanCondition(rvleHandle, condition, port, value)
    
    return (invisible(NULL))
}

rvle.addIntegerCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    .Call("condition_add_integer", rvleHandle, condition, port, as.integer(value),
            PACKAGE="rvle")
    
    return (invisible(NULL))
}

rvle.setIntegerCondition <- function(rvleHandle, condition, port, value)
{
    stopifnot(is.rvle(rvleHandle))
    stopifnot(is.character(condition))
    stopifnot(is.character(port))
    
    rvle.clearConditionPort(rvleHandle, condition, port)
    rvle.addIntegerCondition(rvleHandle, condition, port, value)
    
    return (invisible(NULL))
}


# vim:tw=80:ts=8:sw=4:sts=4
