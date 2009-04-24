/* 
 * Proview   $Id$
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#ifndef rt_pn_gsdml_data_h
#define rt_pn_gsdml_data_h

/* rt_pn_gsdml_data.h -- Profinet configurator data file. */

#include "co_xml_parser.h"

class GsdmlDataRecord {
 public:
  GsdmlDataRecord() : record_idx(0), data(0), data_length(0), index(0), transfer_sequence(0) {}
  
  unsigned int record_idx;
  unsigned char *data;
  unsigned int data_length;
  unsigned short index;
  unsigned short transfer_sequence;

  ~GsdmlDataRecord() { 
    if ( data)
      free( data);
  }
  GsdmlDataRecord( const GsdmlDataRecord& x) : record_idx(x.record_idx), data(x.data), 
    data_length(x.data_length), index(x.index), transfer_sequence(x.transfer_sequence) {
    if ( data) {
      data = (unsigned char *) malloc( data_length);
      memcpy( data, x.data, data_length);
    }
  }

  int print( ofstream& fp);
};

class GsdmlIOCRData {
 public:
  GsdmlIOCRData() : type(0), properties(0), send_clock_factor(0), api(0) {}
  
  unsigned short type;
  unsigned int properties;
  unsigned short send_clock_factor;
  unsigned short reduction_ratio;
  unsigned int api;

  int print( ofstream& fp);
};

class GsdmlSubslotData {
 public:
  GsdmlSubslotData() : subslot_number(0), subslot_idx(0) {}
  
  vector<GsdmlDataRecord *> data_record;
  unsigned int subslot_number;
  unsigned int subslot_idx;

  ~GsdmlSubslotData() { 
    for ( unsigned int i = 0; i < data_record.size(); i++)
      delete data_record[i];
  }
  GsdmlSubslotData( const GsdmlSubslotData& x) : subslot_number(x.subslot_number), 
    subslot_idx(x.subslot_idx) {
    for ( unsigned int i = 0; i < x.data_record.size(); i++) {
      data_record.push_back( new GsdmlDataRecord(*x.data_record[i]));
    }
  }
  int print( ofstream& fp);
};

class GsdmlSlotData {
 public:
  GsdmlSlotData() : module_enum_number(0), module_class(0), module_oid(pwr_cNOid),
    slot_number(0), slot_idx(0)
    { module_text[0]=0;}
  unsigned int module_enum_number;
  pwr_tCid module_class;
  pwr_tOid module_oid;
  char module_text[160];
  unsigned int slot_number;
  unsigned int slot_idx;
  vector<GsdmlSubslotData *> subslot_data;

  ~GsdmlSlotData() { 
    for ( unsigned int i = 0; i < subslot_data.size(); i++)
      delete subslot_data[i];
  }
  GsdmlSlotData( const GsdmlSlotData& x) : module_enum_number(x.module_enum_number), 
    module_class(x.module_class), module_oid(pwr_cNObjid), slot_number(x.slot_number),
    slot_idx(x.slot_idx) {
    for ( unsigned int i = 0; i < x.subslot_data.size(); i++) {
      subslot_data.push_back( new GsdmlSubslotData(*x.subslot_data[i]));
    }
  }
  int print( ofstream& fp);
};

class GsdmlDeviceData {
 public:
  GsdmlDeviceData() { device_name[0]=0; ip_address[0]=0; subnet_mask[0]=0; mac_address[0]=0;
    device_text[0]=0; version[0]=0;}
  char device_name[80];
  char ip_address[20];
  char subnet_mask[20];
  char mac_address[20];
  int device_num;
  char device_text[160];
  unsigned short vendor_id;
  unsigned short device_id;
  char version[20];
  int byte_order;
  vector<GsdmlSlotData *> slot_data;
  vector<GsdmlIOCRData *> iocr_data;
  static GsdmlSlotData *paste_slotdata;

  ~GsdmlDeviceData() { device_reset();}
  void device_reset() {
    for ( unsigned int i = 0; i < slot_data.size(); i++)
      delete slot_data[i];
    slot_data.clear();
    for ( unsigned int i = 0; i < iocr_data.size(); i++)
      delete iocr_data[i];
    iocr_data.clear();
  }
  int print( const char *filename);
  int read( const char *filename);
  int copy_slot( unsigned int slot_idx);
  int cut_slot( unsigned int slot_idx);
  int paste_slot( unsigned int slot_idx);
};

class GsdmlDataReader : public co_xml_interpreter {
 public:
  GsdmlDataReader( GsdmlDeviceData *d) : data(d) {}
  int tag( const char *name);
  int metatag( const char *name);
  int tag_end( const char *name);
  int metatag_end( const char *name);
  int tag_value( const char *value);
  int tag_attribute( const char *name, const char *value);

  int tag_name_to_id( const char *name, unsigned int *id);

  unsigned int current_tag;
  GsdmlDeviceData *data;
};

#endif
