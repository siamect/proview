#!/usr/bin/perl
#
# pwre.pl
#

%verbs = (
  "add", 	"add:usage_add",
  "build", 	"build:usage_build",
  "build_all", 	"build_all:usage_build_all",
  "build_kernel", "build_kernel:usage_build_kernel",
  "build_all_modules", "build_all_modules:usage_build_all_modules",
  "build_all_wbl", "build_all_wbl:usage_build_all_wbl",
  "clean_exe_all", "clean_exe_all:usage_clean_exe_all",
  "method_build", "method_build:usage_method_build",
  "copy", 	"copy:usage_copy",
  "create", 	"create:usage_create",
  "create_all_modules", "create_all_modules:usage_create_all_modules",
  "delete", 	"dele:usage_dele",
  "help",	"help:usage_help",
  "list",	"list:usage_list",
  "init",	"init:usage_init",
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
if ($ENV{"HOSTTYPE"} eq "rs6000") {
  $hw	= "ppc";
} else {
  $hw	= "x86";
}
$desc	= $user. "'s environment";
@vars;
$varstr;


if (($dbname = $ENV{"pwre_env_db"}) eq "") {
  $dbname = $ENV{"HOME"} . "/pwre/pwre";
}


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
    printf("++ Envrinoment %s already exists\n", $_[0]);
  } else {
    get_vars();  
    update_db();
  }
  untie(%envdb)|| die "++ can't untie $dbname!";

}


#
# build()
#
sub build () # args: branch, subbranch, phase
{

  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

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

  printf("--\n");
  printf("-- Build\n");
  printf("-- Branch..........: %s\n", $branch);
  printf("-- Sub branch......: %s\n", $subbranch);
  printf("-- Args............: ");
  foreach (@_) {
    printf("%s ", $_);
  }
  printf("\n--\n");
  
  _build($branch, $subbranch, @_);

}

#
# build_kernel()
#
sub build_kernel ()
{
  _module("xtt");
  build_all("copy");
  merge();
  _module("rt");
  build_all("copy");
  merge();
  _module("wb");
  build_all("copy");
  merge();
  _module("rt");
  build_all("lib");
  merge();
  _module("xtt");
  build_all("lib");
  merge();
  _module("wb");
  build_all("lib");
  merge();
  _module("xtt");
  build_all("exe");
  merge();
  _module("wb");
  build_all("exe");
  merge();
  _module("rt");
  build_all("exe");
  merge();
}

#
# build_all()
#
sub build_all_modules ()
{
  build_kernel();
  _module("nmps");
  build_all();
  merge();
  _module("tlog");
  build_all();
#  merge();
#  _module("remote");
#  build_all();
  merge();
  _module("bcomp");
  build_all();
#  merge();
#  _module("java");
#  build_all();
  merge();
  _module("profibus");
  build_all();
  merge();
  _module("ssabox");
  build_all();
  merge();
  _module("othermanu");
  build_all();
  merge();
  _module("abb");
  build_all();
  merge();
  _module("siemens");
  build_all();
  merge();
  _module("inor");
  build_all();
  merge();
  _module("klocknermoeller");
  build_all();
  merge();
  _module("telemecanique");
  build_all();
  merge();

  method_build();
}  

#
# clean_exe_all()
#
sub clean_exe_all ()
{
  _module("xtt");
  _build("exe","*","clean_exe");
  _module("rt");
  _build("exe","rt*","clean_exe");
  _build("exe","co*","clean_exe");
  _build("exe","wb*","clean_exe");
  _module("wb");
  _build("exe","*","clean_exe");
  _module("nmps");
  _build("exe","*","clean_exe");
  _module("tlog");
  _build("exe","*","clean_exe");
  _module("remote");
  _build("exe","*","clean_exe");
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
  _build("wbl", "pwrs", "lib");
  _build("wbl", "pwrb", "lib");
  _build("wbl", "wb", "lib");
  _build("wbl", "rt", "lib");
  merge();
  _module("nmps");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "nmps", "lib");
  merge();
  _module("tlog");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "tlog", "lib");
  merge();
  _module("profibus");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("remote");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "remote", "lib");
  merge();
  _module("bcomp");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "bcomp", "lib");
  merge();
  _module("ssabox");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("abb");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("siemens");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("telemecanique");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("inor");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("klocknermoeller");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("othermanu");
  my($load_dir) = $ENV{"pwr_load"};
  system( "rm $load_dir/*.dbs");
  _build("wbl", "mcomp", "lib");
  merge();
  _module("rt");
}  

sub method_build ()
{
  my($program) = $_[0];
  if (!defined($program)) {
    printf("-- Relink method dependent programs");
    _module("rt");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/rt_io_comm");
    _build("exe", "rt_io_comm", "all");
#   system("rm $exe_dir/rt_ini");
#   _build("exe", "rt_ini", "all");
    merge();
  
    _module("wb");
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/wb");
    _build("exe", "wb", "all");
    merge();
  
    _module("xtt");
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/rt_xtt");
    _build("exe", "rt_xtt", "all");
    merge();
  }  
  if ( $_[0] eq "rt_io_comm" ) {
    printf("-- Method build $program");
    _module("rt");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/rt_io_comm");
    _build("exe", "rt_io_comm", "all");
    merge();
  }
  if ( $_[0] eq "wb" ) {
    printf("-- Method build $program");
    _module("wb");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/wb");
    _build("exe", "wb", "all");
    merge();
  }  
  if ( $_[0] eq "rt_xtt" ) {
    printf("-- Method build $program");
    _module("xtt");
    $ENV{"export_type"} = "exp";
    my($exe_dir) = $ENV{"pwr_exe"};
    system("rm $exe_dir/rt_xtt");
    _build("exe", "rt_xtt", "all");
    merge();
  }  
}

sub create_all_modules ()
{
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
  _module("profibus");
  create();
  _module("ssabox");
  create();
  _module("othermanu");
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
  if ( $_[0] eq "") {
    $copy = 1;
    $lib = 1;
    $exe = 1;
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

#  my($tmp);
#  print("-- Copy include files from VMS [n]? ");
#  $tmp = <STDIN>;
#  chomp($tmp);

#  if ($tmp =~ /^[yY]/) {
#    copy();
#  }

  
  if ( $module eq "rt") {
    if ( $copy == 1) {
      _build("exp", "inc", "all");
      _build("exp", "com", "all");
      _build("tools/exe", "*", "all");
      merge("exe/tools_cmsg2c");
      merge("exe/tools_msg2cmsg");
      merge("exe/tools_pdrgen");
      _build("msg", "*", "all");
      _build("lib", "rt", "init copy");
      _build("lib", "co", "init copy");
      _build("lib", "dtt", "init copy");
      _build("lib", "co", "all");
      _build("exe", "co*", "all");
      _build("wbl", "pwrs", "copy");
      _build("wbl", "pwrb", "copy");
      _build("exe", "*", "copy");
    }

    if ( $lib == 1) {
      _build("lib", "ge", "all");
      if ( ! -e $nmpsclasses ) {
        _module("nmps");
        _build("wbl", "nmps", "init copy");
        merge("inc/pwr_nmpsclasses.h");
      }
      if ( ! -e $remoteclasses ) {
        _module("remote");
        _build("wbl", "remote", "init copy");
        merge("inc/pwr_remoteclasses.h");
      }
      _module("rt");
      _build("lib", "rt", "all");
      _build("exp", "rt", "all");
      _build("lib", "msg_dummy", "all");
      _build("exe", "wb_rtt", "all");
      _build("lib", "dtt", "all");
      _build("exp", "rt", "all");
    }
    if ( $exe == 1) {
      _build("exe", "rt*", "all");
      _build("exe", "pwr_user", "all");
      _build("exp", "ge", "all");
      _build("mmi", "*", "copy");
      _build("wbl", "pwrs", "lib");
      _build("wbl", "pwrb", "lib");
      _build("wbl", "wb", "lib");
      _build("wbl", "rt", "lib");
      _build("doc", "web", "all");
      _build("doc", "dweb", "all");
      _build("doc", "orm", "all");
      _build("doc", "prm", "all");
      _build("doc", "man", "all");
      _build("doc", "dox", "all");
      _build("wbl", "pwrs", "exe");
      _build("wbl", "pwrb", "exe");
    }
  }
  elsif ( $module eq "java") {
    if ( $lib == 1) {
      _build("jpwr", "rt", "all");
      _build("jpwr", "jop", "all");
      _build("jpwr", "jopc", "all");
      _build("jpwr", "beans", "all");
      _build("jpwr", "rt_client", "all");
    }
  }
  else {
      printf( "Copy: %d Lib: %d Exe: %d\n", $copy, $lib, $exe);
    if ( $copy == 1) {
      _build("wbl", "*", "copy");
      _build("lib", "*", "init copy");
      _build("exp", "*", "init copy");
      _build("mmi", "*", "copy");
      _build("exe", "*", "copy");
    }
    if ( $lib == 1) {
      _build("lib", "*", "lib");
      _build("exp", "*", "lib");
    }
    if ( $exe == 1) {
      _build("wbl", "*", "lib");
      _build("wbl", "*", "exe");
      _build("exe", "*", "all");
      _build("doc", "dsh", "copy");
      _build("doc", "orm", "copy");
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
  my($bindir) = $ENV{"pwre_bin"};

  printf("--\n");
  printf("-- Copy include files from VMS\n");
  printf("-- Include directory on VMS: %s\n", $vmsinc);
  printf("--\n");
  
  system("make -f $bindir/copy_vms_inc.mk") && exit;

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
sub _build () # args: branch, subbranch, phase
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
  shift(@_);
  shift(@_);

  my($grepstr) = $ENV{"pwre_target"};
  
  my($globstr) = $ENV{"pwre_sroot"} . "/$branch";
  my(@dirs1) = glob($globstr);
  my($dir1);
  my(@dirs2);
  my($dir2);
  my(@mfiles);

  foreach $dir1 (@dirs1) {
    $globstr = "$dir1" . "/$subbranch/src/os_$os/hw_$hw"; 
    @dirs2 = glob($globstr);
    foreach $dir2 (@dirs2) {
      if (-d $dir2) {
        @mfiles = `find $dir2 -name makefile| grep $grepstr`;
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
  printf("--\n");
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
      printf("-- mkdir: %s\n", $dir);
    } else {
      printf("++\n++ Cannot mkdir %s, reason: %s\n", $dir, $!);
      exit 1;
    }   
  }
}


sub get_vars ()
{

  $sroot = 	get_var(" Source root [%s]? ", $sroot);
# $vmsinc = 	get_var(" pwr_inc on VMS  [%s]? ", $vmsinc);
  $broot = 	get_var(" Build root  [%s]? ", $broot);
  $btype = 	get_var(" Build type  [%s]? ", $btype);
  $os =    	get_var(" OS          [%s]? ", $os);
  $hw =    	get_var(" Hardware    [%s]? ", $hw);
  $desc =  	get_var(" Description [%s]? ", $desc);

  $varstr = join(":", ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc));

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
  ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc)  =  split(/:/, $varstr);
  @vars = ($sroot, $vmsinc, $broot, $btype, $os, $hw, $desc);
}


sub show_vars ()
{

  my($module) = $ENV{"pwre_module"};

  printf("-- Module.........: %s\n", $module);
  printf("-- Source root....: %s\n", $sroot);
# printf("-- pwr_inc on VMS.: %s\n", $vmsinc);
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

sub usage_build ()
{
  printf("++\n");
  printf("++ build 'branch' 'subbranch' ['phase']: Build, eg. pwre build exe rt* all\n");
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

sub usage_clean_exe_all ()
{
  printf("++\n");
  printf("++ clean_exe_all             : Cleans all exe in all modules\n");
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


