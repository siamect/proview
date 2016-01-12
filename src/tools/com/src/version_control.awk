BEGIN {
  base = 0;
  skip = 0;
  desc_cnt = 0;

  split( version, a, "-");
  if ( a[2] == "1")
    base = 1;

}

END {
  if ( !skip) {
    printf( " %s\n", version);
    printf( "  - %s\n", description);
  }
}

  
{
  if ( $1 == "Version:" )
    printf( "Version: %s\n", version);
  else if ( $1 == "Description:" ) {
    if ( base)
      desc_cnt = 1;
    printf( "%s\n", $0);
  }
  else {
    if ( desc_cnt > 0)
      desc_cnt++;

    if ( base && desc_cnt == 2) {
      printf( " %s Base release\n", version);
      skip = 1;
    }
    if ( !skip)
      printf( "%s\n", $0);
  }
}
