/*
 * vim:expandtab:shiftwidth=8:tabstop=8:
 *
 * Copyright CEA/DAM/DIF  (2008)
 * contributeur : Philippe DENIEL   philippe.deniel@cea.fr
 *                Thomas LEIBOVICI  thomas.leibovici@cea.fr
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * ---------------------------------------
 */

/**
 * @file    nfs4_op_destroy_session.c
 * @brief   Routines used for managing the NFS4_OP_DESTROY_SESSION operation.
 *
 * Routines used for managing the NFS4_OP_DESTROY_SESSION operation.
 *
 *
 */
#include "config.h"
#include "sal_functions.h"

/**
 *
 * @brief The NFS4_OP_DESTROY_SESSION operation
 *
 * This function implements the NFS4_OP_DESTROY_SESSION operation.
 *
 * @param[in]     op   nfs4_op arguments
 * @param[in,out] data Compound request's data
 * @param[out]    resp nfs4_op results
 *
 * @return values as per RFC5661 p. 364
 *
 * @see nfs4_Compound
 *
 */

int nfs4_op_destroy_session(struct nfs_argop4 *op,
                            compound_data_t *data,
                            struct nfs_resop4 *resp)
{

#define arg_DESTROY_SESSION4 op->nfs_argop4_u.opdestroy_session
#define res_DESTROY_SESSION4 resp->nfs_resop4_u.opdestroy_session

  resp->resop = NFS4_OP_DESTROY_SESSION;
  res_DESTROY_SESSION4.dsr_status = NFS4_OK;

  if (data->minorversion == 0)
    {
      return (res_DESTROY_SESSION4.dsr_status = NFS4ERR_INVAL);
    }

  if(!nfs41_Session_Del(arg_DESTROY_SESSION4.dsa_sessionid))
    res_DESTROY_SESSION4.dsr_status = NFS4ERR_BADSESSION;
  else
    res_DESTROY_SESSION4.dsr_status = NFS4_OK;

  return res_DESTROY_SESSION4.dsr_status;
} /* nfs41_op_destroy_session */

/**
 * @brief Free memory allocated for result of nfs41_op_destroy_session
 *
 * This function frees memory allocated for result of
 * nfs41_op_destroy_session
 *
 * @param[in,out] resp  nfs4_op results
 *
 */
void nfs4_op_destroy_session_Free(DESTROY_SESSION4res * resp)
{
  return;
} /* nfs41_op_destroy_session_Free */