#include "wb_vrepdb.h"
#include "wb_orepdb.h"
#include "db_cxx.h"

void wb_vrepdb::unref()
{
    if (--m_nRef == 0)
        delete this;
}

wb_vrep *wb_vrepdb::ref()
{
    m_nRef++;
    return this;
}

/*
typedef struct {
    pwr_tOid      oid;
    pwr_tCid      cid;
    pwr_tObjName  name;     //
    pwr_tObjName  normname; //
    pwr_tOid      poid;
    pwr_tOid      bwsoid;  // previous sibling
    pwr_tOid      fwsoid;  // next sibling
    pwr_tOid      fcoid;   // first child
    pwr_tOid      lcoid;   // last child
    pwr_tTime     time;
    pwr_tTime     b_time[2];
    pwr_tUInt32   b_siz[2];
    pwr_mClassDef flags;
} ldh_sDbOdata;

typedef struct {
    pwr_tOix     key;
    ldh_sDbOdata data;
} ldh_sDbObject;


typedef struct {
    struct {
        pwr_tOix oix;
        pwr_tBix bix;
    } key;
    void *data;
} ldh_sDbBody;

typedef struct {
    struct {
        pwr_tOix     poix;
        pwr_tObjName normname;
    } key;
    struct {
        pwr_tOix      oix;
        pwr_tCid      cid;   // saved here to optimize tree traversal
        pwr_mClassDef flags; // saved here to optimize tree traversal
    } data;
} ldh_sDbName;

typedef struct 
{
    pwr_tCid cid;
    pwr_tOix oix;
} ldh_sDbClassList;


*/


wb_vrepdb::oh_k::oh_k()
{
    m_oix = 0;
    set_data(&m_oix);
    set_size(sizeof(m_oix));
}

wb_vrepdb::oh_k::oh_k(pwr_tOix oix)
{
    m_oix = oix;
    set_data(&m_oix);
    set_size(sizeof(m_oix));
}

wb_vrepdb::oh_k::oh_k(wb_orepdb *o)
{
    m_oix = o->oix();
    set_data(&m_oix);
    set_size(sizeof(m_oix));
}

wb_vrepdb::oh_d::oh_d()
{
    m_orep = new wb_orepdb();
    set_data(m_orep);
    set_size(sizeof(*m_orep));
}

wb_vrepdb::oh_d::oh_d(wb_orepdb *o)
{
    m_orep = o;
    set_data(m_orep);
    set_size(sizeof(*m_orep));
}

wb_vrepdb::ob_k::ob_k()
{
    m_data.oix = 0;
    m_data.bix = 0;
    
    set_data(&m_data);
    set_size(sizeof(m_data));
}

wb_vrepdb::ob_k::ob_k(pwr_tOix oix)
{
    m_data.oix = oix;
    m_data.bix = 0;
    
    set_data(&m_data);
    set_size(sizeof(m_data));
}

wb_vrepdb::ob_k::ob_k(wb_orepdb *o)
{
    m_data.oix = o->oix();
    m_data.bix = 0;
    
    set_data(&m_data);
    set_size(sizeof(m_data));
}

wb_vrepdb::ob_k::ob_k(wb_orepdb *o, pwr_tOix bix)
{
    m_data.oix = o->oix();
    m_data.bix = bix;
    
    set_data(&m_data);
    set_size(sizeof(m_data));
}

wb_vrepdb::na_k::na_k()
{
    memset(&m_key, 0, sizeof(m_key));
    
    set_data(&m_key);
    set_size(sizeof(m_key));
}

wb_vrepdb::na_k::na_k(wb_orepdb *o)
{
    memset(&m_key, 0, sizeof(m_key));
    
    set_data(&m_key);
    set_size(sizeof(m_key));
}

wb_vrepdb::na_k::na_k(wb_orepdb *o, char *name)
{
    memset(&m_key, 0, sizeof(m_key));
    m_key.poix = o->oix();
    strcpy(m_key.name, name);
    
    set_data(&m_key);
    set_size(sizeof(m_key));
}

wb_vrepdb::na_k::na_k(pwr_tOix poix)
{
    memset(&m_key, 0, sizeof(m_key));
    m_key.poix = poix;
    
    set_data(&m_key);
    set_size(sizeof(m_key));
}

wb_vrepdb::na_k::na_k(pwr_tOix poix, char *name)
{
    memset(&m_key, 0, sizeof(m_key));
    m_key.poix = poix;
    strcpy(m_key.name, name);
    
    set_data(&m_key);
    set_size(sizeof(m_key));
}




/* Table db.name
   Key:  poix, name
   Data: oix
*/
typedef struct {
    struct {
        pwr_tOix     poix;
        pwr_tObjName normname;
    } key;
    struct {
        pwr_tOix     oix;
    } data;
} sName;



/* Table db.clist
   Key:  cid, oix
   Data: (null)
*/

typedef struct {
    pwr_tCid cid;
    pwr_tOix oix;
} sClist;


/* Table db.ohead
   Key:  oix
   Data: wb_DbObject
*/


/* Table db.obody
   Key:  oix, bix
   Data: body
*/



/* Table db.info
   Key:  idx
   Data: (varying)
         1 => next free oix
*/

/*
static add_object()
{
}*/

/*
void
add_ohead(DB_ENV *dbenv, DB *db, char *fruit, char *name)
{
    DBT key, data;
    DbTxn *txn = 0;
    int ret;

    // Initialization.
    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));
    key.data = fruit;
    key.size = strlen(fruit) + 1;
    data.data = name;
    data.size = strlen(name) + 1;
    
    for (;;) {
        // Begin the transaction.
        if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
            err(ret, "txn_begin");
            exit(1);
        }
        
        //printf("tid: %d\n", tid->txnid);

        
        switch (ret = db->put(db, tid, &key, &data, 0)) {
        case 0:
            // Success: commit the change.
            if ((ret = txn_commit(tid, 0)) != 0) {
                dbenv->err(dbenv, ret, "txn_commit");
                exit(1);
            }
            return;
        case DB_LOCK_DEADLOCK:
            // Deadlock: retry the operation.
            if ((ret = txn_abort(tid)) != 0) {
                dbenv->err(dbenv, ret, "txn_abort");
                exit(1);
            }
            break;
        default:
            // Error: run recovery.
            dbenv->err(dbenv, ret, "dbc->put: %s/%s", fruit, name);
            exit(1);
        }
    }

    oix = next_oix(txn);
    
    wb_DbName name;
    name.key.poix = ?;
    name.key.normname = ?;
    Dbt key(&name.key, sizeof(name.key));
    name.data.oix = oix;
    Dbt data(&name.data, sizeof(name.data));
    ret = m_db.name.put(tid, &key, &data, 0);

    wb_DbClist clist
    clist.cid = ?;
    clist.oix = ?;
    Dbt key(&clist, sizeof(clist));
    ret = m_db.clist.put(tid, &key, 0, 0);
    
    for (;;) {
        wb_DbObody body;
        body.key.oix = ?;
        body.key.bix = ?;
        Dbt key(&body.key, sizeof(body.key));
        body.data.oix = oix;
        Dbt data(&body.data, size);
        ret = m_db.obody.put(tid, &key, &data, 0);
    }
    
}
*/
#if 0
int wb_vrepdb::put_ohead(DbTxn *txn, wb_orepdb *o, u_int32_t flags)
{
    oh_k key(o);
    oh_d data(o);
    
    return m_db_ohead->put(txn, &key, &data, flags);
}

int wb_vrepdb::get_ohead(DbTxn *txn, wb_orepdb *o)
{
    oh_k key(o);
    oh_d data(o);
    
    return m_db_ohead->get(txn, &key, &data, 0);
}

int wb_vrepdb::get_ohead(wb_orep **orep, DbTxn *txn, pwr_tOid oid)
{
    wb_orepdb *o = new wb_orepdb(oid);
    
    oh_k key(o);
    oh_d data(o);
    
    int sts = m_db_ohead->get(txn, &key, &data, 0);
    *orep = o;

    return sts;
}

int wb_vrepdb::del_ohead(DbTxn *txn, wb_orepdb *o)
{
    oh_k  key(o);
    
    return m_db_ohead->del(txn, &key, 0);
}


int wb_vrepdb::get_clist(DbTxn *txn, wb_orepdb *o)
{
    int ret;
    pwr_tOix oix = o->oix();

    Dbt key(&oix, sizeof(oix));
    Dbt data(&oix, sizeof(oix));
    
    ret = m_db_clist->get(txn, &key, &data, 0);

    return ret;
}

int wb_vrepdb::put_clist(DbTxn *txn, wb_orepdb *o, u_int32_t flags)
{
    int ret;
    pwr_tOix oix = o->oix();

    Dbt key(&oix, sizeof(oix));
    
    ret = m_db_clist->put(txn, &key, 0, flags);

    return ret;
}

int wb_vrepdb::del_clist(DbTxn *txn, wb_orepdb *o)
{
    int ret;
    pwr_tOix oix = o->oix();

    Dbt key(&oix, sizeof(oix));
    
    ret = m_db_clist->del(txn, &key, 0);

    return ret;
}

int wb_vrepdb::get_ohead(wb_orep **orep, DbTxn *txn, wb_orepdb *o, char *name)
{
    int ret;

    na_k key(o, name);
    na_d data(o);

    ret = m_db_name->get(txn, &key, &data, 0);

    return ret;
}

int wb_vrepdb::get_name(DbTxn *txn, pwr_tOix poix, char *name, pwr_tOix *oix)
{
    int ret;
    
    na_k key(poix, name);
    na_d data();

    ret = m_db_name->get(txn, &key, &data, 0);

    *oix = data.oix();
    
    return ret;
}

int wb_vrepdb::put_name(DbTxn *txn, wb_orepdb *o, u_int32_t flags)
{
    na_k key(o);
    na_d data(o);
    
    ret = m_db_name->put(txn, &key, &data, flags);

    return ret;
}

int wb_vrepdb::del_name(DbTxn *txn, wb_orepdb *o)
{
    na_k key(o);
    
    return m_db_name->del(txn, &key, 0);
}

pwr_tOix wb_vrepdb::new_oix(DbTxn *txn)
{
    int index = 1;
    int ret;
    Dbt key(&index, sizeof(index));
    pwr_tOix oix;
    Dbt data(&oix, sizeof(oix));
    
    ret = m_info->get(txn, &key, &data, 0);
    oix++;
    ret = m_info->put(txn, &key, &data, 0);

    return oix;
}

int wb_vrepdb::del_family(DbTxn *txn, Dbc *cp, pwr_tOix poix)
{
    int ret;
    dbName name;
    name.poix = poix;
    name.normname = "";
    pwr_tOix oix = 0;
    
    FamilyKey  nk(po, );
    FamiltData nd(&oix, sizeof(oix));    
    Dbc *ncp;
    cp->dup(*ncp, 0);

    while ((ret = cp->get(&nk, &nd, DB_NEXT)) != DB_NOTFOUND) {
        del_family(txn, ncp, oix);
        cp->del(0);
        oh_k ok(nd);
        oh_d od();
        m_db_ohead->get(txn, &ok, &od, 0);
        wb_DbClistK ck(od);
        m_db_clist->del(txn, &ck, 0);
        wb_DbBodyK bk(od);
        m_db_obody->del(txn, &bk, 0);
        m_db_ohead->del(txn, &ok, 0);
    }
    ncp->close();

    ret = m_db_name->del(txn, &key, 0);

    return ret;
}

#endif

/* Save all changes done in the current transaction. */
bool wb_vrepdb::commit(pwr_tStatus *sts)
{
    int ret;
    
    if ((ret = m_txn->commit(0)) != 0) {
        err(ret, "commit");
        exit(1);
    }
    txn_checkpoint(0, 0, 0);

    return true;
}

bool wb_vrepdb::abort(pwr_tStatus *sts)
{    
    int ret;
    
    if ((ret = m_txn->abort()) != 0) {
        err(ret, "abort");
        exit(1);
    }

    return true;
}


wb_orep* wb_vrepdb::object(pwr_tStatus *sts, pwr_tOid oid)
{
    int ret;
    
    wb_orepdb *o = new wb_orepdb();
    
    do {        
        oh_k ohk(oid.oix);
        oh_d ohd(o);
    
        ret = m_db_ohead->get(m_txn, &ohk, &ohd, 0);
        if (ret != 0) break;
    
        return o;
    } while (0);

    delete o;
    return 0;
}

wb_orep* wb_vrepdb::object(pwr_tStatus *sts, wb_orep *parent, wb_name name)
{
    int ret;
    
    wb_orepdb *o = new wb_orepdb();

    do {        
        na_k nak(parent->oix(), name);
        na_d nad;

        ret = m_db_name->get(m_txn, &nak, &nad, 0);
        if (ret != 0) break;
        
        oh_k ok(nad.oix());
        oh_d od(o);
    
        ret = m_db_ohead->get(m_txn, &ok, &od, 0);
    
        if (ret != 0) break;

        return o;
    } while (0);
    
    delete o;
    return 0;
}


wb_orep* wb_vrepdb::createObject(pwr_tStatus *sts, wb_cdef cdef, wb_destination d, wb_name name)
{
    wb_orepdb *o = new wb_orepdb();
    DbTxn *txn = 0;
    int ret;
    
    if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
        err(ret, "txn_begin");
        exit(1);
    }
        
    try {        
        //o->oix(new_oix(txn));
        //put_ohead(txn, o);
        //put_clist(txn, o);
        //put_name(txn, o);
        //put_body(txn, o);

        if ((ret = txn->commit(0)) != 0) {
            err(ret, "commit");
            exit(1);
        }
        return o;
    }
    catch (DbException &e) {
        delete o;
        
        if ((ret = txn->abort()) != 0) {
            err(ret, "abort");
            exit(1);
        }
        return 0;
    }    
}

bool wb_vrepdb::deleteObject(pwr_tStatus *sts, wb_orep *o)
{
    DbTxn *txn = 0;
    int ret;
    
    
    if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
        err(ret, "txn_begin");
        exit(1);
    }
        
    try {
        //unadopt(txn, wb_Position(o));
        //del_ohead(txn, o);
        //del_clist(txn, o);
        //del_body(txn, o);

        if ((ret = txn->commit(0)) != 0) {
            err(ret, "commit");
            exit(1);
        }
        //o->mark(is_deleted);
        
    }
    catch (DbException &e) {
        if ((ret = txn->abort()) != 0) {
            err(ret, "abort");
            exit(1);
        }
    }
    
    return true;
}


bool wb_vrepdb::deleteFamily(pwr_tStatus *sts, wb_orep *o)
{
    DbTxn *txn = 0;
    int ret;
    
    
    if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
        err(ret, "txn_begin");
        exit(1);
    }
        
    try {
        //unadopt(txn, wb_Position(o));
        //del_ohead(txn, o);
        //del_clist(txn, o);
        //del_body(txn, o);

        if ((ret = txn->commit(0)) != 0) {
            err(ret, "commit");
            exit(1);
        }
        //o->mark(is_deleted);
        
    }
    catch (DbException &e) {
        if ((ret = txn->abort()) != 0) {
            err(ret, "abort");
            exit(1);
        }
    }
    
    return true;
}


bool wb_vrepdb::deleteOset(pwr_tStatus *sts, wb_oset *o)
{
    DbTxn *txn = 0;
    int ret;
    
    
    if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
        err(ret, "txn_begin");
        exit(1);
    }
        
    try {
        //del_family(txn, o);
        //unadopt(txn, wb_Position(o));
        //del_ohead(txn, o);
        //del_clist(txn, o);
        //del_name(txn, o);
        //del_body(txn, o);

        if ((ret = txn->commit(0)) != 0) {
            err(ret, "commit");
            exit(1);
        }
    }
    catch (DbException &e) {
        if ((ret = txn->abort()) != 0) {
            err(ret, "abort");
            exit(1);
        }
    }

    return true;
}


/*
void wb_orepdb::move(pwr_tStatus *sts, wb_Position *p)
{

    // check that object is local
    if (!isLocal()) {
        return;
    }
    
    // check that destination is local
    if (!islocal(d->parent())) {
        return;
    }

    if (isoffspring(d->parent())) {
        return;
    }

    triggAnteMove();
    triggAnteUnadopt();
    d->parent().triggAnteAdopt(this);
    
    
    o = d.parent().child(m_name);
    if (o != this) {
        *sts = LDH__NAMALREXI;
        return;
    }

    m_db->move(m_orep, p)
    unadopt();
    d.parent().adopt(this);
}
*/

bool wb_vrepdb::moveObject(pwr_tStatus *sts, wb_orep *o, wb_destination d)
{
    DbTxn *txn = 0;
    int ret;

#if 0
    if (!isLocal(o))
        return ;
    if (!isLocal(d))
        return ;
    wb_location l = o.location();
    //l.trigAnteUnadopt();
    //d.trigAnteAdopt();
#endif
    
    if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
        err(ret, "txn_begin");
        exit(1);
    }
        
    try {
        //unadopt(txn, l);
        //adopt(txn, d);
        //o->location(nloc);
        //put_ohead(txn, o, 0);

        if ((ret = txn->commit(0)) != 0) {
            err(ret, "commit");
            exit(1);
        }
    }
    catch (DbException &e) {
        if ((ret = txn->abort()) != 0) {
            err(ret, "abort");
            exit(1);
        }
    }
    
    return true;
}

bool wb_vrepdb::renameObject(pwr_tStatus *sts, wb_orep *o, wb_name name)
{
    DbTxn *txn = 0;
    int ret;
    
    
    if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
        err(ret, "txn_begin");
        exit(1);
    }
        
    try {
        //del_name(txn, o);
        //o->name(name);
        //put_name(txn, o, 0);
        //put_ohead(txn, o, 0);

        if ((ret = txn->commit(0)) != 0) {
            err(ret, "commit");
            exit(1);
        }
    }
    catch (DbException &e) {
        if ((ret = txn->abort()) != 0) {
            err(ret, "abort");
            exit(1);
        }
        // ?? How do we reset the Orep ???, the name was changed
    }
    return true;
}

bool wb_vrepdb::writeAttribute(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, unsigned int offset, unsigned int size, void *p)
{
    //body.oix = ?;
    //body.bix = ?;
    //key.data = &body;
    //key.len = sizeof(body);
    
    //data.data = ?;
    //data.offset = ?;
    //data.size = ?;
    
    //update(body-tab)

    return true;
}

void *
wb_vrepdb::readAttribute(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, unsigned int offset, unsigned int size, void *p)
{
    ob_k obk(o->oix(), bix);
    //ob_d obd;
    
    //obd.set_doff(offset);
    //obd.set_dlen(size);
    
    
    DbTxn *txn = 0;
    int ret;
    
    
    if ((ret = txn_begin(m_txn, &txn, 0)) != 0) {
        err(ret, "txn_begin");
        exit(1);
    }
        
    try {
        //m_db.obody.get(txn, &bk, &bd, 0);

        if ((ret = txn->commit(0)) != 0) {
            err(ret, "commit");
            exit(1);
        }
    }
    catch (DbException &e) {
        if ((ret = txn->abort()) != 0) {
            err(ret, "abort");
            exit(1);
        }
        // ?? How do we reset the Orep ???, the name was changed
    }
    return 0;
}

void *
wb_vrepdb::readBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p)
{
    return 0;
}

bool
wb_vrepdb::writeBody(pwr_tStatus *sts, wb_orep *o, cdh_eBix bix, void *p)
{
    return true;
}

wb_orep *
wb_vrepdb::ancestor(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}

wb_orep *
wb_vrepdb::parent(pwr_tStatus *sts, wb_orep *o) const
{
    wb_orep *orep = 0;
    
    //int ret = get_ohead(&orep, m_txn, o->m_ohead.poid);
    
    return orep;
}

wb_orep *
wb_vrepdb::after(pwr_tStatus *sts, wb_orep *o) const
{
    wb_orep *orep = 0;
    
    //int ret = get_ohead(&orep, m_txn, o->m_ohead.aoid);
    
    return orep;
}

wb_orep *
wb_vrepdb::before(pwr_tStatus *sts, wb_orep *o) const
{
    wb_orep *orep = 0;
    
    //int ret = get_ohead(&orep, m_txn, o->m_ohead.boid);
    
    return orep;
}

wb_orep *
wb_vrepdb::first(pwr_tStatus *sts, wb_orep *o) const
{
    wb_orep *orep = 0;
    
    //int ret = get_ohead(&orep, m_txn, o->m_ohead.foid);
    
    return orep;
}

wb_orep *wb_vrepdb::child(pwr_tStatus *sts, wb_orep *o, char *name) const
{
    return 0;
}

wb_orep *wb_vrepdb::last(pwr_tStatus *sts, wb_orep *o) const
{
    wb_orep *orep = 0;
    
    //int ret = get_ohead(&orep, m_txn, o->m_ohead.loid);
    
    return orep;
}

wb_orep *wb_vrepdb::next(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}

wb_orep *wb_vrepdb::previous(pwr_tStatus *sts, wb_orep *o) const
{
    return 0;
}

