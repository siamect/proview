BEGIN {
  innum = 0;
  inbody = 0;
  innumaggr = 0;
  inbodyaggr = 0;
  pos = 0;
  par = "";
  inclass = "";
}

# Sv, ATv and DTv  Attr ActualValue -> Attr InitialValue

function sv( prefix, attr)
{
    if ( prefix "ActualValue" == attr)
      return prefix "InitialValue";
    return "";
}


{
  if ( $3 == "Sv" || 
       $3 == "ATv" ||
       $3 == "DTv") {
    innum = 1;
    inbody = 0;
    inclass = $3;
  }
  if ( innum) {
    if ( $1 == "Body" && $2 == "RtBody") {
      inbody = 1;
    }
    if ( inbody) {
      if ( $1 == "EndBody") {
        printf( "%s\n", $0);
        innum = 0;
      }
      else if ( $1 == "EndObject") {
        innum = 0;
        printf( "%s\n", $0);
      }
      else if ( $1 == "Attr") {
	if ( inclass == "Sv")
	  str = sv("", $2);
	else if ( inclass == "ATv") 
	  str = sv( "", $2);
	else if ( inclass == "DTv") 
	  str = sv( "", $2);

	if ( str != "") {
	  printf( "Attr   %s = ", str); 
	  for ( i = 4; i <= NF; i++)
	    printf("%s ", $i);
	  printf( "\n");
	}
	else {
	  printf( "%s\n", $0);
	}
      }
      else {
        printf( "%s\n", $0);
      }
    }
    else {
      printf( "%s\n", $0);
    }
  } 
  else {
    printf( "%s\n", $0);
  }
}
