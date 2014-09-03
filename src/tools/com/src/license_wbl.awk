BEGIN {
  inlicense = 0;
  rowcnt = 0;
}

{
  if ( $2 == "Proview" && $3 == "Open" && $4 == "Source" && $5 == "Process" && $6 == "Control.") {
    inlicense = 1;
    printf( "%s\n", $0);
  }
  else if ( inlicense) {
    rowcnt++;

    if ( rowcnt == 1) {
      printf( "!  Copyright (C) 2005-2014 SSAB AB.\n");
      inlicense = 0;
    }
    else 
      printf( "%s\n", $0);
  }
  else 
    printf( "%s\n", $0);
}

