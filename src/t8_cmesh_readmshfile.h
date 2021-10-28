/*
  This file is part of t8code.
  t8code is a C library to manage a collection (a forest) of multiple
  connected adaptive space-trees of general element classes in parallel.

  Copyright (C) 2015 the developers

  t8code is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  t8code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with t8code; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

/** \file t8_cmesh_readmshfile.h
 * We define function here that serve to open a mesh file generated by
 * GMSH and consructing a cmesh from it.
 */

#ifndef T8_CMESH_READMSHFILE_H
#define T8_CMESH_READMSHFILE_H

#include <t8.h>
#include <t8_eclass.h>
#include <t8_cmesh.h>

/* The supported .msh file versions.
 * Currently, we support gmsh's file version 2 and 4 in ASCII format.
 */
#define T8_CMESH_N_SUPPORTED_MSH_FILE_VERSIONS 2

const int 
t8_cmesh_supported_msh_file_versions[T8_CMESH_N_SUPPORTED_MSH_FILE_VERSIONS] =
{
  2, 4
};

/* put typedefs here */

/* The nodes are stored in the .msh file in the format
 *
 * $Nodes
 * n_nodes     // The number of nodes
 * i x_i y_i z_i  // the node index and the node coordinates
 * j x_j y_j z_j
 * .....
 * $EndNodes
 *
 * The node indices do not need to be in consecutive order.
 * We thus use a hash table to read all node indices and coordinates.
 * The hash value is the node index modulo the number of nodes.
 */
typedef struct
{
  t8_locidx_t         index;
  double              coordinates[3];
} t8_msh_file_node_t;

typedef struct
{
  t8_locidx_t         index;
  double              coordinates[3];
  double              parameters[2];
  int                 entity_dim;
  t8_locidx_t         entity_tag;
} t8_msh_file_node_parametric_t;

T8_EXTERN_C_BEGIN ();

/* put declarations here */

/** Read a .msh file and create a cmesh from it.
 * \param [in]    fileprefix    The prefix of the mesh file.
 *                              The file fileprefix.msh is read.
 * \param [in]    partition     If true the file is only opened on one process
 *                              specified by the \a master argument and saved as
 *                              a partitioned cmesh where each other process does not
 *                              have any trees.
 * \param [in]    parametric    If true the file will be checked if parameters and
 *                              a brep geometry are present. If yes they will be read and
 *                              saved in an occ geometry.
 * \param [in]    comm          The MPI communicator with which the cmesh is to be committed.
 * \param [in]    dim           The dimension to read from the .msh files. The .msh format
 *                              can store several dimensions of the mesh and therefore the
 *                              dimension to read has to be set manually.
 * \param [in]    master        If partition is true, a valid MPI rank that will
 *                              read the file and store all the trees alone.
 * \return        A committed cmesh holding the mesh of dimension \a dim in the
 *                specified .msh file.
 */
t8_cmesh_t
t8_cmesh_from_msh_file (const char *fileprefix, int partition,
                        sc_MPI_Comm comm, int dim, int master);

T8_EXTERN_C_END ();

#endif /* !T8_CMESH_READMSHFILE_H */
