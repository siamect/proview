package jpwr.jop;
import jpwr.rt.*;
import java.io.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.border.*;
import java.awt.event.*;
import java.net.*;
import java.applet.*;


public class JopLang {

    public static final int ITEM_TYPE_EXACT = 0;
    public static final int ITEM_TYPE_BEGINNING = 1;
    public static final int LANGUAGE_      = 0;
    public static final int LANGUAGE_af_ZA = 1;
    public static final int LANGUAGE_ar_AE = 2;
    public static final int LANGUAGE_ar_BH = 3;
    public static final int LANGUAGE_ar_DZ = 4;
    public static final int LANGUAGE_ar_EG = 5;
    public static final int LANGUAGE_ar_IN = 6;
    public static final int LANGUAGE_ar_IQ = 7;
    public static final int LANGUAGE_ar_JO = 8;
    public static final int LANGUAGE_ar_KW = 9;
    public static final int LANGUAGE_ar_LB = 10;
    public static final int LANGUAGE_ar_LY = 11;
    public static final int LANGUAGE_ar_MA = 12;
    public static final int LANGUAGE_ar_OM = 13;
    public static final int LANGUAGE_ar_QA = 14;
    public static final int LANGUAGE_ar_SA = 15;
    public static final int LANGUAGE_ar_SD = 16;
    public static final int LANGUAGE_ar_SY = 17;
    public static final int LANGUAGE_ar_TN = 18;
    public static final int LANGUAGE_ar_YE = 19;
    public static final int LANGUAGE_be_BY = 20;
    public static final int LANGUAGE_bg_BG = 21;
    public static final int LANGUAGE_br_FR = 22;
    public static final int LANGUAGE_bs_BA = 23;
    public static final int LANGUAGE_ca_ES = 24;
    public static final int LANGUAGE_cs_CZ = 25;
    public static final int LANGUAGE_cy_GB = 26;
    public static final int LANGUAGE_da_DK = 27;
    public static final int LANGUAGE_de_AT = 28;
    public static final int LANGUAGE_de_BE = 29;
    public static final int LANGUAGE_de_CH = 30;
    public static final int LANGUAGE_de_DE = 31;
    public static final int LANGUAGE_de_LU = 32;
    public static final int LANGUAGE_el_GR = 33;
    public static final int LANGUAGE_en_AU = 34;
    public static final int LANGUAGE_en_BW = 35;
    public static final int LANGUAGE_en_CA = 36;
    public static final int LANGUAGE_en_DK = 37;
    public static final int LANGUAGE_en_GB = 38;
    public static final int LANGUAGE_en_HK = 39;
    public static final int LANGUAGE_en_IE = 40;
    public static final int LANGUAGE_en_IN = 41;
    public static final int LANGUAGE_en_NZ = 42;
    public static final int LANGUAGE_en_PH = 43;
    public static final int LANGUAGE_en_SG = 44;
    public static final int LANGUAGE_en_US = 45;
    public static final int LANGUAGE_en_ZA = 46;
    public static final int LANGUAGE_en_ZW = 47;
    public static final int LANGUAGE_es_AR = 48;
    public static final int LANGUAGE_es_BO = 49;
    public static final int LANGUAGE_es_CL = 50;
    public static final int LANGUAGE_es_CO = 51;
    public static final int LANGUAGE_es_CR = 52;
    public static final int LANGUAGE_es_DO = 53;
    public static final int LANGUAGE_es_EC = 54;
    public static final int LANGUAGE_es_ES = 55;
    public static final int LANGUAGE_es_GT = 56;
    public static final int LANGUAGE_es_HN = 57;
    public static final int LANGUAGE_es_MX = 58;
    public static final int LANGUAGE_es_NI = 59;
    public static final int LANGUAGE_es_PA = 60;
    public static final int LANGUAGE_es_PE = 61;
    public static final int LANGUAGE_es_PR = 62;
    public static final int LANGUAGE_es_PY = 63;
    public static final int LANGUAGE_es_SV = 64;
    public static final int LANGUAGE_es_US = 65;
    public static final int LANGUAGE_es_UY = 66;
    public static final int LANGUAGE_es_VE = 67;
    public static final int LANGUAGE_et_EE = 68;
    public static final int LANGUAGE_eu_ES = 69;
    public static final int LANGUAGE_fa_IR = 70;
    public static final int LANGUAGE_fi_FI = 71;
    public static final int LANGUAGE_fo_FO = 72;
    public static final int LANGUAGE_fr_BE = 73;
    public static final int LANGUAGE_fr_CA = 74;
    public static final int LANGUAGE_fr_CH = 75;
    public static final int LANGUAGE_fr_FR = 76;
    public static final int LANGUAGE_fr_LU = 77;
    public static final int LANGUAGE_ga_IE = 78;
    public static final int LANGUAGE_gl_ES = 79;
    public static final int LANGUAGE_gv_GB = 80;
    public static final int LANGUAGE_he_IL = 81;
    public static final int LANGUAGE_hi_IN = 82;
    public static final int LANGUAGE_hr_HR = 83;
    public static final int LANGUAGE_hu_HU = 84;
    public static final int LANGUAGE_id_ID = 85;
    public static final int LANGUAGE_is_IS = 86;
    public static final int LANGUAGE_it_CH = 87;
    public static final int LANGUAGE_it_IT = 88;
    public static final int LANGUAGE_iw_IL = 89;
    public static final int LANGUAGE_ja_JP = 90;
    public static final int LANGUAGE_ka_GE = 91;
    public static final int LANGUAGE_kl_GL = 92;
    public static final int LANGUAGE_ko_KR = 93;
    public static final int LANGUAGE_kw_GB = 94;
    public static final int LANGUAGE_lt_LT = 95;
    public static final int LANGUAGE_lv_LV = 96;
    public static final int LANGUAGE_mi_NZ = 97;
    public static final int LANGUAGE_mk_MK = 98;
    public static final int LANGUAGE_mr_IN = 99;
    public static final int LANGUAGE_ms_MY = 100;
    public static final int LANGUAGE_mt_MT = 101;
    public static final int LANGUAGE_nl_BE = 102;
    public static final int LANGUAGE_nl_NL = 103;
    public static final int LANGUAGE_nn_NO = 104;
    public static final int LANGUAGE_no_NO = 105;
    public static final int LANGUAGE_oc_FR = 106;
    public static final int LANGUAGE_pl_PL = 107;
    public static final int LANGUAGE_pt_BR = 108;
    public static final int LANGUAGE_pt_PT = 109;
    public static final int LANGUAGE_ro_RO = 110;
    public static final int LANGUAGE_ru_RU = 111;
    public static final int LANGUAGE_ru_UA = 112;
    public static final int LANGUAGE_se_NO = 113;
    public static final int LANGUAGE_sk_SK = 114;
    public static final int LANGUAGE_sl_SI = 115;
    public static final int LANGUAGE_sq_AL = 116;
    public static final int LANGUAGE_sr_YU = 117;
    public static final int LANGUAGE_sv_FI = 118;
    public static final int LANGUAGE_sv_SE = 119;
    public static final int LANGUAGE_ta_IN = 120;
    public static final int LANGUAGE_te_IN = 121;
    public static final int LANGUAGE_tg_TJ = 122;
    public static final int LANGUAGE_th_TH = 123;
    public static final int LANGUAGE_tl_PH = 124;
    public static final int LANGUAGE_tr_TR = 125;
    public static final int LANGUAGE_uk_UA = 126;
    public static final int LANGUAGE_ur_PK = 127;
    public static final int LANGUAGE_uz_UZ = 128;
    public static final int LANGUAGE_vi_VN = 129;
    public static final int LANGUAGE_wa_BE = 130;
    public static final int LANGUAGE_yi_US = 131;
    public static final int LANGUAGE_zh_CN = 132;
    public static final int LANGUAGE_zh_HK = 133;
    public static final int LANGUAGE_zh_TW = 134;
    public static final int LANGUAGE__     = 135;


    //
    // A node in a binary tree with string key
    // and data: string data and integer type
    //
    public class TreeNode {
	TreeNode left;
	TreeNode right;
	String key;
	String data;
	int type;

	public TreeNode( String key, String data, int type) {
	    this.key = key;
	    this.data = data;
	    this.type = type;
	}

	public void insert( TreeNode node) {
	    if ( key.compareTo( node.key) > 0) {
		if ( left != null)
		    left.insert( node);
		else
		    left = node;
	    }
	    else if ( key.compareTo( node.key) < 0) {
		if ( right != null)
		    right.insert( node);
		else
		    right = node;
	    }
	}

	public TreeNode find( String key) {
	    if ( this.key.compareTo( key) > 0) {
		if ( left == null)
		    return null;
		else
		    return left.find( key);
	    }
	    else if ( this.key.compareTo( key) < 0) {
		if ( right == null)
		    return null;
		else 
		    return right.find( key);
	    }
	    else
		// Hit !
		return this;
	}

	public void print() {
	    System.out.println( key + ", " + data);
	}
	public void printTree() {
	    print();
	    if ( left != null)
		left.printTree();
	    if ( right != null)
		right.printTree();
	}
    }

    //
    // A binary tree with string key
    //
    public class BinaryTree {
	TreeNode root = new TreeNode( "MKey", "RootNode", 0);

	public void insert( TreeNode node) {
	    root.insert( node);
	}
    
	public void printTree() {
	    root.printTree();
	}

	public TreeNode find( String key) {
	    return root.find( key);
	}
    }

    BinaryTree btree_en_us;
    BinaryTree btree_lng;
    JopEngine engine;
    JopSession session;
    Object root;
    int lang = LANGUAGE_en_US;
    static JopLang default_lang = null;

    public JopLang( JopSession session) {
	this.session = session;
	this.engine = session.getEngine();
	root = session.getRoot();
    }

    public JopLang() {
	engine = new JopEngine( 500, null);
	session = new JopSession( engine, null);
	root = (Object)this;
	// btree.test(); 
    }

    public static void setDefault( JopLang lng) {
	default_lang = lng;
    }
    public static String transl( String text) {
	if ( default_lang == null)
	    return text;
	return default_lang.translate( text);
    }

    public static int switchLanguage( int language) {
	if ( default_lang == null)
	    return 0;
	return default_lang.set( language);
    }

    public static int getLanguage() {
	if ( default_lang == null)
	    return 0;
	return default_lang.lang;
    }

    public int set( int language) {
	if ( lang == language)
	    return 1;

	if ( language == LANGUAGE_en_US) {
	    lang = language;
	    btree_en_us = null;
	    btree_lng = null;
	}
	else {
	    if ( !isInstalled( language))
	    	return 0;
	    
	    lang = language;
	    read();
	}
	return 1;
    }

    public int set( String str) {
	return set( stringToLng( str));
    }

    public String lngToString( int language) {
	switch ( language) {
	case LANGUAGE_en_US:
	    return "en_US";
	case LANGUAGE_sv_SE:
	    return "sv_SE";
	case LANGUAGE_de_DE:
	    return "de_DE";
	case LANGUAGE_fr_FR:
	    return "fr_FR";
	case LANGUAGE_zh_CN:
	    return "zh_CN";
	}
	return "en_US";
    }

    public int stringToLng( String str) {
	if ( str.equalsIgnoreCase( "en_US"))
	    return LANGUAGE_en_US;
	if ( str.equalsIgnoreCase( "sv_SE"))
	    return LANGUAGE_sv_SE;
	if ( str.equalsIgnoreCase( "de_DE"))
	    return LANGUAGE_de_DE;
	if ( str.equalsIgnoreCase( "fr_FR"))
	    return LANGUAGE_fr_FR;
	if ( str.equalsIgnoreCase( "zh_CN"))
	    return LANGUAGE_zh_CN;
	return 0;
    }
    public boolean isInstalled( int language) {
	// NYI
	return true;
    }

    public void read() {
	String fname1 = "en_us/xtt_lng.dat";
	String fname2 = lngToString(lang).toLowerCase() + "/xtt_lng.dat";
	String line;
	Boolean end = false;
	BufferedReader reader = null;
	String filename = "";
	int type;
    
	for ( int i = 0; i < 2; i++) {
	    if ( i == 0 && btree_en_us != null)
		continue;
	
	    if ( root != null && root instanceof JApplet) {
	      try {
		URL current = ((JApplet) root).getCodeBase();
		String current_str = current.toString();
		int idx1 = current_str.lastIndexOf('/');
		int idx2 = current_str.lastIndexOf(':');
		int idx = idx1;
		if ( idx2 > idx)
		  idx = idx2;
		String path = current_str.substring(0,idx + 1);
		if ( i == 0 )
		  filename = path + fname1;
		else
		  filename = path + fname2;
		System.out.println( "Opening file " + filename);
		URL fileURL = new URL( filename);
		InputStream in = fileURL.openStream();
		InputStreamReader r2 = new InputStreamReader(in);
		reader = new BufferedReader( r2);
	      }
	      catch ( Exception e) {
		System.out.println( "Unable to open file" + filename);
	      }
	    }
	    else {

	      if ( i == 0)
		filename = "$pwr_exe/" + fname1;
	      else
		filename = "$pwr_exe/" + fname2;
	      filename = engine.gdh.translateFilename( filename);
	      try {
		reader =  new BufferedReader(new FileReader(filename));
	      }
	      catch ( Exception e) {
		System.out.println( "Unable to open file " + filename);
		return;
	      }
	    }
	    
	    if ( i == 0)
	      btree_en_us = new BinaryTree();
	    else
	      btree_lng = new BinaryTree();


	    try {
		while( (line = reader.readLine()) != null) {
		    line = line.trim();
		    if ( line.equals("") || line.charAt(0) == '#')
			continue;

		    if ( line.charAt(0) == 'E')
			type = ITEM_TYPE_EXACT;
		    else if ( line.charAt(0) == 'B')
			type = ITEM_TYPE_BEGINNING;
		    else
			continue;
		
		    int idx = line.indexOf( ' ', 2);
		    if ( idx == -1) {
			System.out.println( "Syntax error: " + line);
			continue;
		    }
		    String key = line.substring( 2, idx);
		    String data = line.substring( idx + 2, line.length()-1);

		    if ( i == 0)			
			btree_en_us.insert( new TreeNode( data, key, type));
		    else
			btree_lng.insert( new TreeNode( key, data, type));
		}
	    }
	    catch ( Exception e) {
		System.out.println( "IOException JopLang");
	    }
	}
    }

    String translate( String text) {
	if ( lang == LANGUAGE_en_US ||
	     btree_en_us == null || btree_lng == null)
	    return text;

	TreeNode n1 = btree_en_us.find( text);
	if ( n1 == null)
	    return text;

	TreeNode n2 = btree_lng.find( n1.data);
	if ( n2 == null)
	    return text;

	return n2.data;
    }
}
		    