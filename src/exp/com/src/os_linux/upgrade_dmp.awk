BEGIN {
  innum = 0;
  inbody = 0;
  pos = 0;
  par = "";
}

{
  if ( $3 == "BaseFcPPO5PbModule" || $3 == "BaseFcPPO3PbModule" ||
       $3 == "ABB_ACS_PPO5" || $3 == "ABB_ACC_PPO5") {
    innum = 1;
    inbody = 0;
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
  else {
    printf( "%s\n", $0);
  }
}
