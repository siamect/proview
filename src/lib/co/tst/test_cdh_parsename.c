#include <stddef.h>
#include <stdio.h>
#include "pwr.h"
#include "co_cdh.h"

main ()
{
  char			string[256];
  cdh_sParseName	ParseName;
  cdh_sParseName	*pn;
  pwr_tStatus		sts;
  pwr_tObjid		poid = pwr_cNObjid;
  int			i;

  while (gets(string) != NULL) {

    pn = cdh_ParseName(&sts, &ParseName, poid, string, 0);
    if (pn == NULL) {
      printf("Error, sts = %d\n", sts);
      continue;
    }

    printf("flags: ");
    if (pn->flags.b.parent) printf("Parent,");
    if (pn->flags.b.idString) printf("IdString,");
    if (pn->flags.b.volume) printf("Volume,");
    if (pn->flags.b.path) printf("Path,");
    if (pn->flags.b.object) printf("Object,");
    if (pn->flags.b.bodyId) printf("BodyId,");
    if (pn->flags.b.bodyName) printf("BodyName,");
    if (pn->flags.b.attribute) printf("Attribute,");
    if (pn->flags.b.index) printf("Index,");
    if (pn->flags.b.escapeGMS) printf("EscapeGMS,");
    if (pn->flags.b.separator) printf("Separator,");
    printf("\n");
    printf("Parent : %s\n", cdh_ObjidToString(NULL, pn->poid, 1));
    printf("Index  : %d\n", pn->index);
    printf("Offset : %d\n", pn->offset);
    printf("Size   : %d\n", pn->size);
    printf("nobject: %d\n", pn->nObject);
    printf("nbody  : %d\n", pn->nBody);

    if (pn->flags.b.idString) {
      switch (pn->eId) {
      case cdh_eId_objectIx:
	printf("eId  : ObjectIx\n");
	printf("Value: %s\n", cdh_ObjectIxToString(NULL, pn->uId.oix, 1));
	break;
      case cdh_eId_objid:
	printf("eId  : Objid\n");
	printf("Value: %s\n", cdh_ObjidToString(NULL, pn->uId.oid, 1));
	break;
      case cdh_eId_classId:
	printf("eId  : ClassId\n");
	printf("Value: %s\n", cdh_ClassIdToString(NULL, pn->uId.cid, 1));
	break;
      case cdh_eId_volumeId:
	printf("eId  : VolumId\n");
	printf("Value: %s\n", cdh_VolumeIdToString(NULL, pn->uId.vid, 1, 1));
	break;
      case cdh_eId_typeId:
	printf("eId  : TypeId\n");
	printf("Value: %s\n", cdh_TypeIdToString(NULL, pn->uId.tid, 1));
	break;
      case cdh_eId_aref:
	printf("eId  : Aref\n");
	printf("Value: %s\n", cdh_ArefToString(NULL, &pn->uId.aref, 1));
	break;
      case cdh_eId_subid:
	printf("eId  : Subid\n");
	printf("Value: %s\n", cdh_SubidToString(NULL, pn->uId.sid, 1));
	break;
      case cdh_eId_dlid:
	printf("eId  : DLid\n");
	printf("Value: %s\n", cdh_DlidToString(NULL, pn->uId.did, 1));
	break;
      default:
	printf("eId: Error");
      }
    }
    if (pn->flags.b.volume)
      printf("Volume      : %d,%d,%c,%c, %s %s\n", pn->volume.pack.c.len, pn->volume.pack.c.hash,
	pn->volume.pack.c.first, pn->volume.pack.c.last, pn->volume.orig, pn->volume.norm);
    for (i = 0; i < pn->nObject; i++) {
      printf("Object(%.4d): %d,%d,%c,%c, %s %s\n", i, pn->object[i].pack.c.len, pn->object[i].pack.c.hash,
	pn->object[i].pack.c.first, pn->object[i].pack.c.last, pn->object[i].orig, pn->object[i].norm);
    }
    for (i = 0; i < pn->nBody; i++) {
      printf("Body(%.4d)  : %d,%d,%c,%c, %s %s\n", i, pn->body[i].pack.c.len, pn->body[i].pack.c.hash,
	pn->body[i].pack.c.first, pn->body[i].pack.c.last, pn->body[i].orig, pn->body[i].norm);
    }
    if (pn->flags.b.attribute)
      printf("Attribute   : %d,%d,%c,%c, %s %s\n", pn->attribute.pack.c.len, pn->attribute.pack.c.hash,
	pn->attribute.pack.c.first, pn->attribute.pack.c.last, pn->attribute.orig, pn->attribute.norm);
  }


}
