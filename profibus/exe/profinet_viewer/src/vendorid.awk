BEGIN {
  ininfo = 0;
  par = "";
}

{
  if ( $1 == "<Manufacturer") {
    par = substr($2,5,length($2)-6);
    printf( "%s ", par);
  }
  if ( ininfo == 1) {
    par = substr($0,23,length($0)-41);
    printf( "%s\n", par);
    ininfo = 0;
  }
  if ( $1 == "<ManufacturerInfo>") {
    ininfo = 1;
  }
}
