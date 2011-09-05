#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"
#include "rt_io_m_ssab_locals.h"

void bfbeth_set_read_req(io_sRackLocal *r, pwr_tUInt16 address) {
  r->read_req.item[r->next_read_req_item].address = address;
  r->next_read_req_item++;
  return;
}

void bfbeth_set_write_req(io_sRackLocal *r, pwr_tUInt16 address, pwr_tUInt16 data) {
  r->write_req.item[r->next_write_req_item].address = address;
  r->write_req.item[r->next_write_req_item].data = data;
  r->next_write_req_item++;
  return;
}

pwr_tUInt16 bfbeth_get_data(io_sRackLocal *r, pwr_tUInt16 address, int *sts) {
  int i;
  
  *sts = -1;
  for (i=0; i<350; i++) {
    if (r->read_area.item[i].address == address) {
      *sts = 1;
      return (r->read_area.item[i].data);
    }
  }
  return 0;
}

void bfbeth_get_write_status(io_sRackLocal *r, pwr_tUInt16 address, int *sts) {
  int i;
  
  *sts = 1;
  for (i=0; i<350; i++) {
    if ( (0x8000 | r->write_area.item[i].address) == address) {
      *sts = 0x8000 & r->write_area.item[i].address ? 1 : -1;
      return;
    }
  }
}
