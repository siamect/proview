package jpwr.app;
import android.graphics.Color;

import jpwr.pwrxtt.R;

import jpwr.rt.*;

public class AXtt {
	public static final int CRR_READ 		= 0;
	public static final int CRR_WRITE 		= 1;
	public static final int CRR_READWRITE 	= 2;
	
	public Gdh gdh;
	public PlowCmn cmn;
	public PlowNodeClass ncObject;
	public PlowPixmapData iconMap;
	public PlowPixmapData iconOpenmap;
	public PlowPixmapData iconLeaf;
	public PlowPixmapData iconAttr;
	public PlowPixmapData iconAttrArray;
	public PlowPixmapData iconAttrArrayElem;
	public PlowPixmapData iconAttrEnum;
	public PlowPixmapData iconObject;
	public PlowPixmapData iconCrrRead;
	public PlowPixmapData iconCrrWrite;
	
	public AXtt(PlowCmn cmn, Gdh gdh) {
		this.gdh = gdh;
		this.cmn = cmn;

		createPixmaps();
		createNodeClasses();
	}

	public void createPixmaps() {	
		// iconOpenmap = new PlowPixmapData(cmn, R.drawable.xtthelp_icon_openmap);
		iconLeaf = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_leaf8, R.drawable.xnav_bitmap_leaf10, 
				R.drawable.xnav_bitmap_leaf12, R.drawable.xnav_bitmap_leaf14, R.drawable.xnav_bitmap_leaf16, 
				R.drawable.xnav_bitmap_leaf18, R.drawable.xnav_bitmap_leaf20, R.drawable.xnav_bitmap_leaf24});
		iconOpenmap = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_openmap8, R.drawable.xnav_bitmap_openmap10, 
				R.drawable.xnav_bitmap_openmap12, R.drawable.xnav_bitmap_openmap14, R.drawable.xnav_bitmap_openmap16, 
				R.drawable.xnav_bitmap_openmap18, R.drawable.xnav_bitmap_openmap20, R.drawable.xnav_bitmap_openmap24});
		iconMap = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_map8, R.drawable.xnav_bitmap_map10, 
				R.drawable.xnav_bitmap_map12, R.drawable.xnav_bitmap_map14, R.drawable.xnav_bitmap_map16, 
				R.drawable.xnav_bitmap_map18, R.drawable.xnav_bitmap_map20, R.drawable.xnav_bitmap_map24});
		iconAttr = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_attr8, R.drawable.xnav_bitmap_attr10, 
				R.drawable.xnav_bitmap_attr12, R.drawable.xnav_bitmap_attr14, R.drawable.xnav_bitmap_attr16, 
				R.drawable.xnav_bitmap_attr18, R.drawable.xnav_bitmap_attr20, R.drawable.xnav_bitmap_attr24});
		iconAttrArray = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_attrarra8, R.drawable.xnav_bitmap_attrarra10, 
				R.drawable.xnav_bitmap_attrarra12, R.drawable.xnav_bitmap_attrarra14, R.drawable.xnav_bitmap_attrarra16, 
				R.drawable.xnav_bitmap_attrarra18, R.drawable.xnav_bitmap_attrarra20, R.drawable.xnav_bitmap_attrarra24});
		iconObject = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_object8, R.drawable.xnav_bitmap_object10, 
				R.drawable.xnav_bitmap_object12, R.drawable.xnav_bitmap_object14, R.drawable.xnav_bitmap_object16, 
				R.drawable.xnav_bitmap_object18, R.drawable.xnav_bitmap_object20, R.drawable.xnav_bitmap_object24});
		iconAttrArrayElem = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_attrarel8, R.drawable.xnav_bitmap_attrarel10, 
				R.drawable.xnav_bitmap_attrarel12, R.drawable.xnav_bitmap_attrarel14, R.drawable.xnav_bitmap_attrarel16, 
				R.drawable.xnav_bitmap_attrarel18, R.drawable.xnav_bitmap_attrarel20, R.drawable.xnav_bitmap_attrarel24});
		iconAttrEnum = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_attrenum8, R.drawable.xnav_bitmap_attrenum10, 
				R.drawable.xnav_bitmap_attrenum12, R.drawable.xnav_bitmap_attrenum14, R.drawable.xnav_bitmap_attrenum16, 
				R.drawable.xnav_bitmap_attrenum18, R.drawable.xnav_bitmap_attrenum20, R.drawable.xnav_bitmap_attrenum24});
		iconCrrRead = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_crrread8, R.drawable.xnav_bitmap_crrread10, 
				R.drawable.xnav_bitmap_crrread12, R.drawable.xnav_bitmap_crrread14, R.drawable.xnav_bitmap_crrread16, 
				R.drawable.xnav_bitmap_crrread18, R.drawable.xnav_bitmap_crrread20, R.drawable.xnav_bitmap_crrread24});
		iconCrrWrite = new PlowPixmapData(cmn, new int[] {R.drawable.xnav_bitmap_crrwrite8, R.drawable.xnav_bitmap_crrwrite10, 
				R.drawable.xnav_bitmap_crrwrite12, R.drawable.xnav_bitmap_crrwrite14, R.drawable.xnav_bitmap_crrwrite16, 
				R.drawable.xnav_bitmap_crrwrite18, R.drawable.xnav_bitmap_crrwrite20, R.drawable.xnav_bitmap_crrwrite24});
	}
	
	public void createNodeClasses() {
		PlowRect r1 = new PlowRect(cmn, 0D, 0D, 50D, 1.5D, Plow.COLOR_WHITE, Plow.COLOR_BLACK,true,false,false);
		PlowAnnot a1 = new PlowAnnot(cmn, 3D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.NEXT_RELATIVE_POSITION, 0);
		PlowAnnot a11 = new PlowAnnot(cmn, 9D, 1.1D, 12, Plow.COLOR_BLACK, 0, PlowAnnot.RELATIVE_POSITION, 1);
		PlowAnnotPixmap p1 = new PlowAnnotPixmap(cmn, 1D, 0.28D, 0);
		ncObject = new PlowNodeClass(cmn);
		ncObject.insert(r1);
		ncObject.insert(a1);
		ncObject.insert(a11);
		ncObject.insert(p1);
		cmn.insert_nc(ncObject);		
	}
}
