#!/usr/bin/perl
#
# pwre.pl
#

%verbs = (
  "add", 	"add:usage_add",
  "configure", 	"configure:usage_configure",
  "build", 	"build:usage_build",
  "build_all", 	"build_all:usage_build_all",
  "build_kernel", "build_kernel:usage_build_kernel",
  "build_all_modules", "build_all_modules:usage_build_all_modules",
  "build_all_wbl", "build_all_wbl:usage_build_all_wbl",
  "ebuild",    "ebuild:usage_ebuild",
  "clean_exe_all", "clean_exe_all:usage_clean_exe_all",
  "method_build", "method_build:usage_method_build",
  "copy", 	"copy:usage_copy",
  "create", 	"create:usage_create",
  "create_all_modules", "create_all_modules:usage_create_all_modules",
  "delete", 	"dele:usage_dele",
  "help",	"help:usage_help",
  "list",	"list:usage_list",
  "init",	"init:usage_init",
  "import",	"import:usage_import",
  "module",	"module:usage_module",
  "merge",	"merge:usage_merge",
  "modify",	"modify:usage_modify",
  "show",	"show:usage_show",
  "tags",	"tags:usage_tags",
  "_build", 	"_build:",
  "_exists", 	"_exists:",
  "_print",	"_print:"
);

#
# Variables
#
$user = $ENV{"USER"};

$label  = $ENV{"pwre_env"};
$sroot	= "/view/$user/vobs/pwr_src/src";
$vmsinc = "";
$broot  = "/usr/users/$user/$user" . "_dbg";
$btype  = "dbg";
$os	= "linux";
if ($ENV{"pwre_hosttype"} eq "rs6000") {
  $hw	= "ppc";
} else {
  if ($ENV{"pwre_hosttype"} eq "x86_64") {
    $hw	= "x86_64";
  } else {
    $hw	= "x86";
  }
}

$desc	= $user. "'s environment";
@vars;
$varstr;


if (($dbname = $ENV{"pwre_env_db"}) eq "") {
  $dbname = $ENV{"HOME"} . "/pwre/pwre";
}
$os = substr( $ENV{"pwre_os"}, 3, 100);
$hw = substr( $ENV{"pwre_hw"}, 3, 100);
$configfile = $ENV{"pwre_broot"} . "/pwre_" . $hw . "_" . $os . ".cnf";


$arg1 = $ARGV[0];
shift;

if ($arg1 eq "") {
  help();
}



$amb = 0;
$found = 0;
@ambs;

foreach $key (sort keys (%verbs)) {
  if ($key =~ /^$arg1/) {
    if ($key =~ /$arg1\b/) {
      $amb = 0;
      $found = 1;
      ($verb, $usage)  =  split(/:/, $verbs{$key});
      last; 
    }

    push(@ambs, $key);
    if ($found == 1) {
      $amb = 1;
    } else {
      $found = 1;
     ($verb, $usage)  =  split(/:/, $verbs{$key});
    }
  }
}

if ($amb == 1) {
  printf("++\n++ Verb \"%s\" is ambigous\n", $arg1);
  printf("++ Select one of: ");
  foreach (@ambs) {
    printf("%s ", $_);
  }
  print("\n");
  exit 1;

} elsif ($found == 1) {
  &$verb(@ARGV);
  exit 0;

} else {
  help();
  exit 1;
}

#
# Verb subroutines


#
# add()
#
sub add ()
{

  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";
  $label = $_[0];
  $varstr = $envdb{$label};
  if ($varstr ne "") {
    printf("++ Environment %s already exists\n", $_[0]);
  } else {
    get_vars(@_);  
    update_db();
  }
  untie(%envdb)|| die "++ can't untie $dbname!";

}

#
# configure()
#
sub configure()
{
  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

  my $fname = $ENV{"pwre_bin"} . "/pwre_configure.sh"." ".$_[0]." ".$_[1]." ".$_[2];
  system( $fname);
}

#
# build()
#
sub build () # args: branch, subbranch, flavour, phase
{

  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

  if ( ! -e $configfile ) {
    print("++ Environment is not configured!\n");
    exit 1;
  }

  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";

  read_vars();

  my($branch) = $_[0];
  if (!defined($branch)) {
    usage_build();
    exit 1;
  }

  my($subbranch) = $_[1];
  if (!defined($subbranch)) {
    usage_build();
    exit 1;
  }

  shift(@_);
  shift(@_);

  if ( $_[0] eq "") {
    $flavour = "src";
  }
  else {
    $flavour = $_[0];
    shift(@_);
  }

  printf("--\n");
  printf("-- Build\n");
  printf("-- Branch.............: %s\n", $branch);
  printf("-- Sub branch.........: %s\n", $subbranch);
  printf("-- Sub branch flavour.: %s\n", $flavour);
  printf("-- Args...............: ");
  foreach (@_) {
    printf("%s ", $_);
    $ENV{"PWRE_ARGS"} = $_;
  }
  printf("\n--\n");
  
  _build($branch, $subbranch, $flavour, @_);

}

#
# build_kernel()
#
sub build_kernel # args: flavour
{
  my $flavour = $_[0];

  _module("xtt");
  build_all("copy", $flavour);
  merge();
  _module("rt");
  build_all("copy", $flavour);
  merge();
  _module("wb");
  build_all("copy", $flavour);
  merge();
  _module("nmps");
  build_all("copy", $flavour);
  merge();
  _module("remote");
  build_all("copy", $flavour);
  merge();
  _module("opc");
  build_all("copy", $flavour);
  merge();
  _module("rt");
  build_all("lib", $flavour);
  merge();
  _module("xtt");
  build_all("lib", $flavour);
  merge();
  _module("wb");
  build_all("lib", $flavour);
  merge();
  _module("otherio");
  _build("exp","rt","src","copy");
  _build("lib","usbio_dummy","src","init lib");
  _build("lib","usb_dummy","src","init lib");
  _build("lib","cifx_dummy","src","init lib");
  _build("lib","nodave_dummy","src","init lib");
  _build("lib","epl_dummy","src","init lib");
  merge();
  _module("xtt");
  build_all("exe", $flavour);
  merge();
  _module("wb");
  build_all("exe", $flavour);
  merge();
  _module("rt");
  build_all("exe", $flavour);
  merge();
}

#
# ebuild()
#
sub ebuild # args: pass flavour
{
  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";

  read_vars();

  my $fname = $ENV{"pwre_bin"} . "/ebuild.dat";
  open FILE, $fname or die $!;

  my $build_bcomp = 0;
  my $build_java = 0;
  my $build_remote = 0;
  my $build_nmps = 0;
  my $build_sev = 0;
  my $build_opc = 0;
  my $build_profibus = 0;
  my $build_otherio = 0;
  my $build_misc = 0;
  my $build_ssabox = 0;
  my $build_tlog = 0;
  my $build_othermanu = 0;
  my $build_abb = 0;
  my $build_siemens = 0;
  my $build_klocknermoeller = 0;
  my $build_inor = 0;
  my $build_telemecanique = 0;

  printf( "-- Building modules\n");
  while ( <FILE>) {

    my @value = split( ' ', $_);
    if ( $value[0] eq "bcomp" && $value[1] eq "1" ) { $build_bcomp = 1; printf( "--   bcomp\n");}
    elsif ( $value[0] eq "remote" && $value[1] eq "1" ) { $build_remote = 1; printf( "--   remote\n");}
    elsif ( $value[0] eq "java" && $value[1] eq "1" ) { $build_java = 1; printf( "--   java\n");}
    elsif ( $value[0] eq "nmps" && $value[1] eq "1" ) { $build_nmps = 1; printf( "--   nmps\n");}
    elsif ( $value[0] eq "sev" && $value[1] eq "1" ) { $build_sev = 1; printf( "--   sev\n");}
    elsif ( $value[0] eq "opc" && $value[1] eq "1" ) { $build_opc = 1; printf( "--   opc\n");}
    elsif ( $value[0] eq "profibus" && $value[1] eq "1" ) { $build_profibus = 1; printf( "--   profibus\n");}
    elsif ( $value[0] eq "otherio" && $value[1] eq "1" ) { $build_otherio = 1; printf( "--   otherio\n");}
    elsif ( $value[0] eq "misc" && $value[1] eq "1" )  { $build_misc = 1; printf( "--   misc\n");}
    elsif ( $value[0] eq "ssabox" && $value[1] eq "1" ) { $build_ssabox = 1; printf( "--   ssabox\n");}
    elsif ( $value[0] eq "tlog" && $value[1] eq "1" ) { $build_tlog = 1; printf( "--   tlog\n");}
    elsif ( $value[0] eq "othermanu" && $value[1] eq "1" ) { $build_othermanu = 1; printf( "--   othermanu\n");}
    elsif ( $value[0] eq "abb" && $value[1] eq "1" ) { $build_abb = 1; printf( "--   abb\n");}
    elsif ( $value[0] eq "siemens" && $value[1] eq "1" ) { $build_siemens = 1; printf( "--   siemens\n");}
    elsif ( $value[0] eq "klocknermoeller" && $value[1] eq "1" ) { $build_klocknermoeller = 1; printf( "--   klocknermoeller\n");}
    elsif ( $value[0] eq "inor" && $value[1] eq "1" )  { $build_inor = 1; printf( "--   inor \n");}
    elsif ( $value[0] eq "telemecanique" && $value[1] eq "1" )  { $build_telemecanique = 1; printf( "--   telemecanique\n");}
  }
  printf( "\n");
  close FILE;
 
  my $pass = $_[0];
  my $flavour = $_[1];
  if ( $_[1] eq "motif") {
    $flavour = "motif";
  }
  else {
    $flavour = "gtk";
  }

  if ( $pass eq "rt" ) {
    _module("rt");
    _build("exp", "inc", "src", "all");
    _build("exp", "com", "src", "all");
    _build("tools/exe", "*", "src", "all");
    merge("exe/tools_cmsg2c");
    merge("exe/tools_msg2cmsg");
    merge("exe/tools_pdrgen");
    _module("nmps");
    _build("lib","nmps","src","copy");
    merge();
    _module("rt");
    _build("msg", "*", "src", "all");
    _build("lib", "rt", "src", "init copy");
    _build("lib", "rt", $flavour, "copy");
    _build("lib", "co", "src", "init copy");
    _build("lib", "co", $flavour, "copy");
    _build("lib", "dtt", "src", "init copy");
    _build("lib", "statussrv", "src", "init copy");
    _build("lib", "co", "src", "all");
    _build("lib", "co", $flavour, "all");
    _build("exe", "co*", "src", "all");
    _build("exe", "*", "src", "copy");
    _build("exp", "stdsoap2", "src", "copy");  
    _build("lib", "rt", "src", "all");
    _build("lib", "statussrv", "src", "all");
    _build("lib", "msg_dummy", "src", "init copy lib");
    _build("exp", "rt", "src", "all");
    _build("exe", "wb_rtt", "src", "all");
    _build("lib", "dtt", "src", "all");
    _build("exp", "stdsoap2", "src", "all");
    merge();
    _module("otherio");
    _build("exp","rt","src","copy");
    _build("lib","usbio_dummy","src","init lib");
    _build("lib","usb_dummy","src","init lib");
    _build("lib","cifx_dummy","src","init lib");
    _build("lib","nodave_dummy","src","init lib");
    _build("lib","epl_dummy","src","init lib");
    merge();
    _module("rt");
    _build("exe", "rt*", "src", "all");
    _build("exe", "co*", $flavour, "all");
    _build("exe", "pwr_user", "src", "all");
    if ( $build_nmps == 1) {
      merge();
      _module("nmps");
      _build("lib", "nmps", "src", "all");
      merge();
    }
    if ( $build_bcomp == 1) {
      _module("profibus");
      merge();
      _module("bcomp");
      _build("lib", "rt", "src", "all");
      merge();
    }
    if ( $build_misc == 1) {
      _module("misc");
      _build("lib", "misc", "src", "all");
      merge();
    }
    if ( $build_ssabox == 1) {
      _module("ssabox");
      _build("lib", "rt", "src", "all");
      _build("lib", "ssabox", "src", "all");
      merge();
    }
    if ( $build_otherio == 1) {
      _module("otherio");
      _build("lib", "rt", "src", "all");
      merge();
    }
    if ( $build_profibus == 1) {
      _module("profibus");
      _build("lib", "rt", "src", "all");
      _build("lib","pnak_dummy","src","init lib");
      merge();
    }
    if ( $build_remote == 1) {
      _module("remote");
      _build("lib", "remote", "src", "all");
      _build("exe", "*", "src", "all");
      merge();
    }
    if ( $build_opc == 1) {
      _module("opc");
      _build("lib", "opc", "src", "all");
      _build("exe", "*", "src", "all");
      merge();
    }
    if ( $build_tlog == 1) {
      _module("tlog");
      _build("lib", "tlog", "src", "all");
      merge();
    }
    if ( $build_java == 1) {
      _module("java");
      _build("exe", "jpwr_rt_gdh", "src", "all");
      merge();
    }
    if ( $build_sev == 1) {
      _module("sev");
      _build("lib", "sev", "src", "all");
      _build("exe", "sev_server", "src", "all");
      merge();
    }
    if ( $build_abb == 1) {
      _module("abb");
      merge();
    }
    if ( $build_siemens == 1) {
      _module("siemens");
      merge();
    }
    if ( $build_othermanu == 1) {
      _module("othermanu");
      merge();
    }
    if ( $build_inor == 1) {
      _module("inor");
      merge();
    }
    if ( $build_klocknermoeller == 1) {
      _module("klocknermoeller");
      merge();
    }
    if ( $build_telemecanique == 1) {
      _module("telemecanique");
      merge();
    }
    if ( $build_misc == 1) {
      _module("misc");
      merge();
    }
  }
  elsif ( $pass eq "op" ) {
    _module("xtt");
    _build("lib", "*", "src", "init copy");
    _build("lib", "*", $flavour, "copy");
    _build("exp", "*", "src", "init copy");
    _build("mmi", "*", "src", "copy");
    _build("mmi", "*", $flavour, "copy");
    _build("exe", "*", "src", "copy");
    _build("lib", "*", "src", "lib");
    _build("lib", "*", $flavour, "lib");
    _build("exp", "*", "src", "lib");
    _build("exe", "rt*", "src", "all");
    _build("exe", "xtt*", "src", "all");
    _build("exe", "co*", "src", "all");
    _build("exe", "pwr*", "src", "all");
    _build("exe", "rt*", $flavour, "all");
    _build("exe", "xtt*", $flavour, "all");
    _build("exe", "co*", $flavour, "all");
    _build("exe", "pwr*", $flavour, "all");
    merge();
    if ( $build_bcomp == 1) {
      _module("bcomp");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_profibus == 1) {
      _module("profibus");
      _build("mmi", "*", "src", "copy");
      _build("lib", "cow", "src", "init copy lib");
      _build("lib", "cow", $flavour, "init copy lib");
      _build("lib", "xtt", "src", "init copy lib");
      _build("lib", "xtt", $flavour, "init copy lib");
      merge();
    }
    if ( $build_opc == 1) {
      _module("opc");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_otherio == 1) {
      _module("otherio");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_sev == 1) {
      _module("sev");
      _build("exe", "sev_xtt", "src", "all");
      _build("exe", "sev_xtt", $flavour, "all");
      merge();
    }
    if ( $build_othermanu == 1) {
      _module("othermanu");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_abb == 1) {
      _module("abb");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_inor == 1) {
      _module("inor");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_siemens == 1) {
      _module("siemens");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_klocknermoeller == 1) {
      _module("klocknermoeller");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_misc == 1) {
      _module("misc");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_ssabox == 1) {
      _module("ssabox");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    if ( $build_telemecanique == 1) {
      _module("telemecanique");
      _build("mmi", "*", "src", "copy");
      merge();
    }
    method_build( "rt_io_comm", $flavour);
    method_build( "rt_xtt", $flavour);
  }
}

#
# import()
#
sub import ()
{
  my $flavour = $_[1];
  if ( $_[1] eq "motif") {
    $flavour = "motif";
  }
  else {
    $flavour = "gtk";
  }

  my($vmsinc) = $ENV{"pwre_vmsinc"};
  if ( $vmsinc ne "" ) {

    copy( $_[0], $flavour);

    _module("rt");
    merge();
    _module("bcomp");
    merge();
    _module("nmps");
    merge();
    _module("opc");
    merge();
    _module("profibus");
    merge();
    _module("otherio");
    merge();
    _module("remote");
    merge();
  }
  
}

#
# build_all()
#
sub build_all_modules ()
{
  my $flavour = $_[0];

  build_kernel( $flavour);
  _module("nmps");
  build_all( $flavour);
  merge();
  _module("tlog");
  build_all( $flavour);
  merge();
  _module("remote");
  build_all( $flavour);
  merge();
  _module("profibus");
  build_all( $flavour);
  merge();
  _module("bcomp");
  build_all( $flavour);
  merge();
  _module("java");
  build_all( $flavour);
  merge();
  _module("opc");
  build_all( $flavour);
  merge();
  _module("misc");
  build_all( $flavour);
  merge();
  _module("ssabox");
  build_all( $flavour);
  merge();
  _module("othermanu");
  build_all( $flavour);
  merge();
  _module("otherio");
  build_all( $flavour);
  merge();
  _module("sev");
  build_all( $flavour);
  merge();
  _module("abb");
  build_all( $flavour);
  merge();
  _module("siemens");
  build_all( $flavour);
  merge();
  _module("inor");
  build_all( $flavour);
  merge();
  _module("klocknermoeller");
  build_all( $flavour);
  merge();
  _module("telemecanique");
  build_all( $flavour);
  merge();

  method_build( $flavour);
}  

#
# clean_exe_all()
#
sub clean_exe_all ()
{
  _module("xtt");
  _build("exe","*","src","clean_exe");
  _module("rt");
  _build("exe","rt*","src","clean_exe");
  _build("exe","co*","src","clean_exe");
  _build("exe","wb*","src","clean_exe");
  _module("wb");
  _build("exe","*","src","clean_exe");
  _module("nmps");
  _build("exe","*","src","clean_exe");
  _module("tlog");
  _build("exe","*","src","clean_exe");
  _module("remote");
  _build("exe","*","src","clean_exe");
  _module("rt");

}  


#
# build_all_wbl()
#
sub build_all_wbl ()
{
  _module("rt");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "pwrs", "src", "lib");
  _build("wbl", "pwrb", "src", "lib");
  _build("wbl", "wb", "src", "lib");
  _build("wbl", "rt", "src", "lib");
  merge();
  _module("nmps");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "nmps", "src", "lib");
  merge();
  _module("tlog");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "tlog", "src", "lib");
  merge();
  _module("profibus");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("opc");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("remote");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "remote", "src", "lib");
  merge();
  _module("bcomp");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "bcomp", "src", "lib");
  merge();
  _module("misc");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "misc", "src", "lib");
  merge();
  _module("ssabox");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("abb");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("siemens");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("telemecanique");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("inor");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("klocknermoeller");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("otherio");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("othermanu");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "src", "lib");
  merge();
  _module("rt");
}  

sub method_build ()
{
  my $flavour;
  my $program;
  if ( $_[0] eq "motif" || $_[0] eq "gtk") {
    $flavour = $_[0];
  }
  else {
    $program = $_[0];
    $flavour = $_[1];
  }
  if (!defined($program)) {
    printf("-- Relink method dependent programs $flavour");
    _module("rt");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/rt_io_comm");
    _build("exe", "rt_io_comm", "src", "all");
#   system("rm $exe_dir/rt_ini");
#   _build("exe", "rt_ini", $flavour, "all");
    merge();
  
    method_build( "wb", $flavour);
#    _module("wb");
#    my($exe_dir) = $ENV{"pwr_exe"};
#    system("rm $exe_dir/wb");
#    _build("exe", "wb", "src", "all");
#    _build("exe", "wb", $flavour, "all");
#    merge();
  
    method_build( "rt_xtt", $flavour);
#    _module("xtt");
#    my($exe_dir) = $ENV{"pwr_exe"};
#    system("rm $exe_dir/rt_xtt");
#    _build("exe", "rt_xtt", "src", "all");
#    _build("exe", "rt_xtt", $flavour, "all");
#    merge();
  }  
  if ( $_[0] eq "rt_io_comm" ) {
    printf("-- Method build $program");
    _module("rt");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/rt_io_comm");
    _build("exe", "rt_io_comm", "src", "all");
    merge();
  }
  if ( $_[0] eq "wb" ) {
    printf("-- Method build $program $flavour");

    _module("wb");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/wb");
    _build("exe", "wb", $flavour, "all");
    _build("exe", "wb", "src", "all");
    merge();
  }
  if ( $_[0] eq "rt_xtt" ) {
    printf("-- Method build $program $flavour");

    _module("xtt");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/rt_xtt");
    _build("exe", "rt_xtt", $flavour, "all");
    _build("exe", "rt_xtt", "src", "all");
    merge();
  }  
}

sub create_all_modules ()
{
  if ( ! -e $configfile ) {
    configure();
  }

  _module("rt");
  create();
  _module("xtt");
  create();
  _module("wb");
  create();
  _module("nmps");
  create();
  _module("tlog");
  create();
  _module("remote");
  create();
  _module("java");
  create();
  _module("bcomp");
  create();
  _module("misc");
  create();
  _module("profibus");
  create();
  _module("sev");
  create();
  _module("opc");
  create();
  _module("ssabox");
  create();
  _module("othermanu");
  create();
  _module("otherio");
  create();
  _module("abb");
  create();
  _module("siemens");
  create();
  _module("inor");
  create();
  _module("klocknermoeller");
  create();
  _module("telemecanique");
  create();
}

#
# build_all()
#
sub build_all ()
{
  $copy = 0;
  $lib = 0;
  $exe = 0;

  if ( $_[0] eq "copy" || $_[1] eq "copy" || $_[2] eq "copy") {
    $copy = 1;
  }
  if ( $_[0] eq "lib" || $_[1] eq "lib" || $_[2] eq "lib") {
    $lib = 1;
  }
  if ( $_[0] eq "exe" || $_[1] eq "exe" || $_[2] eq "exe") {
    $exe = 1;
  }
  if ( $copy == 0 && $lib == 0 && $exe == 0) {
    $copy = 1;
    $lib = 1;
    $exe = 1;
  }
  if ( $_[0] eq "motif" || $_[1] eq "motif" || $_[2] eq "motif" || $_[3] eq "motif") {
    $flavour = "motif";
  }
  else {
    $flavour = "gtk";
  }

  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }
  my($module) = $ENV{"pwre_module"};
  my($einc) = $ENV{"pwr_einc"};
  my($remoteclasses) = $einc . "/pwr_remoteclasses.h";
  my($nmpsclasses) = $einc . "/pwr_nmpsclasses.h";

  printf("--\n");
  printf("-- Build all\n");
  show();

  if ( $module eq "rt") {
    if ( $copy == 1) {
      _build("exp", "inc", "src", "all");
      _build("exp", "com", "src", "all");
      _build("tools/exe", "*", "src", "all");
      merge("exe/tools_cmsg2c");
      merge("exe/tools_msg2cmsg");
      merge("exe/tools_pdrgen");
      _build("msg", "*", "src", "all");
      _build("lib", "rt", "src", "init copy");
      _build("lib", "co", "src", "init copy");
      _build("lib", "dtt", "src", "init copy");
      _build("lib", "statussrv", "src", "init copy");
      _build("lib", "co", "src", "all");
      _build("exe", "co*", "src", "all");
      _build("wbl", "pwrs", "src", "copy");
      _build("wbl", "pwrb", "src", "copy");
      _build("exe", "*", "src", "copy");
      _build("exp", "stdsoap2", "src", "copy");
    }

    if ( $lib == 1) {
      _build("lib", "ge", "all");
      if ( ! -e $nmpsclasses ) {
        _module("nmps");
        _build("wbl", "nmps", "src", "init copy");
        merge("inc/pwr_nmpsclasses.h");
      }
      if ( ! -e $remoteclasses ) {
        _module("remote");
        _build("wbl", "remote", "src", "init copy");
        merge("inc/pwr_remoteclasses.h");
      }
      _module("rt");
      _build("lib", "rt", "src", "all");
      _build("exp", "rt", "src", "all");
      _build("lib", "msg_dummy", "src", "all");
      _build("exp", "rt", "src", "all");
      _build("lib", "statussrv", "src", "all");
      _build("exp", "stdsoap2", "src", "all");
    }
    if ( $exe == 1) {
      _build("exe", "wb_rtt", "src", "all");
      _build("lib", "dtt", "src", "all");
      _build("exe", "rt*", "src", "all");
      _build("exe", "co*", $flavour, "all");
      _build("exe", "pwr_user", "src", "all");
      _build("exe", "pwr_rtmon", "src", "all");
      _build("exe", "pwr_rtmon", "$flavour", "all");
      _build("exp", "ge", "src", "all");
      _build("mmi", "*", "src", "copy");
      _build("mmi", "*", $flavour, "copy");
      _build("wbl", "pwrs", "src", "lib");
      _build("wbl", "pwrb", "src", "lib");
      _build("wbl", "wb", "src", "lib");
      _build("wbl", "rt", "src", "lib");
      _build("doc", "web", "src", "all");
      _build("doc", "dweb", "src", "all");
      _build("doc", "orm", "src", "all");
      _build("doc", "prm", "src", "all");
      _build("doc", "man", "src", "all");
      _build("doc", "dox", "src", "all");
      _build("wbl", "pwrs", "src", "exe");
      _build("wbl", "pwrb", "src", "exe");
    }
  }
  elsif ( $module eq "java") {
    if ( $lib == 1) {
      _build("jpwr", "rt", "src", "all");
      _build("jpwr", "jopg", "src", "all");
      _build("jpwr", "jop", "src", "all");
#      _build("jpwr", "jopc", "src", "all");
#      _build("jpwr", "beans", "src", "all");
      _build("jpwr", "rt_client", "src", "all");
    }
    if ( $exe == 1) {
      _build("exe", "jpwr_rt_gdh", "src", "all");
    }
  }
  elsif ( $module eq "profibus") {
    if ( $copy == 1) {
      _build("wbl", "*", "src", "copy");
      _build("lib", "*", "src", "init copy");
      _build("lib", "*", $flavour, "copy");
      _build("exp", "*", "src", "init copy");
      _build("mmi", "*", "src", "copy");
      _build("mmi", "*", $flavour, "copy");
      _build("exe", "*", "src", "copy");
    }
    if ( $lib == 1) {
      _build("lib", "*", "src", "lib");
      _build("lib", "*", $flavour, "lib");
      _build("exp", "*", "src", "lib");
    }
    if ( $exe == 1) {
      _build("wbl", "*", "src", "lib");
      _build("wbl", "*", "src", "exe");
      if ( $lib == 1) {
	merge();
      }
      _build("exe", "*", "src", "all");
      _build("exe", "*", $flavour, "all");
    }
  }
  else {
    if ( $copy == 1) {
      _build("wbl", "*", "src", "copy");
      _build("lib", "*", "src", "init copy");
      _build("lib", "*", $flavour, "copy");
      _build("exp", "*", "src", "init copy");
      _build("mmi", "*", "src", "copy");
      _build("mmi", "*", $flavour, "copy");
      _build("exe", "*", "src", "copy");
    }
    if ( $lib == 1) {
      _build("lib", "*", "src", "lib");
      _build("lib", "*", $flavour, "lib");
      _build("exp", "*", "src", "lib");
    }
    if ( $exe == 1) {
      _build("wbl", "*", "src", "lib");
      _build("wbl", "*", "src", "exe");
      _build("exe", "*", "src", "all");
      _build("exe", "*", $flavour, "all");
      _build("doc", "dsh", "src", "copy");
      _build("doc", "orm", "src", "copy");
    }
  }
}


#
# copy()
#
sub copy () 
{
  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

  my($vmsinc) = $ENV{"pwre_vmsinc"};
  my($bindir) = $ENV{"pwre_bin"} . "/" . $ENV{"pwre_hw"};
  my($docroot) = $ENV{"pwre_broot"} . "/" . $ENV{"pwre_os"} . "/" . $ENV{"pwre_hw"} . "/exp/doc/";
  my($exproot) = $ENV{"pwre_broot"} . "/" . $ENV{"pwre_os"} . "/" . $ENV{"pwre_hw"};

  printf("--\n");
  printf("-- Copy file from Import root\n");
  printf("-- Import root: %s\n", $vmsinc);
  printf("--\n");
  
  if ( $_[0] eq "doc" ) {
    my($cmd) = "cp -r " . $vmsinc . "/exp/doc/* " . $docroot; 
    system("$cmd");
  }
  elsif ( $_[0] eq "dbs" ) {
    my($cmd) = "$bindir/import_dbs.sh " . $vmsinc . " " . $exproot; 
    system("$cmd");
  }
  else {
    my($cmd) = "make -f $bindir/import_files.mk" . " " . $_[0]; 
    system("$cmd");

    my $flavour = $_[1];
    if ( $_[1] eq "motif") {
      $flavour = "motif";
    }
    else {
      $flavour = "gtk";
    }

    my($cmd) = "make -f $bindir/import_files.mk" . " " . $_[0] . "_" . $flavour; 
    system("$cmd");
  }
}

#
# create
#
sub create()
{
  my($newdir);
  my($root) = $ENV{"pwre_broot"};
  my($module) = $ENV{"pwre_bmodule"};


 if (!defined($root)) {
    printf("++\n++ No build root is defined\n");
    exit 1;
  }

  create_dir($root);

  $root .= "/" . $ENV{"pwre_os"};
  create_dir($root);

  $root .= "/" . $ENV{"pwre_hw"};
  create_dir($root);

  printf( "-- Creating build tree %s/%s\n", $root, $module);

  $newdir = $root . "/bld";
  create_dir($newdir);
  create_dir($newdir . "/exe");
  create_dir($newdir . "/lib");
  create_dir($newdir . "/msg");
  create_dir($newdir . "/jpwr");
  create_dir($newdir . "/wbl");

  if ($module eq "rt") {
    $newdir = $root . "/exp";
    create_base($newdir);
  }
#  else {
    $newdir = $root . "/$module";
    create_base($newdir);
#  }
  $newdir = $root . "/tmp";
  create_dir($newdir);
}

#
# dele()
#
sub dele ()
{

  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";

  $label = $_[0];
  
  if (defined($envdb{$label})) {
    print("-- Delete environment \"$label\" [n]? ");
    my($tmp);
    chomp($tmp = <STDIN>);

    if ($tmp =~ /^[yY]/) {
      delete $envdb{$label};
      printf("-- Environment \"%s\" has been deleted\n", $label);
    }

  } else {
    printf("++ Environment \"%s\" doesn't exists\n", $label);
  }

  untie(%envdb)|| die "++ can't untie $dbname!";
}

#
# help
#
sub help ()
{
  my($key);

  printf("++\n");
  printf("++ Usage: pwre verb [...]\n");
  printf("++ A verb name may be shortned as long as it isn't ambiguos, eg. pwre h[elp]\n");
  printf("++ Verbs:\n");

  foreach $key (sort keys (%verbs)) {
    ($verb, $usage)  =  split(/:/, $verbs{$key});
    if (!($verb =~ /^_/)) {
      &$usage();
    }
  }

  printf("++\n");
  exit 1;
}


#
# list()
#
sub list ()
{
  my($env);
  printf("Using Database:%s\n",$dbname);

  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";

  print("-- Defined environments:\n");
  foreach $env (sort keys (%envdb)) {
      $varstr = $envdb{$env};
      ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc)  =  split(/:/, $varstr);
      @vars = ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc);
    printf("   %s			%s\n", $env, $desc);
  }
  print("--\n");

  untie(%envdb)|| die "++ can't untie $dbname!";

}


#
# modify()
#
sub modify ()
{
  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";
  $label = $_[0];
  $varstr = $envdb{$label};
  if ($varstr ne "") {
    read_vars();
    get_vars();
    update_db();
  } else {
    printf("Environment %s doesn't exists\n", $label);
  }
  untie(%envdb)|| die "++ can't untie $dbname!";
}


#
# show
#
sub show ()
{

  if ($_[0] ne "") {
    $label = $_[0];
  } else {
    $label = $ENV{"pwre_env"};
    if (!defined($label)) {
      print("++ Environment is not initialized!\n");
      exit 1;
    }
  }

  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";

  read_vars();
  if ($varstr ne "") {
    print("--\n");
    printf("-- Environment    : %s\n", $label);
    show_vars();
    print("--\n");
  } else {
    printf("++ Environment %s doesn't exists\n", $label);
  }
  untie(%envdb)|| die "++ can't untie $dbname!";
}

#
# tags()
#
sub tags () 
{
  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

  printf("--\n");
  printf("-- Creating a CRiSP tag-file named ~/pwre/tags\n");
  system("pwr_tags") && exit;

}


#
# _build()
#
sub _build () # args: branch, subbranch, flavour, phase
{

  my($branch) = $_[0];
  if (!defined($branch)) {
    usage_build();
    exit 1;
  }

  my($subbranch) = $_[1];
  if (!defined($subbranch)) {
    usage_build();
    exit 1;
  }
  my($flavour) = $_[2];
  shift(@_);
  shift(@_);
  shift(@_);

  my($grepstr) = $ENV{"pwre_target"};
  
  my($globstr) = $ENV{"pwre_sroot"} . "/$branch";
  my(@dirs1) = glob($globstr);
  my($dir1);
  my(@dirs2);
  my($dir2);
  my(@mfiles);
  my($globstr1);
  my($globstr2);
  my($globstr3);

  foreach $dir1 (@dirs1) {

#    $globstr = "$dir1" . "/$subbranch/$flavour/os_$os/hw_$hw"; 
#    if ( ! -e $globstr ) {
#      $globstr = "$dir1" . "/$subbranch/$flavour/os_$os/.hw_$hw"; 
#      if ( ! -e $globstr ) {
#	$globstr = "$dir1" . "/$subbranch/$flavour/.os_$os/.hw_$hw"; 
#      }
#    }
    $globstr1 = "$dir1" . "/$subbranch/$flavour/os_$os/hw_$hw"; 
    $globstr2 = "$dir1" . "/$subbranch/$flavour/os_$os/.hw_$hw"; 
    $globstr3 = "$dir1" . "/$subbranch/$flavour/.os_$os/.hw_$hw"; 
    
    @dirs2 = (glob($globstr1),glob($globstr2),glob($globstr3));

    foreach $dir2 (@dirs2) {
      if (-d $dir2) {
        @mfiles = `find $dir2 -name makefile`;
        foreach (@mfiles) {
          chomp($_);
          $_ =~ s#/makefile##;
          print("\n$_\n");
          chdir($_) || die "cannot cd to $_ ($!)";
          $ENV{"PWD"} = $_;
          system("make @_") && exit 1;
        }
      }
    }  
  }
}

#
# merge()
#
sub merge () 
{
  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

  my($eroot) = $ENV{"pwre_broot"};

  if (!defined($eroot)) {
    printf("++\n++ No build root is defined\n");
    exit 1;
  }
  my($file) = $_[0];

#  if ($module eq "rt") {
#    printf("++\n++ No merge for module rt needed\n");
#    exit 1;
#  }
  $eroot .= "/" . $ENV{"pwre_os"};
  $eroot .= "/" . $ENV{"pwre_hw"};
  my($mroot) = $eroot;
  $mroot .= "/" . $ENV{"pwre_module"};
  $eroot .= "/exp";
  printf("\n");
  printf("-- Merge %s %s\n", $module, $file);

  my($cmd) = $ENV{pwre_bin} . "/pwre_merge.sh " . $mroot . " " . $eroot . " " . $file;
  system("$cmd");
}


#
# _exists()
#
sub _exists ()
{
  my($ret) = 1;
  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";

  $label = $_[0];
  
  if (defined($envdb{$label})) {
    $ret = 0;
  }

  untie(%envdb)|| die "++ can't untie $dbname!";
  exit $ret;
}

#
# _print()
#
sub _print ()
{

  use DB_File;
  tie(%envdb, "DB_File", $dbname, O_CREAT|O_RDWR, 0644) || die "++ can't tie $dbname!";

  $label = $_[0];
  read_vars();
  if ($varstr ne "") {
     foreach (@vars) {
       if ($_ eq "") {
	 printf("\"\"\n");
       } else {
         printf("%s\n", $_);
       }
     }
  } else {
    printf("++ Environment %s doesn't exists\n", $label);
  }
  untie(%envdb)|| die "++ can't untie $dbname!";
}

sub _module()
{
  my($modu) = $_[0];
  my($sroot) = $ENV{"pwre_sroot"};
  my($idx) = rindex($sroot,"/");
  $sroot = substr($sroot, 0, $idx);
  my($broot) = $ENV{"pwre_broot"} . "/" . $ENV{"pwre_os"} . "/" . $ENV{"pwre_hw"};

  $ENV{"pwre_module"} = $modu;
  $ENV{"pwre_bmodule"} = $modu;
  if ( $modu eq "rt") {
#    $ENV{"pwre_bmodule"} = "exp";
    $ENV{"pwre_sroot"} = $sroot . "/src";
  }
  else {
    $ENV{"pwre_sroot"} = $sroot . "/" . $modu;
  }
  $ENV{"pwr_exe"} = $broot . "/" . $ENV{"pwre_bmodule"} . "/exe";
  $ENV{"pwr_inc"} = $broot . "/" . $ENV{"pwre_bmodule"} . "/inc";
  $ENV{"pwr_load"} = $broot . "/" . $ENV{"pwre_bmodule"} . "/load";
  $ENV{"pwr_lib"} = $broot . "/" . $ENV{"pwre_bmodule"} . "/lib";
  $ENV{"pwr_lis"} = $broot . "/" . $ENV{"pwre_bmodule"} . "/lis";
  $ENV{"pwr_obj"} = $broot . "/" . $ENV{"pwre_bmodule"} . "/obj";
  $ENV{"pwr_doc"} = $broot . "/" . "exp" . "/doc";
}

#
# Misc. subroutines
# 
sub create_base()
{
  my($newdir) = $_[0];

  create_dir($newdir);
  create_dir($newdir . "/db");
  create_dir($newdir . "/exe");
  create_dir($newdir . "/exe/sv_se");
  create_dir($newdir . "/exe/en_us");
  create_dir($newdir . "/exe/de_de");
  create_dir($newdir . "/exe/fr_fr");
  create_dir($newdir . "/exe/zh_cn");
  create_dir($newdir . "/inc");
  create_dir($newdir . "/load");
  create_dir($newdir . "/lib");
  create_dir($newdir . "/lis");
  create_dir($newdir . "/obj");
  create_dir($newdir . "/src");
  create_dir($newdir . "/cnf");
  create_dir($newdir . "/doc");
  create_dir($newdir . "/doc/dox");
  create_dir($newdir . "/doc/prm");
  create_dir($newdir . "/doc/sv_se");
  create_dir($newdir . "/doc/en_us");
  create_dir($newdir . "/doc/sv_se/orm");
  create_dir($newdir . "/doc/en_us/orm");
  create_dir($newdir . "/doc/sv_se/dsh");
  create_dir($newdir . "/doc/en_us/dsh");
  create_dir($newdir . "/doc/help");
}

sub create_dir()
{
  my($dir) = $_[0];

  if (!chdir($dir)) {
    if (mkdir($dir, 0775)) {
#     printf("-- mkdir: %s\n", $dir);
    } else {
      printf("++\n++ Cannot mkdir %s, reason: %s\n", $dir, $!);
      exit 1;
    }   
  }
}


sub get_vars ()
{
    printf( "Arg: %s\n", $_[1]);
  if ($_[1] eq "") {
    $sroot = 	get_var(" Source root [%s]? ", $sroot);
  } else {
    $sroot = $_[1];
  }
  if ($_[2] eq "") {
    $vmsinc = 	get_var(" Import root  [%s]? ", $vmsinc);
  } else {
    $vmsinc = $_[2];
  }
  if ($_[3] eq "") {
    $broot = 	get_var(" Build root  [%s]? ", $broot);
  } else {
    $broot = $_[3];
  }
  if ($_[4] eq "") {
    $btype = 	get_var(" Build type  [%s]? ", $btype);
  } else {
    $btype = $_[4];
  }
  if ($_[5] eq "") {
    $os =    	get_var(" OS          [%s]? ", $os);
  } else {
    $os = $_[5];
  }
  if ($_[6] eq "") {
    $hw =    	get_var(" Hardware    [%s]? ", $hw);
  } else {
    $hw = $_[6];
  }
  if ($_[7] eq "") {
    $desc =  	get_var(" Description [%s]? ", $desc);
  } else {
    $desc = $_[7];
  }

  $varstr = join(";", ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc));

}


sub get_var()
{
  my($tmp);

  printf($_[0], $_[1]);
  chomp($tmp = <STDIN>);
  if ($tmp eq "") {
    $tmp = $_[1];
  }
  return $tmp;
  
}

sub read_vars ()
{
  $varstr = $envdb{$label};
  ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc)  =  split(/;/, $varstr);
  @vars = ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc);
}


sub show_vars ()
{

  my($module) = $ENV{"pwre_module"};

  printf("-- Module.........: %s\n", $module);
  printf("-- Source root....: %s\n", $sroot);
  if ( $vmsinc ne "") {
    printf("-- Import root....: %s\n", $vmsinc);
  }
  printf("-- Build root.....: %s\n", $broot);
  printf("-- Build type.....: %s\n", $btype);
  printf("-- OS.............: %s\n", $os);
  printf("-- Hardware.......: %s\n", $hw);
  printf("-- Description....: %s\n", $desc);
}

sub update_db ()
{
  $envdb{$label} = $varstr;
}

sub usage_add ()
{
  printf("++\n");
  printf("++ add env                       : Adds an environment to the database\n");
}

sub usage_configure ()
{
  printf("++\n");
  printf("++ configure                     : Configures an environment\n");
}

sub usage_build ()
{
  printf("++\n");
  printf("++ build 'branch' 'subbranch' ['flavour']['phase']: Build, eg. pwre build exe rt* src all\n");
}

sub usage_build_all ()
{
  printf("++\n");
  printf("++ build_all                     : Builds all in current module\n");
}

sub usage_build_kernel ()
{
  printf("++\n");
  printf("++ build_kernel                  : Builds all in kernel modules (kernel, xtt, wb)\n");
}

sub usage_build_all_modules ()
{
  printf("++\n");
  printf("++ build_all_modules             : Builds all in all modules\n");
}

sub usage_build_all_wbl ()
{
  printf("++\n");
  printf("++ build_all_wbl                 : Builds wbl in all modules\n");
}

sub usage_ebuild ()
{
  printf("++\n");
  printf("++ ebuild 'block' 'flavour'       : Builds rt or op block\n");
}

sub usage_clean_exe_all ()
{
  printf("++\n");
  printf("++ clean_exe_all                 : Cleans all exe in all modules\n");
}

sub usage_method_build ()
{
  printf("++\n");
  printf("++ method_build                  : Rebuild method dependent programs\n");
}

sub usage_create_all_modules ()
{
  printf("++\n");
  printf("++ create_all_modules            : Create build trees for all modules\n");
}

sub usage_copy ()
{
  printf("++\n");
  printf("++ copy                          : Copy Proview include files from the VMS host\n");
}

sub usage_create ()
{
  printf("++\n");
  printf("++ create                        : Creates the build directory tree\n");
}

sub usage_dele ()
{
  printf("++\n");
  printf("++ delete 'env'                  : Deletes an environment from the database\n");
}

sub usage_help ()
{
  printf("++\n");
  printf("++ help                          : Lists all command verbs\n");
}

sub usage_list ()
{
  printf("++\n");
  printf("++ list                          : Lists all environments in the database\n");
}

sub usage_init ()
{
  printf("++\n");
  printf("++ init 'env'                    : Inits an environment\n");
}

sub usage_import ()
{
  printf("++\n");
  printf("++ import 'block' ['flavour']    : Import files from import root, block dbs, rt, op, java, doc\n");
}

sub usage_module ()
{
  printf("++\n");
  printf("++ module 'module'               : Set module\n");
}

sub usage_merge ()
{
  printf("++\n");
  printf("++ merge                         : Merge module base to exp base\n");
}

sub usage_modify ()
{
  printf("++\n");
  printf("++ modify 'env'                  : Modfies an existing environment\n");
}


sub usage_show ()
{
  printf("++\n");
  printf("++ show [env]                    : Shows current or given environment\n");
}

sub usage_tags ()
{
  printf("++\n");
  printf("++ tags                          : Creates a CRiSP tag-file named ~/pwre/tags\n");
}


