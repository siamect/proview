package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;

public class GeDyn {
    public static final int mDynType_No			= 0;
    public static final int mDynType_Inherit		= 1 << 0;
    public static final int mDynType_Tone		= 1 << 1;
    public static final int mDynType_DigLowColor       	= 1 << 2;
    public static final int mDynType_DigColor		= 1 << 3;
    public static final int mDynType_DigError		= 1 << 4;
    public static final int mDynType_DigWarning		= 1 << 5;
    public static final int mDynType_DigFlash		= 1 << 6;
    public static final int mDynType_Invisible		= 1 << 7;
    public static final int mDynType_DigBorder		= 1 << 8;
    public static final int mDynType_DigText		= 1 << 9;
    public static final int mDynType_Value		= 1 << 10;
    public static final int mDynType_AnalogColor       	= 1 << 11;
    public static final int mDynType_Rotate		= 1 << 12;
    public static final int mDynType_Move	       	= 1 << 13;
    public static final int mDynType_AnalogShift       	= 1 << 14;
    public static final int mDynType_DigShift		= 1 << 15;
    public static final int mDynType_Animation		= 1 << 16;
    public static final int mDynType_Bar	       	= 1 << 17;
    public static final int mDynType_Trend		= 1 << 18;
    public static final int mDynType_SliderBackground	= 1 << 19;
    public static final int mDynType_Video		= 1 << 20;
    public static final int mDynType_FillLevel		= 1 << 21;
    public static final int mDynType_FastCurve		= 1 << 22;
    public static final int mDynType_AnalogText		= 1 << 23;
    public static final int mDynType_Table		= 1 << 24;

    public static final int mActionType_No		= 0;
    public static final int mActionType_Inherit		= 1 << 0;
    public static final int mActionType_PopupMenu      	= 1 << 1;
    public static final int mActionType_SetDig		= 1 << 2;
    public static final int mActionType_ResetDig       	= 1 << 3;
    public static final int mActionType_ToggleDig      	= 1 << 4;
    public static final int mActionType_StoDig		= 1 << 5;
    public static final int mActionType_Command		= 1 << 6;
    public static final int mActionType_CommandDoubleClick = 1 << 7;
    public static final int mActionType_Confirm		= 1 << 8;
    public static final int mActionType_IncrAnalog	= 1 << 9;
    public static final int mActionType_RadioButton	= 1 << 10;
    public static final int mActionType_Slider		= 1 << 11;
    public static final int mActionType_ValueInput	= 1 << 12;
    public static final int mActionType_TipText		= 1 << 13;
    public static final int mActionType_CloseGraph     	= 1 << 18;
    public static final int mActionType_PulldownMenu    = 1 << 19;
    public static final int mActionType_OptionMenu     	= 1 << 20;

    public static final int eAnimSequence_Cycle		= 0;
    public static final int eAnimSequence_Dig		= 1;
    public static final int eAnimSequence_ForwBack	= 2;

    public static final int eLimitType_Gt		= 0;
    public static final int eLimitType_Lt		= 1;

    public static final int eEvent_MB1Up		= 0;
    public static final int eEvent_MB1Down		= 1;
    public static final int eEvent_MB1Click		= 2;
    public static final int eEvent_MB3Press		= 3;
    public static final int eEvent_SliderMoved		= 4;
    public static final int eEvent_ValueChanged		= 5;
    public static final int eEvent_FocusLost		= 6;


    public int 		dynType;
    public int		actionType;
    public int		access;
    public GeDynElemIfc[] elements;
    public GeComponentIfc comp;
    public String       instance; 
    public boolean	ignoreColor;
    public boolean	resetColor;
    public boolean	repaintNow;
    public JopSession	session;
    public JopEngine	en;

    public boolean	invisible;
    public boolean	invisibleOld;
    public double         rotate;
    public double         x0;
    public double         y0;

    public GeDyn( Object comp) {
	this.comp = (GeComponentIfc) comp;
    }
    public GeDyn( Object comp, int dynType, int actionType, int access, GeDynElemIfc[] elements) {
	this.comp = (GeComponentIfc) comp;
	this.dynType = dynType;
	this.actionType = actionType;
	this.access = access;
	this.elements = elements;
    }
    public void setSession( JopSession session) {
	this.session = session;
	en = session.getEngine();
    }
    public void setDynType( int dynType) {
	this.dynType = dynType;
    }
    public void setActionType( int actionType) {
	this.actionType = actionType;
    }
    public void setAccess( int access) {
	this.access = access;
    }
    public void setInstance( String instance) {
	this.instance = instance;
    }
    public void setElements( GeDynElemIfc[] elements) {
	this.elements = elements;
    }
    public void connect() {
	if ( elements == null)
	    return;

	for ( int i = 0; i < elements.length; i++)
	    elements[i].connect();
    }
    public void disconnect() {
	if ( elements == null)
	    return;

	for ( int i = 0; i < elements.length; i++)
	    elements[i].disconnect();
    }
    public void scan() {
	if ( elements == null)
	    return;
	resetColor = false;
	ignoreColor = false;
	repaintNow = false;
	for ( int i = 0; i < elements.length; i++)
	    elements[i].scan();
	if ( repaintNow)
	    comp.repaintForeground();
    }
    public void action( int eventType, MouseEvent e) {
	if ( elements == null)
	    return;
	repaintNow = false;
	for ( int i = 0; i < elements.length; i++)
	    elements[i].action( eventType, e);
	if ( repaintNow)
	    comp.repaintForeground();
    }

    public void confirmedAction( int eventType, MouseEvent e) {
	if ( elements == null)
	    return;
	actionType = actionType & ~mActionType_Confirm;
	for ( int i = 0; i < elements.length; i++)
	    elements[i].action( eventType, e);
	actionType = actionType | mActionType_Confirm;
    }

    public String getAttrName( String str) {
	if ( instance == null) {
	    if ( str.startsWith("!"))
		return str.substring(1);
	    else
		return str;
	}
	else {
	    String s = RtUtilities.strReplace( str, "$object", instance);
	    if ( s.startsWith("!"))
		return s.substring(1);
	    else
		return s;
	}
    }

    public String getAttrNameNoSuffix( String str) {
	int startIdx;
	String s;
	if ( instance == null)
	    s = str;
	else
	    s = RtUtilities.strReplace( str, "$object", instance);
   
	if ( s.startsWith("!"))
	    startIdx = 1;
	else
	    startIdx = 0;

	int idx1 = s.indexOf('#');
	if ( idx1 != -1) {
	    int idx2 = s.indexOf('[');
	    if ( idx2 != -1)
		return s.substring( startIdx, idx1) + s.substring(idx2);
	    else
		return s.substring( startIdx, idx1);
	}
	else
	    return s.substring(startIdx);
    }
    static public boolean getAttrInverted( String str) {
	return str.startsWith("!");
    }

}









