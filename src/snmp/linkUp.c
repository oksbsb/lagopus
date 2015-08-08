/*
 * Copyright 2014-2015 Nippon Telegraph and Telephone Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.notify.conf 10110 2004-04-15 12:29:19Z dts12 $
 */

#include <stdint.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "linkUp.h"

static oid snmptrap_oid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};

int
send_linkUp_trap(int32_t ifindex, int32_t ifAdminStatus,
                 int32_t ifOperStatus) {
  netsnmp_variable_list  *var_list = NULL;
  oid linkUp_oid[] = { 1,3,6,1,6,3,1,1,5,4 };
  oid ifIndex_oid[] = { 1,3,6,1,2,1,2,2,1,1, /* insert index here */ };
  oid ifAdminStatus_oid[] = { 1,3,6,1,2,1,2,2,1,7, /* insert index here */ };
  oid ifOperStatus_oid[] = { 1,3,6,1,2,1,2,2,1,8, /* insert index here */ };

  /*
   * Set the snmpTrapOid.0 value
   */
  snmp_varlist_add_variable(&var_list,
                            snmptrap_oid,
                            OID_LENGTH(snmptrap_oid),
                            ASN_OBJECT_ID,
                            (const u_char *)linkUp_oid, sizeof(linkUp_oid));

  /*
   * Add any objects from the trap definition
   */
  snmp_varlist_add_variable(&var_list,
                            ifIndex_oid,
                            OID_LENGTH(ifIndex_oid),
                            ASN_INTEGER,
                            (const u_char *)&ifindex, sizeof(ifindex));
  snmp_varlist_add_variable(&var_list,
                            ifAdminStatus_oid,
                            OID_LENGTH(ifAdminStatus_oid),
                            ASN_INTEGER,
                            (const u_char *)&ifAdminStatus, sizeof(ifAdminStatus));
  snmp_varlist_add_variable(&var_list,
                            ifOperStatus_oid,
                            OID_LENGTH(ifOperStatus_oid),
                            ASN_INTEGER,
                            (const u_char *)&ifOperStatus, sizeof(ifOperStatus));

  /*
   * Add any extra (optional) objects here
   */

  /*
   * Send the trap to the list of configured destinations
   *  and clean up
   */
  send_v2trap( var_list );
  snmp_free_varbind( var_list );

  return SNMP_ERR_NOERROR;
}
