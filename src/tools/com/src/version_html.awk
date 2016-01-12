BEGIN {
}

{
  if ( $1 == "<p" && substr($2,0,18) == "id=\"pwrversion\">V") {
    printf( "    <p id=\"pwrversion\">%s</p>\n", version);
  }
  else 
    printf( "%s\n", $0);
}
