BEGIN {
  printf( "%s : \\\n", target);
}

{
  if ( match( $0, "_i_methods.o") || match( $0, "_i_base_methods.o") || match( $0, "_io_base_methods.o")) {
    # Skip
  }
  else {

    gsub( "\\(/", "/");
    if ( match( $0, "\\)")) {
      gsub( "\\)", "(");
      printf( "%s) \\\n", $0);
    }
    else
      printf( "%s \\\n", $0);
  }
}

END {
  printf( "\n");
}