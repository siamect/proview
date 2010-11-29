BEGIN {
  innum = 0;
  inbody = 0;
  innumaggr = 0;
  inbodyaggr = 0;
  pos = 0;
  par = "";
}

{
  if ( $3 == "BaseFcPPO5PbModule" || $3 == "BaseFcPPO3PbModule" ||
       $3 == "ABB_ACS_PPO5" || $3 == "ABB_ACC_PPO5") {
    innum = 1;
    inbody = 0;
  }
  else if ( $3 == "BaseFcPPO5FanAggr" || $3 == "BaseFcPPO3FanAggr" ||
	    $3 == "BaseFcPPO5PumpAggr" || $3 == "BaseFcPPO3PumpAggr" ||
	    $3 == "BaseFcPPO5MotorAggr" || $3 == "BaseFcPPO3MotorAggr" ||
	    $3 == "BaseFcPPO5" || $3 == "BaseFcPPO3" ||
	    $3 == "ABB_ACS800_1" || $3 == "ABB_ACC800" ||
	    $3 == "ABB_ACS800MotorAggr" || $3 == "ABB_ACS800PumpAggr" ||
	    $3 == "ABB_ACS800FanAggr" || $3 == "ABB_Converter_ACS800_PPO3" ||
	    $3 == "ABB_Converter_ACS800_PPO5") {
    innumaggr = 1;
    inbodyaggr = 0;
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
	pos = index($2, ".");
	if ( pos != 0) {
	  a1 = substr( $2, 1, pos - 1);
	  a2 = substr( $2, pos + 1, length($2) - pos  + 1);

	  if ( substr( a1, 1, 2) == "I_" || substr( a1, 1, 2) == "O_") {
#	    printf( "Attr Io.%s.%s = %s\n", a1, a2, $4);
	    $2 = "Io." $2;
	    printf( "     %s\n", $0);
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
  if ( innumaggr) {
    if ( $1 == "Body" && $2 == "RtBody") {
      inbodyaggr = 1;
    }
    if ( inbodyaggr) {
      if ( $1 == "EndBody") {
        printf( "%s\n", $0);
        innumaggr = 0;
      }
      else if ( $1 == "EndObject") {
        innumaggr = 0;
        printf( "%s\n", $0);
      }
      else if ( $1 == "Attr") {
	pos = index($4, "PbModule.I_");
	if ( pos == 0)
	  pos = index($4, "PbModule.O_");
	if ( pos != 0) {
	  a1 = substr( $4, 1, pos - 1);
	  a2 = substr( $4, pos + 2, length($4) - pos - 1);

#         printf( "Attr %sIo%s = %s\n", a1, a2, $4);
	  $4 = a1 "Io" a2;
	  printf( "     %s\n", $0);
	}
	else {
	  pos = index($2, "IoConnect");
          if ( pos != 0) {
	    a2 = substr( $4, 0, length($4) - 1);
            $4 = a2 ".Io\"";
	    printf( "     %s\n", $0);
          }
          else {
	    printf( "%s\n", $0);
          }
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
