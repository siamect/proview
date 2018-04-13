void bfbeth_set_read_req(	io_sRackLocal *r, 
				pwr_tUInt16 address);

void bfbeth_set_write_req(	io_sRackLocal *r, 
				pwr_tUInt16 address,
				pwr_tUInt16 data);

pwr_tUInt16 bfbeth_get_data(	io_sRackLocal *r,
				pwr_tUInt16 address,
				int *sts);

void bfbeth_get_write_status(	io_sRackLocal *r,
				pwr_tUInt16 address,
				int *sts);
