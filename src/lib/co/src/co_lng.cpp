
#include <string.h>
#include <stdio.h>
 
extern "C" {
#include "co_dcli.h"
}
#include "co_lng.h"

lng_eLanguage Lng::lang = lng_eLanguage_en_us;

char Lng::items[][2][40] = {
      {"Help",         "Hjälp"},
      {"Help Class",   "Hjälp Klass"},
      {"Graph",        "Bild"},
      {"Trend",        "Trend"},
      {"Open Object",  "Öppna Objekt"},
      {"Open Object...", "Öppna Objekt"},
      {"RtNavigator",  "RtNavigatör"},
      {"Class Graph",  "Klass Bild"},
      {"Crossreferences", "Korsreferenser"},
      {"DataSheet",    "Datablad"},
      {"Collect",      "Samla"},
      {"Type Graph...","Typ Bild..."}, 
      {"Open Plc...",  "Öppna Plc..."},
      {"Alarm list",   "Larm lista"},
      {"Event list",   "Händelse lista"},
      {"Link",         "Länkar"},
      {"Subscription Client",  "Prenumerationer Klient"},
      {"Subscription Server",  "Prenumerationer Server"},
      {"Nethandler",   "Näthanterare"},
      {"Communication", "Kommunikation"},
      {"Device",       "Enheter"},
      {"PlcThread",    "PlcTrådar"},
      {"PlcPgm",       "PlcPgm"},
      {"Logging",      "Loggning"},
      {"Logging entry 1", "Loggning entry 1"},
      {"Logging entry 2", "Loggning entry 2"},
      {"Logging entry 3", "Loggning entry 3"},
      {"Logging entry 4", "Loggning entry 4"},
      {"Logging entry 5", "Loggning entry 5"},
      {"Logging entry 6", "Loggning entry 6"},
      {"Logging entry 7", "Loggning entry 7"},
      {"Logging entry 8", "Loggning entry 8"},
      {"Logging entry 9", "Loggning entry 9"},
      {"Logging entry 10", "Loggning entry 10"},
      {"Database",     "Databas"},
      {"Alarm",        "Larm"},
      {"Store",        "Lagra"},
      {"System",       "System"},
      {"Exit",         "Avsluta"},
      {"Active",       "Aktiv"},
      {"Insert",       "LäggIn"},
      {"Start",        "Starta"},
      {"Stop",         "Stoppa"},
      {"Restore",      "Återskapa"},
      {"ShowFile",     "VisaFil"},
      {"Time (ms)",    "Tid (ms)"},
      {"File",         "Fil"},
      {"Type",         "Typ"},
      {"BufferSize",   "BufferStorlek"},
      {"FullBufferStop","FullBufferStopp"},
      {"ShortName",    "KortNamn"},
      {"Parameter",    "Parameter"},
      {"ConditionParameter", "VillkorsParameter"},
      {"value >",      "värde >"},
      {"Description",  "Beskrivning"},
      {"InitialValue", "InitialVärde"},
      {"ActualValue",  "NuVärde"},
      {"Unit",         "Enhet"},
      {"Channel",      "Kanal"},
      {"Hold",         "Håll"},
      {"Slider",       "Reglage"},
      {"FilterType",   "FilterTyp"},
      {"Value",        "Värde"},
      {"Identity",     "Identitet"},
      {"Number",       "Nummer"},
      {"Range",        "Område"},
      {"SensorSignalValue", "GivarSignalVärde"},
      {"ChannelSignalValue", "KanalSignalVärde"},
      {"RawValue",     "RåVärde"},
      {"Update range", "Uppdatera område"},
      {"Signal",       "Signal"},
      {"TestOn",       "TestTill"},
      {"TestValue",    "TestVärde"},
      {"Mode",         "Mod"},
      {"Man",          "Man"},
      {"Auto",         "Auto"},
      {"Cascade",      "Kaskad"},
      {"Extern SetValue", "Externt BörVärde"},
      {"Force",        "Tvinga"},
      {"Set value",    "Bör värde"},
      {"Process value","Är värde"},
      {"Out value",    "Ut signal"},
      };


char *Lng::get_language_str()
{
  static char result[20];

  switch( lang) {
    case lng_eLanguage_sv_se:
      strcpy( result, "sv_se");
      break;
    default:
      strcpy( result, "en_us");
  }
  return result;
}

char *Lng::translate( char *str)
{
  static char result[200];
  char *in_p;

  if ( lang == lng_eLanguage_en_us) {
    // No translation is needed
    strcpy( result, str);
    return result;
  }

  // Remove space
  for ( in_p = str; *in_p == ' ' && *in_p; in_p++)
    ;

  for ( int i = 0; i < (int) (sizeof(items)/sizeof(items[0])); i++) {
    if ( strcmp( items[i][0], in_p) == 0) {
      strcpy( result, items[i][lang]);
      return result;
    }
  } 
  // Not found
  strcpy( result, str);
  return result;
}

int Lng::translate( char *in, char *out)
{
  char *in_p;

  if ( lang == lng_eLanguage_en_us) {
    // No translation is needed
    return 0;
  }

  // Remove space
  for ( in_p = in; *in_p == ' ' && *in_p; in_p++)
    ;

  for ( int i = 0; i < (int) (sizeof(items)/sizeof(items[0])); i++) {
    if ( strcmp( items[i][0], in_p) == 0) {
      strcpy( out, items[i][lang]);
      return 1;
    }
  } 
  // Not found
  return 0;
}

void Lng::get_uid( char *in, char *out)
{
  char result[200];

#if defined OS_VMS
  {
    char dev[80], dir[80], file[80], type[80];
    int version;
    char c;

    dcli_parse_filename( "pwr_exe:", dev, dir, file, type, &version);
    sprintf( result, "%s%s", dev, dir);
    c = result[strlen(result)-1];
    sprintf( &result[strlen(result)-1], ".%s%c%s%", get_language_str(), 
	     c, in);
  }
#else
  sprintf( result, "$pwr_exe/%s/%s", get_language_str(), in);
  dcli_translate_filename( result, result);
#endif
  strcpy( out, result);
}












