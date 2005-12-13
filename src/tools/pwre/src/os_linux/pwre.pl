#!/usr/bin/perl
#
# pwre.pl
#

%verbs = (
  "add", 	"add:usage_add",
  "build", 	"build:usage_build",
  "build_all", 	"build_all:usage_build_all",
  "build_ssab", "build_ssab:usage_build_ssab",
  "copy", 	"copy:usage_copy",
  "create", 	"create:usage_create",
  "delete", 	"dele:usage_dele",
  "help",	"help:usage_help",
  "list",	"list:usage_list",
  "init",	"init:usage_init",
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
# build_all()
#
sub build_all ()
{


  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

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

  
  _build("exp", "inc", "all");
  _build("exp", "com", "all");
  _build("tools/exe", "*", "all");
  _build("msg", "*", "all");
  _build("lib", "rt", "init copy");
  _build("lib", "wb", "init copy");
  _build("lib", "co", "init copy");
  _build("lib", "dtt", "init copy");
  _build("exp", "wb", "init copy");
  _build("lib", "flow", "all");
  _build("lib", "glow", "all");
  _build("lib", "co", "all");
  _build("exe", "co*", "all");
  _build("wbl", "pwrs", "copy");
  _build("wbl", "pwrb", "copy");
  _build("wbl", "nmps", "copy");
  _build("wbl", "ssab", "copy");
  _build("wbl", "tlog", "copy");
  _build("wbl", "bcomp", "copy");
  _build("wbl", "mcomp", "copy");
  _build("lib", "ge", "all");
  _build("lib", "wb", "all");
  _build("lib", "rt", "all");
  _build("lib", "rs", "all");
  _build("exp", "rt", "all");
  _build("exp", "wb", "all");
  _build("lib", "msg_dummy", "all");
  _build("exe", "wb*", "all");
  _build("lib", "dtt", "all");
  _build("exp", "rt", "all");
  _build("exe", "rt*", "all");
  _build("exe", "rs*", "all");
  _build("exe", "jpwr*", "all");
  _build("exp", "ge", "all");
  _build("mmi", "*", "copy");
  _build("jpwr", "rt", "all");
  _build("jpwr", "jop", "all");
  _build("jpwr", "jopc", "all");
  _build("jpwr", "beans", "all");
  _build("jpwr", "rt_client", "all");
#  _build("db", "wb", "init");
  _build("wbl", "pwrs", "lib");
  _build("wbl", "pwrb", "lib");
  _build("wbl", "nmps", "lib");
  _build("wbl", "ssab", "lib");
  _build("wbl", "tlog", "lib");
  _build("wbl", "bcomp", "lib");
  _build("wbl", "mcomp", "lib");
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
  _build("wbl", "nmps", "exe");
  _build("wbl", "tlog", "exe");
  _build("wbl", "ssab", "exe");
  _build("wbl", "bcomp", "exe");
  _build("wbl", "mcomp", "exe");

}


#
# build_ssab()
#
sub build_ssab ()
{


  if (!defined($ENV{"pwre_env"})) {
    print("++ Environment is not initialized!\n");
    exit 1;
  }

  printf("--\n");
  printf("-- Build ssab\n");
  show();

#  my($tmp);
#  print("-- Copy include files from VMS [n]? ");
#  $tmp = <STDIN>;
#  chomp($tmp);

#  if ($tmp =~ /^[yY]/) {
#    copy();
#  }

  
  _build("wbl", "tlog", "copy");
  _build("msg", "rs", "all");
  _build("lib", "rs", "all");
  _build("exe", "rs*", "all");
  _build("wbl", "nmps", "lib");
  _build("wbl", "tlog", "lib");
  _build("wbl", "ssab", "lib");
  _build("wbl", "nmps", "exe");
  _build("wbl", "tlog", "exe");
  _build("wbl", "ssab", "exe");

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

  $newdir = $root . "/exp";
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
    printf("   %s\n", $env);
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


#
# Misc. subroutines
# 
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
  $vmsinc = 	get_var(" pwr_inc on VMS  [%s]? ", $vmsinc);
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

  printf("-- Source root....: %s\n", $sroot);
  printf("-- pwr_inc on VMS.: %s\n", $vmsinc);
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
  printf("++ build branch subbranch [phase]: Build, eg.  pwre build exe rt* all\n");
}

sub usage_build_all ()
{
  printf("++\n");
  printf("++ build_all                     : Builds all\n");
}

sub usage_build_ssab ()
{
  printf("++\n");
  printf("++ build_ssab                    : Builds additional components\n");
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
  printf("++ delete env                    : Deletes an environment from the database\n");
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
  printf("++ init env                      : Inits an environment\n");
}

sub usage_modify ()
{
  printf("++\n");
  printf("++ modify env                    : Modfies an existing environment\n");
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


