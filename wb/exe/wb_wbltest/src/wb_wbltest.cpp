
#include "pwr.h"
#include "wb_erep.h"
#include "wb_env.h"
#include "wb_vrepdbs.h"
#include "wb_vrepdb.h"
#include "wb_vrep.h"
#include "wb_dbs.h"
#include "wb_db.h"
#include "wb_vrepwbl.h"
#include "wb_vrepdbs.h"
#include "wb_orepdbs.h"
#include "wb_orepwbl.h"
#include "wb_volume.h"
#include "wb_session.h"
#include "wb_error.h"
#include "co_dbs.h"
#include "co_time.h"

#if 1
int main( int argc, char *argv[])
{
  pwr_tStatus sts;
  //dbs_sEnv env;
  //dbs_sEnv *ep;
  wb_erep *erep = new wb_erep();

  if (argc <= 2) exit(0);
  //  ep = dbs_Map(&sts, &env, argv[1]);
  wb_vrepdbs *vdbs = new wb_vrepdbs(erep, argv[1]);
  vdbs->load();

  // wb_dbs dbs(vdbs);
  // dbs.setFileName("lasse.dbs");
  // dbs.importVolume(*vdbs);
  
  //wb_orepdbs *op = (wb_orepdbs *)vdbs->object(&sts);

  wb_db db(pwr_cNVid);
  
  db.copy(*vdbs, argv[2]);
  //db.importVolume(*vdbs);
  db.close();

  wb_vrepdb vdb(erep, argv[2]);
  wb_name n("VHX-NU4-KOM-EVELINA-Transar-W-cn254");
  wb_orepdb *op = (wb_orepdb *)vdb.object(&sts, n);
  
  wb_dbs adbs(vdbs);
  adbs.setFileName("alasse.dbs");
  adbs.importVolume(vdb);
}

#elif 0
int main( int argc, char *argv[])
{
  //pwr_tStatus sts;
  //dbs_sEnv env;
  //dbs_sEnv *ep;
  wb_erep *erep = new wb_erep();

  if (argc <= 2) exit(0);

  //wb_vrepdb vdb(erep, argv[2]);
  wb_vrepdb vdb(erep, 1000, pwr_eClass_RootVolume, "Lasse", argv[2]);
  
}

#elif 0
int main( int argc, char *argv[])
{
  pwr_tStatus sts;
  //dbs_sEnv env;
  //dbs_sEnv *ep;
  wb_erep *erep = new wb_erep();

  if (argc <= 2) exit(0);
  //  ep = dbs_Map(&sts, &env, argv[1]);
  wb_vrepdbs *vdbs = new wb_vrepdbs(erep, argv[1]);
  vdbs->load();

  wb_dbs dbs(vdbs);
  dbs.setFileName("lasse.dbs");
  dbs.importVolume(*vdbs);
  
  //wb_orepdbs *op = (wb_orepdbs *)vdbs->object(&sts);

  wb_db db(vdbs->vid());
  db.create(vdbs->vid(), vdbs->cid(), vdbs->name(), argv[2]);
  db.importVolume(*vdbs);
  db.close();

  wb_vrepdb vdb(erep, argv[2]);
  
  wb_dbs adbs(vdbs);
  adbs.setFileName("alasse.dbs");
  adbs.importVolume(vdb);
  
}

#else
static pwr_tStatus iterFunc( void *udata, 
			     pwr_tOid oid, pwr_tCid cid, pwr_tOid poid, pwr_tOid fwsoid,
			     pwr_tOid bswoid, pwr_tOid fchoid, pwr_tOid lchoid, char *name,
			     pwr_tTime time, int rsize, int dsize);

int main( int argc, char *argv[])
{
  char fname[200];
  pwr_tStatus sts;

  if ( argc <= 1) exit(0);
  strcpy( fname, argv[1]);

  try {
    // Create wb env
    wb_erep *erep = new wb_erep();

    wb_volume v2(erep->volume(&sts));
    while ( v2.oddSts()) {
      cout << "Volume loaded: " << v2.vid() << endl;
      v2 = v2.next();
    }

    wb_env *env = new wb_env(erep);

    // Load pwrs
    wb_vrepwbl *wbl = new wb_vrepwbl(erep);
    wbl->load( (char *)fname);
    // wbl->info();

    // Add pwrs to env
    erep->addExtern( &sts, wbl);
    cout << "addExtern: " << sts << endl;

    // Get pwrs
    // wb_volume vol = env->externVolume( 1);
    wb_volume vol = wb_volume(wbl); // Test

    cout << "Get pwrs: " << env->sts() << endl;

    // wb_volume *vol = new wb_volume( wbl);
    wb_session *ses = new wb_session( vol);
    // ses->access( ldh_eAccess_ReadWrite);
    // ses->utility( ldh_eUtility_Configurator);

    wb_object ob = ses->object();
    wb_object ob2 = ob.first();
    wb_object ob3 = ob2.after();
    wb_object ob4 = ob.last();
    wb_object ob5 = ob4.before();
    wb_object ob6 = ob4.parent();
    cout << "ansc: " << ob.name() << "  " << ob.what() << endl;
    cout << "first: " << ob2.name() << endl;
    cout << "after: " << ob3.name() << endl;
    cout << "last: " << ob4.name() << endl;
    cout << "before: " << ob5.name() << endl;
    cout << "parent: " << ob6.name() << endl;

    wb_orep *o = wbl->object( &sts);
    wb_orep *o2 = o->first( &sts);
    wb_orep *o3 = o2->after( &sts);
    wb_orep *o4 = o->last( &sts);
    wb_orep *o5 = o4->before( &sts);
    wb_orep *o6 = o4->parent( &sts);
    cout << "ansc: " << o->name() << endl;
    cout << "first: " << o2->name() << endl;
    cout << "after: " << o3->name() << endl;
    cout << "last: " << o4->name() << endl;
    cout << "before: " << o5->name() << endl;
    cout << "parent: " << o6->name() << endl;

    // Print the resulting tree
    // cout << wbl->rootAST[0]->toStringTree() << endl;

    //wbl->iterObject( (void *)22, iterFunc);

    erep->removeExtern( &sts, wbl);
    delete ses;
    // delete wbl;
  }
  catch ( wb_error &e) {
    cout << "Exception: " << e.what() << endl;
  }
}

static pwr_tStatus iterFunc( void *udata, 
			     pwr_tOid oid, pwr_tCid cid, pwr_tOid poid, pwr_tOid fwsoid,
			     pwr_tOid bswoid, pwr_tOid fchoid, pwr_tOid lchoid, char *name,
			     pwr_tTime time, int rsize, int dsize)
{
  char timstr[40];
  time_AtoAscii( &time, time_eFormat_DateAndTime, timstr, sizeof(timstr));
  if ( cid == pwr_eClass_ClassDef) {
    // cout << "iterFunc : " << oid.oix << " " << name << " " << timstr << endl;
  }
  return 1;
}



#endif
