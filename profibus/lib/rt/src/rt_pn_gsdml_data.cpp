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

/* cow_pn_gsdml_data.cpp -- Profinet configurator data file */

#include "glow_std.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "co_dcli.h"
#include "co_xml_parser.h"
#include "rt_pb_msg.h"
#include "rt_pn_gsdml_data.h"

typedef enum {
  gsdmldata_eTag_,
  gsdmldata_eTag_PnDevice,
  gsdmldata_eTag_NetworkSettings,
  gsdmldata_eTag_Slot,
  gsdmldata_eTag_Subslot,
  gsdmldata_eTag_DataRecord,
  gsdmldata_eTag_IOCR,
  gsdmldata_eTag_ChannelDiag,
  gsdmldata_eTag__
} gsdmldata_eTag;

GsdmlSlotData *GsdmlDeviceData::paste_slotdata = 0;

int GsdmlChannelDiag::print( ofstream& fp)
{
  fp <<
    "      <ChannelDiag ErrorType=\"" << error_type << "\"" << endl << 
    "         Name=\"" << name << "\"" << endl <<
    "         Help=\"" << help << "\" />" << endl;

  return 1;
}

int GsdmlDataRecord::print( ofstream& fp)
{
  char str[1024];

  co_xml_parser::data_to_ostring( data, data_length, str, sizeof(str));

  fp <<
    "      <DataRecord Index=\"" << index << "\"" << endl << 
    "         TransferSequence=\"" << transfer_sequence << "\"" << endl <<
    "         DataLength=\"" << data_length << "\"" << endl <<
    "         Data=\"" << str << "\" />" << endl;

  return 1;
}

int GsdmlSubslotData::print( ofstream& fp)
{
  fp <<
    "    <Subslot SubslotNumber=\"" << subslot_number << "\"" << endl <<
    "       SubmoduleIdentNumber=\"" << submodule_ident_number << "\"" << endl <<
    "       IOInputLength=\"" << io_input_length << "\"" << endl <<
    "       IOOutputLength=\"" << io_output_length << "\" >" << endl;

  for ( unsigned int i = 0; i < data_record.size(); i++) {
    data_record[i]->print( fp);
  }

  fp << 
    "    </Subslot>" << endl;
  return 1;
}

int GsdmlSlotData::print( ofstream& fp)
{
  fp <<
    "  <Slot ModuleEnumNumber=\"" << module_enum_number << "\"" << endl <<
    "        ModuleIdentNumber=\"" << module_ident_number << "\"" << endl <<
    "        ModuleClass=\"" << module_class << "\"" << endl <<
    "        ModuleText=\"" << module_text << "\"" << endl <<
    "        SlotNumber=\"" << slot_number << "\" >" << endl;

  for ( unsigned int i = 0; i < subslot_data.size(); i++) {
    subslot_data[i]->print( fp);
  }

  fp << 
    "  </Slot>" << endl;
  return 1;
}

int GsdmlIOCRData::print( ofstream& fp)
{
  fp <<
    "  <IOCR Type=\"" << type << "\"" << endl << 
    "    Properties=\"" << properties << "\"" << endl <<
    "    SendClockFactor=\"" << send_clock_factor << "\"" << endl <<
    "    ReductionRatio=\"" << reduction_ratio << "\"" << endl <<
    "    Phase=\"" << phase << "\"" << endl <<
    "    API=\"" << api << "\" />" << endl;

  return 1;
}

int GsdmlDeviceData::print( const char *filename)
{
  pwr_tFileName fname;
  ofstream fp;
  char *gsdmlfile_p;
  
  // Print name of gsdmlfile, not path
  if ( (gsdmlfile_p = strrchr(gsdmlfile, '/')))
    gsdmlfile_p++;
  else
    gsdmlfile_p = gsdmlfile;

  dcli_translate_filename( fname, filename);

  fp.open( fname);

  fp << 
    "<PnDevice GsdmlFile=\"" << gsdmlfile_p << "\"" << endl <<
    "  DeviceNumber=\"" << device_num << "\"" << endl <<
    "  DeviceText=\"" << device_text << "\"" << endl <<
    "  VendorId=\"" << vendor_id << "\"" << endl <<
    "  DeviceId=\"" << device_id << "\"" << endl <<
    "  Version=\"" << version << "\"" << endl <<
    "  ByteOrder=\"" << byte_order << "\">" << endl <<
    "  <NetworkSettings " << endl <<
    "    DeviceName=\"" << device_name << "\"" << endl <<
    "    IP_Address=\"" << ip_address << "\"" << endl <<
    "    SubnetMask=\"" << subnet_mask << "\"" << endl <<
    "    MAC_Address=\"" << mac_address << "\" />" << endl;

  for ( unsigned int i = 0; i < slot_data.size(); i++) {
    slot_data[i]->print( fp);
  }
  for ( unsigned int i = 0; i < iocr_data.size(); i++) {
    iocr_data[i]->print( fp);
  }
  for ( unsigned int i = 0; i < channel_diag.size(); i++) {
    channel_diag[i]->print( fp);
  }

  fp <<
    "</PnDevice>" << endl;   

  fp.close();

  return PB__SUCCESS;
}

int GsdmlDeviceData::read( const char *filename)
{
  int sts;
  GsdmlDataReader *reader = new GsdmlDataReader( this);
  co_xml_parser *xml_parser = new co_xml_parser( reader);

  sts = xml_parser->read( filename);
  delete xml_parser;
  delete reader;

  return sts;
}

int GsdmlDeviceData::copy_slot( unsigned int slot_idx)
{
  if ( slot_idx >= slot_data.size())
    return 0;

  if ( paste_slotdata)
    delete paste_slotdata;

  paste_slotdata = new GsdmlSlotData( *slot_data[slot_idx]);
  return PB__SUCCESS;
}

int GsdmlDeviceData::cut_slot( unsigned int slot_idx)
{
  if ( slot_idx >= slot_data.size())
    return 0;

  unsigned int tmp_slot_idx;
  unsigned int tmp_slot_number;
  unsigned int prev_slot_idx = slot_data[slot_idx]->slot_idx;
  unsigned int prev_slot_number = slot_data[slot_idx]->slot_number;

  delete slot_data[slot_idx];
  
  for ( unsigned int i = slot_idx; i < slot_data.size() - 1; i++) {
    tmp_slot_idx = slot_data[i+1]->slot_idx;
    tmp_slot_number = slot_data[i+1]->slot_number;

    slot_data[i] = slot_data[i+1];

    slot_data[i]->slot_idx = prev_slot_idx;
    slot_data[i]->slot_number = prev_slot_number;
    prev_slot_idx = tmp_slot_idx;
    prev_slot_number = tmp_slot_number;
  }
  slot_data[slot_data.size()-1] = new GsdmlSlotData();
  slot_data[slot_data.size()-1]->slot_idx = prev_slot_idx;
  slot_data[slot_data.size()-1]->slot_number = prev_slot_number;
  return PB__SUCCESS;  
}

int GsdmlDeviceData::paste_slot( unsigned int slot_idx)
{
  if ( slot_idx >= slot_data.size())
    return 0;

  if ( !paste_slotdata)
    return 0;

  unsigned int tmp_slot_idx;
  unsigned int tmp_slot_number;
  unsigned int prev_slot_idx = slot_data[slot_data.size()-1]->slot_idx;
  unsigned int prev_slot_number = slot_data[slot_data.size()-1]->slot_number;

  delete slot_data[slot_data.size()-1];
  
  for ( unsigned int i = slot_data.size()-1; i > slot_idx; i--) {
    tmp_slot_idx = slot_data[i-1]->slot_idx;
    tmp_slot_number = slot_data[i-1]->slot_number;
    
    slot_data[i] = slot_data[i-1];

    slot_data[i]->slot_idx = prev_slot_idx;
    slot_data[i]->slot_number = prev_slot_number;
    prev_slot_idx = tmp_slot_idx;
    prev_slot_number = tmp_slot_number;
  }
  slot_data[slot_idx] = new GsdmlSlotData(*paste_slotdata);
  slot_data[slot_idx]->slot_idx = prev_slot_idx;
  slot_data[slot_idx]->slot_number = prev_slot_number;
  return PB__SUCCESS;  
}

int GsdmlDataReader::tag_name_to_id( const char *name, unsigned int *id)
{
  if ( strcmp( name, "PnDevice") == 0)
    *id = gsdmldata_eTag_PnDevice;
  else if ( strcmp( name, "NetworkSettings") == 0)    
    *id = gsdmldata_eTag_NetworkSettings;
  else if ( strcmp( name, "Slot") == 0)    
    *id = gsdmldata_eTag_Slot;
  else if ( strcmp( name, "Subslot") == 0)    
    *id = gsdmldata_eTag_Subslot;
  else if ( strcmp( name, "DataRecord") == 0)    
    *id = gsdmldata_eTag_DataRecord;
  else if ( strcmp( name, "IOCR") == 0)    
    *id = gsdmldata_eTag_IOCR;
  else if ( strcmp( name, "ChannelDiag") == 0)    
    *id = gsdmldata_eTag_ChannelDiag;
  else
    return 0;

  return 1;
}

int GsdmlDataReader::tag( const char *name)
{
  if ( tag_name_to_id( name, &current_tag))
    tag_stack_push( current_tag);
  else {
    printf( "XML-Parser: Unknown tag: %s\n", name);
    return 0;
  }

  switch ( current_tag) {
  case gsdmldata_eTag_Slot: {
    GsdmlSlotData *sd = new GsdmlSlotData();
    
    data->slot_data.push_back( sd);
    object_stack_push( sd, current_tag);
    break;
  }
  case gsdmldata_eTag_Subslot: {
    GsdmlSlotData *sd = (GsdmlSlotData *) get_object_stack( gsdmldata_eTag_Slot);
    if ( !sd) {
      printf( "XML-Parser: Subslot outside slot");
      break;
    }

    GsdmlSubslotData *ssd = new GsdmlSubslotData();
    
    sd->subslot_data.push_back( ssd);
    object_stack_push( ssd, current_tag);
    break;
  }
  case gsdmldata_eTag_DataRecord: {
    GsdmlSubslotData *ssd = (GsdmlSubslotData *) get_object_stack( gsdmldata_eTag_Subslot);
    if ( !ssd) {
      printf( "XML-Parser: RecordData outside subslot");
      break;
    }

    GsdmlDataRecord *dr = new GsdmlDataRecord();
    
    ssd->data_record.push_back( dr);
    object_stack_push( dr, current_tag);
    break;
  }
  case gsdmldata_eTag_IOCR: {
    GsdmlIOCRData *iod = new GsdmlIOCRData();
    
    data->iocr_data.push_back( iod);
    object_stack_push( iod, current_tag);
    break;
  }
  case gsdmldata_eTag_ChannelDiag: {
    GsdmlChannelDiag *cd = new GsdmlChannelDiag();
    
    data->channel_diag.push_back( cd);
    object_stack_push( cd, current_tag);
    break;
  }
  default: ;
  }

  return 1;
}
int GsdmlDataReader::metatag( const char *name)
{
  return 1;
}
int GsdmlDataReader::tag_end( const char *name)
{
  unsigned int id;

  if ( tag_name_to_id( name, &id))
    tag_stack_pull( id);
  else {
    printf( "XML-Parser: Tag/EndTag mismach: %s\n", name);
    return 0;
  }

  switch( id) {
  case gsdmldata_eTag_Slot:
    object_stack_pull( id);
    break;
  case gsdmldata_eTag_Subslot:
    object_stack_pull( id);
    break;
  case gsdmldata_eTag_DataRecord:
    object_stack_pull( id);
    break;
  case gsdmldata_eTag_IOCR:
    object_stack_pull( id);
    break;
  default: ;
  }
  return 1;
}
int GsdmlDataReader::metatag_end( const char *name)
{
  return 1;
}
int GsdmlDataReader::tag_value( const char *name)
{
  return 1;
}
int GsdmlDataReader::tag_attribute( const char *name, const char *value)
{

  switch ( current_tag) {
  case gsdmldata_eTag_PnDevice:
    if ( strcmp( name, "GsdmlFile") == 0) {
      // Check that the GSDML file is not changed
      char *gsdmlfile_p;
  
      // Print name of gsdmlfile, not path
      if ( (gsdmlfile_p = strrchr(data->gsdmlfile, '/')))
	gsdmlfile_p++;
      else
	gsdmlfile_p = data->gsdmlfile;

      if ( strcmp( value, gsdmlfile_p) != 0)
	return PB__GSDMLFILEMISMATCH;
    }
    else if ( strcmp( name, "DeviceText") == 0)
      strncpy( data->device_text, value, sizeof(data->device_text));
    else if ( strcmp( name, "DeviceNumber") == 0)
      sscanf( value, "%d", &data->device_num);
    else if ( strcmp( name, "VendorId") == 0)
      sscanf( value, "%hu", &data->vendor_id);
    else if ( strcmp( name, "DeviceId") == 0)
      sscanf( value, "%hu", &data->device_id);
    else if ( strcmp( name, "Version") == 0)
      strncpy( data->version, value, sizeof(data->version));
    else if ( strcmp( name, "ByteOrder") == 0)
      sscanf( value, "%d", &data->byte_order);
    break;
  case gsdmldata_eTag_NetworkSettings:
    if ( strcmp( name, "DeviceName") == 0)
      strncpy( data->device_name, value, sizeof(data->device_name));
    else if ( strcmp( name, "IP_Address") == 0)
      strncpy( data->ip_address, value, sizeof(data->ip_address));
    else if ( strcmp( name, "SubnetMask") == 0)
      strncpy( data->subnet_mask, value, sizeof(data->subnet_mask));
    else if ( strcmp( name, "MAC_Address") == 0)
      strncpy( data->mac_address, value, sizeof(data->mac_address));
    break;
  case gsdmldata_eTag_Slot: {
    GsdmlSlotData *sd = (GsdmlSlotData *) get_object_stack( current_tag);

    if ( strcmp( name, "ModuleEnumNumber") == 0)
      sscanf( value, "%d", &sd->module_enum_number);
    else if ( strcmp( name, "ModuleIdentNumber") == 0)
      sscanf( value, "%u", &sd->module_ident_number);
    else if ( strcmp( name, "ModuleClass") == 0)
      sscanf( value, "%u", &sd->module_class);
    else if ( strcmp( name, "ModuleText") == 0)
      strncpy( sd->module_text, value, sizeof(sd->module_text));
    else if ( strcmp( name, "SlotNumber") == 0)
      sscanf( value, "%u", &sd->slot_number);
    break;
  }
  case gsdmldata_eTag_Subslot: {
    GsdmlSubslotData *sd = (GsdmlSubslotData *) get_object_stack( current_tag);

    if ( strcmp( name, "SubslotNumber") == 0)
      sscanf( value, "%u", &sd->subslot_number);
    else if ( strcmp( name, "SubmoduleIdentNumber") == 0)
      sscanf( value, "%u", &sd->submodule_ident_number);
    else if ( strcmp( name, "IOInputLength") == 0)
      sscanf( value, "%u", &sd->io_input_length);
    else if ( strcmp( name, "IOOutputLength") == 0)
      sscanf( value, "%u", &sd->io_output_length);
    break;
  }
  case gsdmldata_eTag_DataRecord: {
    GsdmlDataRecord *dr = (GsdmlDataRecord *) get_object_stack( current_tag);

    if ( strcmp( name, "DataLength") == 0)
      sscanf( value, "%u", &dr->data_length);
    else if ( strcmp( name, "Index") == 0)
      sscanf( value, "%hu", &dr->index);
    else if ( strcmp( name, "TransferSequence") == 0)
      sscanf( value, "%hu", &dr->transfer_sequence);
    else if ( strcmp( name, "Data") == 0) {
      int len;
      co_xml_parser::ostring_to_data( &dr->data, value, dr->data_length, &len);
    }
    break;
  }
  case gsdmldata_eTag_IOCR: {
    GsdmlIOCRData *iod = (GsdmlIOCRData *) get_object_stack( current_tag);

    if ( strcmp( name, "Type") == 0)
      sscanf( value, "%hu", &iod->type);
    else if ( strcmp( name, "Properties") == 0)
      sscanf( value, "%u", &iod->properties);
    else if ( strcmp( name, "SendClockFactor") == 0)
      sscanf( value, "%hu", &iod->send_clock_factor);
    else if ( strcmp( name, "ReductionRatio") == 0)
      sscanf( value, "%hu", &iod->reduction_ratio);
    else if ( strcmp( name, "Phase") == 0)
      sscanf( value, "%u", &iod->phase);
    else if ( strcmp( name, "API") == 0)
      sscanf( value, "%u", &iod->api);
    break;
  }
  case gsdmldata_eTag_ChannelDiag: {
    GsdmlChannelDiag *cd = (GsdmlChannelDiag *) get_object_stack( current_tag);

    if ( strcmp( name, "ErrorType") == 0)
      sscanf( value, "%hu", &cd->error_type);
    else if ( strcmp( name, "Name") == 0)
      strncpy( cd->name, value, sizeof(cd->name));
    else if ( strcmp( name, "Help") == 0)
      strncpy( cd->help, value, sizeof(cd->help));
    break;
  }
  default: ;
  }
  return 1;
}
