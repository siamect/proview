BEGIN {
}

END {
  printf( "  - %s\n", description);
}

  
{
  printf( "%s\n", $0);
 
}
