/*
 * Copyright (c) 2013 - Mauro Carvalho Chehab <m.chehab@samsung.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * Or, point your browser to http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "descriptors.h"
#include "dvb-fe.h"
#include "dvb-scan.h"
#include "dvb-frontend.h"
#include "dvb-v5-std.h"
#include "dvb-log.h"

#include "descriptors/pat.h"
#include "descriptors/pmt.h"
#include "descriptors/nit.h"
#include "descriptors/sdt.h"
#include "descriptors/eit.h"
#include "descriptors/vct.h"

#include "descriptors/desc_language.h"
#include "descriptors/desc_network_name.h"
#include "descriptors/desc_cable_delivery.h"
#include "descriptors/desc_sat.h"
#include "descriptors/desc_terrestrial_delivery.h"
#include "descriptors/desc_service.h"
#include "descriptors/desc_service_list.h"
#include "descriptors/desc_frequency_list.h"
#include "descriptors/desc_event_short.h"
#include "descriptors/desc_event_extended.h"
#include "descriptors/desc_atsc_service_location.h"
#include "descriptors/desc_hierarchy.h"

struct el_pid {
	uint8_t  type;
	uint16_t pid;
};

struct transport_table {
	uint16_t tr_id;
};

struct nit_table {
	uint16_t network_id;
	unsigned char version;
	char *network_name, *network_alias;
	struct transport_table *tr_table;
	unsigned tr_table_len;
	unsigned virtual_channel;
	unsigned area_code;

	/* Network Parameters */
	uint32_t delivery_system;
	uint32_t guard_interval;
	uint32_t fec_inner, fec_outer;
	uint32_t pol;
	uint32_t modulation;
	uint32_t rolloff;
	uint32_t symbol_rate;
	uint32_t bandwidth;
	uint32_t code_rate_hp;
	uint32_t code_rate_lp;
	uint32_t transmission_mode;
	uint32_t hierarchy;
	uint32_t plp_id;
	uint32_t system_id;

	unsigned has_dvbt:1;
	unsigned is_hp:1;
	unsigned has_time_slicing:1;
	unsigned has_mpe_fec:1;
	unsigned has_other_frequency:1;
	unsigned is_in_depth_interleaver:1;

	char *orbit;
	uint32_t *frequency;
	unsigned frequency_len;

	uint32_t *other_frequency;
	unsigned other_frequency_len;

	uint16_t *partial_reception;
	unsigned partial_reception_len;

	struct lcn_table *lcn;
	unsigned lcn_len;
};

struct dvb_v5_descriptors_program {
	struct dvb_table_pat_program *pat_pgm;
	struct dvb_table_pmt *pmt;
};

struct dvb_v5_descriptors {
	int verbose;
	uint32_t delivery_system;

	struct nit_table nit_table;

	/* New data */

	struct dvb_entry *entry;
	unsigned num_entry;

	struct dvb_table_pat *pat;
	struct dvb_table_vct *vct;
	struct dvb_v5_descriptors_program *program;
	struct dvb_table_nit *nit;
	struct dvb_table_sdt *sdt;

	unsigned num_program;
};

struct dvb_v5_descriptors *dvb_scan_alloc_handler_table(uint32_t delivery_system,
						       int verbose);

void dvb_scan_free_handler_table(struct dvb_v5_descriptors *dvb_scan_handler);