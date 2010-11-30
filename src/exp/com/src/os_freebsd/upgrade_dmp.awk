BEGIN {
  innum = 0;
  inbody = 0;
  par = "";
}

{
  if ( $3 == "CStoNumSp") {
    innum = 1;
    inbody = 0;
  }
  if ( innum) {
    if ( $1 == "Body" && $2 == "DevBody") {
      inbody = 1;
    }
    if ( inbody) {
      if ( $1 == "EndBody") {
        printf( "Attr Object = \"%s\"\n", par);
        printf( "%s\n", $0);
        innum = 0;
      }
      else if ( $1 == "EndObject") {
        innum = 0;
        printf( "%s\n", $0);
      }
      else if ( $1 == "Attr" && $2 == "Object") {
        par = substr( $4, 2, length($4)-2);
      }
      else if ( $1 == "Attr" && $2 == "Parameter") {
        par = par "." substr($4,2,length($4)-2);
        printf( "Attr Object = \"%s\"\n", par);
        innum = 0;
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
