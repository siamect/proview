/* 
 * Proview   $Id: XttTree.java,v 1.8 2005-11-04 11:47:58 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

package jpwr.jop;
import java.awt.*;
import java.awt.Font;
import java.awt.GraphicsEnvironment;
import java.awt.event.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Enumeration;
import java.util.NoSuchElementException;
import java.util.Vector;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.plaf.basic.BasicTreeUI;
import javax.swing.tree.*;
import java.beans.SimpleBeanInfo;
import jpwr.rt.*;

/**
 *@author     Jonas Nylund
 *@created    November 12, 2002
 *@version    1.0
 */

public class XttTree extends JPanel
{
  boolean findFieldEnable = false;
    /*Mats förändringar ny boolean för enterComm tillagd (enterFieldEnable)*/
  boolean enterFieldEnable = false; 
  JPanel userPanel = new JPanel();
  BorderLayout borderLayout1 = new BorderLayout();
  JPanel messagePanel = new JPanel();
   /**  Description of the Field */
  JTextField textUser = new JTextField(10);
  /**  Description of the Field */
  JTextField userValue = new JTextField(25);
  /**  Description of the Field */
  JLabel userValueLabel = new JLabel("Value input: ");    
  JLabel userCommandLabel = new JLabel("Command:");
  JLabel labelMessage = new JLabel("Navigator ver 1.0");
  Dimension size;
  /**  Description of the Field */
  JTree tree;
  Object root;
  /**  Description of the Field */
  private JopSession session;
  /**  Description of the Field */
  private JopEngine engine;
  /**  Description of the Field */
  private boolean firstTime = true;
  /**  Description of the Field */
  private Gdh gdh;
  /**  Description of the Field */
  private DefaultMutableTreeNode rootNode;
  /**  Description of the Field */
  private boolean sim = false;
  /**  Description of the Field */
  private DefaultMutableTreeNode tNodeRoot;
  /**  Description of the Field */
  private DefaultTreeModel treeModel;
  private URL url;
  
    //Mats förändringar: popup borttagen
    //final JPopupMenu popup = new JPopupMenu();
  
  InputMap inputMap = new InputMap();
  ActionMap actionMap = new ActionMap();
  JMenuBar menubar = new JMenuBar();
  
  Image XttOpenCloseIcon;
  Image XttOpenIcon;
  Image XttCloseIcon;
  Image XttClosedLeafIcon;
  Image XttOpenLeafIcon;
  Image XttClosedNodeIcon;
  Image XttOpenNodeIcon;
  Image XttObjAttrOpenNodeIcon;
  Image XttObjAttrIcon;
  Image XttObjAttrPointerIcon;
  Image XttObjAttrArrayIcon;
  Image XttObjAttrArrayElementIcon;
  
  String[] errMess_SW = {"Fel vid kommunikation med GdhServer",
                         "Inget objekt markerat",
			 "Markerat objekt kan ej ändra värde",
			 " gick ej att tolka som"};
			 
  String[] errMess_EN = {"Could not communicate with GdhServer",
                         "No object selected",
			 "Selected object can't change value",
			 " can't be converted to"};
  String[] errMess;
			 
  String functions_EN = "Functions";
  String functions_SW = "Funktioner";
  String functions;
    
  String language_EN = "Language";
  String language_SW = "Språk";
  String language;
    
  String swedish = "Svenska";
  String english = "English";

  String openObject_EN = "open object";
  String openObject_SW = "öppna objekt";
  String openObject;
    
  String changeValue_EN = "change value";
  String changeValue_SW = "ändra värde";
  String changeValue;
    
  String debug_EN = "debug";
  String debug_SW = "avlusa";
  String debug;
  
  String working_EN = "Working.........";
  String working_SW = "Arbetar.........";
  String working;

  String openPlc_EN = "open PLC";
  String openPlc_SW = "öppna PLC";
  String openPlc;

  String showCross_EN = "show cross";
  String showCross_SW = "visa korsref";
  String showCross;

  String find_EN = "find...";
  String find_SW = "sök...";
  String find;
    //Mats förändringar: strängar för enterComm
  String enterComm_EN = "enter command";
  String enterComm_SW = "kommandorad";
  String enterComm;

  
  static final int SWEDISH = 0;
  static final int ENGLISH = 1;
  int currentLanguage = SWEDISH;
  

  
  /**
   *@param  engine  JopEngine-instansen som trädet använder för uppkopling.
   */
  public XttTree(JopSession session, URL url, Object root)
  {
    Logg.text_lbl = this.labelMessage;
    Logg.logg("ver 1.9.6", 0);
    
    this.session = session;
    this.root = root;
    this.url = url;
    this.engine = session.getEngine();
    session.setNavigator( (Object) this);
    this.gdh = engine.gdh;
 
    //construct the messagepanel and add it to the panel
    setLayout(borderLayout1);
    Dimension d = messagePanel.getSize();
    d.height += 20;
    messagePanel.setPreferredSize(d);
    add(messagePanel, BorderLayout.SOUTH);
    userValue.setPreferredSize(d);
    messagePanel.setLayout(new BorderLayout());
    messagePanel.add(labelMessage, BorderLayout.CENTER);
    setOpaque(true);
    userPanel.setLayout(new GridBagLayout());
    
    //initialize the strings depending on the requested language
    switch (currentLanguage)
    {
      case SWEDISH :
        functions = functions_SW;
	language = language_SW;
	openObject = openObject_SW;
	changeValue = changeValue_SW;
	debug = debug_SW;
	working = working_SW;
	errMess = errMess_SW;
	openPlc = openPlc_SW;
	showCross = showCross_SW;
	find = find_SW;
	enterComm=enterComm_SW;
	
      break;
      case ENGLISH :
        functions = functions_EN;
	language = language_EN;
	openObject = openObject_EN;
	changeValue = changeValue_EN;
	debug = debug_EN;
	working = working_EN;
	errMess = errMess_EN;
	openPlc = openPlc_EN;
	showCross = showCross_EN;
	find = find_EN;
	enterComm=enterComm_EN;
      break;
    }
    //get all icons that is to be used in the tree
    XttOpenCloseIcon = JopSpider.getImage(session, "jpwr/jop/xttopencloseicon.gif");
    XttOpenIcon = JopSpider.getImage(session, "jpwr/jop/xttopenicon.gif");
    XttCloseIcon = JopSpider.getImage(session, "jpwr/jop/xttcloseicon.gif");
    XttClosedLeafIcon = JopSpider.getImage(session, "jpwr/jop/xttclosedleaficon.gif");
    XttOpenLeafIcon = JopSpider.getImage(session, "jpwr/jop/xttopenleaficon.gif");
    XttClosedNodeIcon = JopSpider.getImage(session, "jpwr/jop/xttclosednodeicon.gif");
    XttOpenNodeIcon = JopSpider.getImage(session, "jpwr/jop/xttopennodeicon.gif");
    XttObjAttrOpenNodeIcon = JopSpider.getImage(session, "jpwr/jop/xttobjattropennodeicon.gif");
    XttObjAttrIcon = JopSpider.getImage(session, "jpwr/jop/xttobjattricon.gif");
    XttObjAttrPointerIcon = JopSpider.getImage(session, "jpwr/jop/xttobjattrpointericon.gif");
    XttObjAttrArrayIcon = JopSpider.getImage(session, "jpwr/jop/xttobjattrarrayicon.gif");
    XttObjAttrArrayElementIcon = JopSpider.getImage(session, "jpwr/jop/xttobjattrarrayelementicon.gif");

    //create the JTree component
    rootNode = new DefaultMutableTreeNode("DATABAS");
    treeModel = new DefaultTreeModel(rootNode);
    tree = new JTree(treeModel);
    tree.setEditable(false);
    this.tree.setFont(new Font("monospaced", Font.PLAIN, 14));
    this.tree.setCellRenderer(new MyRenderer());
    
    //ugly way to letting all nodes to be able to update the tree when their
    //values change...
    XttObjAttr.treeModel = treeModel;
    //ugly way to letting all objects that wants subscription to get a handle to gdh
    DynamicObj.init(engine);
    
    //get the icons that is for the + and - signs to the left of every node
    BasicTreeUI ui = (BasicTreeUI)tree.getUI();
    ImageIcon OpenCloseIcon = new ImageIcon(XttOpenCloseIcon);
    ImageIcon OpenIcon = new ImageIcon(XttOpenIcon);
    ImageIcon CloseIcon = new ImageIcon(XttCloseIcon);
    ui.setCollapsedIcon(OpenIcon);
    ui.setExpandedIcon(CloseIcon);

    tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);

    final JopScrollPane scrollPane = new JopScrollPane();
    scrollPane.setViewportView(tree);
    scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    add(scrollPane, BorderLayout.CENTER);

    tree.addTreeWillExpandListener(new MyTreeWillExpandListener());
    tree.addTreeExpansionListener(new MyTreeExpansionListener());

    /*---Bygg upp trädet--------------------------------------------------------------*/
    CdhrObjid r = gdh.getRootList();
    if(r.evenSts())
    {
      Logg.logg("XttTree: Kan inte hitta roten avslutar initieringen av trädet", 1);
      return;
    }
    tNodeRoot = new DefaultMutableTreeNode(createXttObj(r));
    rootNode.add(tNodeRoot);
    this.addFirstChild(tNodeRoot);
    int rootRow = 0;
    Vector v = gdh.getAllXttSiblings(r.objid);
    if(v == null)
    {
      Logg.loggToApplet(errMess[0]);
    }
    else
    {
      for(int i = 0; i < v.size(); i++)
      {
        //String loggStr = new String("XttTree: Börjar bygga root " + rootRow++);
        //Logg.logg(loggStr, 0);
        
	GdhrGetXttObj gdhr = (GdhrGetXttObj)v.get(i);
        XttObj xttObj = new XttObj(gdhr, this.gdh, this.engine);
        tNodeRoot = new DefaultMutableTreeNode(xttObj);
        xttObj.treeNode = tNodeRoot;
        xttObj.init(false);
        if(xttObj.refObj != null)
	{
	  xttObj.refObj.objAttr.treeModel = this.treeModel;
        }
        rootNode.add(tNodeRoot);
        if(gdhr.hasChildren)
        {
          DefaultMutableTreeNode childNode = new DefaultMutableTreeNode(new XttObj("DummyXttObj"));
          tNodeRoot.add(childNode);
        }
      }
    }
    /*---slut på bygg upp trädet--------------------------------------------------------*/
    
    //make the root invisible(it's just there becasue the JTree component must have exactly 1 root)
    //then expand row 0 so we can se the rootlevel in the system
    tree.expandRow(0);
    
    tree.setScrollsOnExpand(true);
    tree.setRootVisible(false);
    tree.setShowsRootHandles(true);
    tree.setSelectionRow(0);

    
    //add an actionlistener that responds to userinput in the messagepanel
    this.userValue.addActionListener(
      new ActionListener()
      {
        public void actionPerformed(ActionEvent evt)
        {

	    /*Mats förändringar: Omstrukturering av ifsatser + en tillagd ifsats
	      för enterFieldEnable*/
	    if(enterFieldEnable)
	  {
            Logg.logg("XttTree: innan enterCommand:(" + userValue.getText() + ");", 6);
            enterComm(userValue.getText());
	    messagePanel.remove(userValue);
	    messagePanel.remove(userCommandLabel);
	    enterFieldEnable=false;
          }
	    else if(findFieldEnable)
	  {
            Logg.logg("XttTree: innan find(" + userValue.getText() + ");", 6);
            find(userValue.getText());
	    messagePanel.remove(userValue);
	    messagePanel.remove(userValueLabel);
	    findFieldEnable=false;
          }
	  else
	  {
            Logg.logg("XttTree: innan changeValue(" + userValue.getText() + ");", 6);
            changeValue(userValue.getText());
	    messagePanel.remove(userValue);
	    messagePanel.remove(userValueLabel);
	  }         
	  tree.setRequestFocusEnabled(true);

          messagePanel.add(labelMessage, BorderLayout.CENTER);
          messagePanel.doLayout();
          repaint();

          tree.requestFocus();
        }
      });
    Logg.logg("XttTree ready", 0);
  }
  
  /**
   *  Adderar alla barn till en nod i trädet. Kontrollerar också om barnen har
   *  barn, om så är fallet adderas det första barnet till barnet!!!
   *
   *@param  tN  noden vars eventuella barn skall adderas.
   *@param  tC  nodens första barn som alltid måste finnas qqq
   */
  public void addChildren(DefaultMutableTreeNode tN, DefaultMutableTreeNode tC)
  {
    tN.remove(tC);
    XttObj obj = (XttObj)tN.getUserObject();
    Vector v = this.gdh.getAllXttChildren(obj.objId.objid);
    if(!(v instanceof Vector))
    {
      Logg.logg("XttTree: getAllXttChildren returnerar annat än Vector", 1);
      return;
    }
    //används utifall användaren angett debug
    Vector ref_vec = new Vector();
    //används utifall användaren angett debug
    Vector o_vec = new Vector();

    //loopa igenom resultat-vektorn
    for(int i = 0; i < v.size(); i++)
    {
      GdhrGetXttObj gdhr = (GdhrGetXttObj)v.get(i);
      if(gdhr.evenSts())
      {
        Logg.logg("XttTree: getAllXttChildren har returnerat ett objekt med felstatus", 1);
        break;
      }
      XttObj o = new XttObj(gdhr, this.gdh, this.engine);
      DefaultMutableTreeNode childNode = new DefaultMutableTreeNode(o);
      o.treeNode = childNode;
      o.init(obj.debug);
      if(obj.debug && o.refObj != null)
      {
        ref_vec.add(((XttObjAttr)(o.refObj.getXttObjAttr())).fullName);
        o_vec.add(o.refObj);
      }
      tN.add(childNode);
      if(gdhr.hasChildren)
      {
        DefaultMutableTreeNode c = new DefaultMutableTreeNode(new XttObj("DummyXttObj"));
        childNode.add(c);
      }
    }
    if(obj.debug)
    {
      Vector ret_vec = engine.gdh.refObjectInfo_Vector(ref_vec);
      if(ret_vec == null)
      {
        Logg.logg("refObjectInfo_Vector returnerar null vid debug", 1);
        return;
      }
      for(int i = 0; i < o_vec.size() && i < ret_vec.size(); i++)
      {
        ((XttRefObj)(o_vec.get(i))).objAttr.refObj = (GdhrRefObjectInfo)ret_vec.get(i);
      }
      if(!engine.isReady())
      {
        engine.setFrameReady();
        //firstTime = false;
      }
    }
  }

  /**
   *  Kontrollerar om en nod har barn, om så är fallet adderas det första barnet
   *  till noden.
   *
   *@param  tN  Noden vars eventuella barn skall adderas. qqq
   */
  public void addFirstChild(DefaultMutableTreeNode tN)
  {
    Object obj = tN.getUserObject();
    if(obj instanceof XttObj)
    {
      //we must do this for the first node in the tree
      if(firstTime)
      {
        firstTime = false;
        CdhrObjid child = this.gdh.getChild(((XttObj)obj).objId.objid);
        if(child.oddSts())
        {
          //we must know if the node has children
          ((XttObj)obj).hasChildren = true;
          DefaultMutableTreeNode childNode = new DefaultMutableTreeNode(new XttObj("DummyXttObj"));
          tN.add(childNode);
        }
      }
      else if(((XttObj)obj).hasChildren)
      {
        //we must know if the node has children
        //((XttObj)obj).hasChildren = true;
        DefaultMutableTreeNode childNode = new DefaultMutableTreeNode(new XttObj("DummyXttObj"));
        tN.add(childNode);
      }
    }
    else
    {
      Logg.logg("XttTree: addFirstChild anropades med en nod som ej har ett XttObj utan ett "
           + obj.getClass().getName(), 3);
    }
  }

  /**
   *  Raderar alla barn för en nod, skapar sedan nya med annat innehåll
   *
   *@param  treePath  The feature to be added to the ObjectInfo attribute
   */
  public void addObjectInfo(TreePath treePath)
  {
    TreePath tp;
    if(treePath == null)
    {
      tp = this.tree.getSelectionPath();
      if(tp == null)
      {
        return;
      }
    }
    else
    {
      tp = treePath;
    }

    DefaultMutableTreeNode tN = (DefaultMutableTreeNode)(tp.getLastPathComponent());
    if(tN.getUserObject() instanceof XttObj)
    {
      XttObj obj = (XttObj)tN.getUserObject();
      if(obj.hasBeenReferenced)
      {
        return;
      }
      if(obj.debug && this.tree.isExpanded(tp))
      {
        this.handleTreeCollapsed(tp);
      }
      tN.removeAllChildren();
      Logg.logg("XttTree: Barnen borttagna", 6);

      Logg.logg("XttTree: Före getObjectAttributes", 6);
      PwrtObjid pwrobjid = new PwrtObjid(0, 0);

      Vector v = (Vector)gdh.getAllClassAttributes(obj.classId.classId, pwrobjid);
      Logg.logg("XttTree: Efter getAllClassAttributes", 6);
      if(v == null)
      {
        Logg.logg("XttTree: Vector == null efter getAllClassAttributes", 4);
        return;
      }
      Vector xttObjAttrVec = new Vector();
      for(int i = 0; i < v.size(); i++)
      {
        CdhrObjAttr cdhrobjattr = (CdhrObjAttr)v.get(i);
        XttObjAttr xttObjAttr = new XttObjAttr(cdhrobjattr);
        xttObjAttrVec.add(xttObjAttr);
        DefaultMutableTreeNode objAttrTreeNode = new DefaultMutableTreeNode(xttObjAttr);
        xttObjAttr.treeNode = objAttrTreeNode;
        tN.add(objAttrTreeNode);
      }
      obj.addAttrVector(xttObjAttrVec);
      Logg.logg("XttTree: Efter getObjectAttributes", 6);
      treeModel.reload(tN);
      this.tree.expandPath(tp);
    }
  }

  /**
   *  Ändrar värde på markerat objekt till valueString Om det ej går att ändra
   *  värde anropas Logg.loggToApplet() med felmeddelande.
   *
   *@param  valueString  Önskat värde i strängform
   */
  public void changeValue(String valueString)
  {
    Logg.logg("XttTree: Användare vill ändra värde", 6);

    TreePath tp = this.tree.getSelectionPath();
    //inget objekt är markerat
    if(tp == null)
    {
      Logg.loggToApplet(errMess[1]);
      return;
    }
    DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
    //inget objekt är markerat
    if(tn == null)
    {
      Logg.loggToApplet(errMess[1]);
      return;
    }
    Object userObject = tn.getUserObject();
    String name = null;
    int type = 0;

    if(userObject instanceof XttObjAttr)
    {
      XttObjAttr obj = (XttObjAttr)userObject;
      name = obj.fullName;
      type = obj.type;
    }
    else if(userObject instanceof XttArrayAttr)
    {
      XttArrayAttr obj = (XttArrayAttr)userObject;
      name = obj.fullName;
      type = obj.type;
    }
    //Markerat objekt kan ej ändra värde
    if(name == null)
    {
      Logg.loggToApplet(errMess[2]);
      return;
    }
    Logg.logg("XttTree: Användren vill ändra värde på " + name, 6);
    PwrtStatus sts = new PwrtStatus(1);
    switch (type)
    {
      case Pwr.eType_Float32:
      case Pwr.eType_Float64:
        Logg.logg("XttTree.changeValue: innan setObjectInfo(string, float);", 6);
        try
        {
          Float f = new Float(valueString);
          sts = gdh.setObjectInfo(name, f.floatValue());
        }
        catch(NumberFormatException e)
        {
          Logg.loggToApplet(valueString + errMess[3] + " float");
          return;
        }

        break;
      case Pwr.eType_Int8:
      case Pwr.eType_Int16:
      case Pwr.eType_Int32:
      case Pwr.eType_Int64:
      case Pwr.eType_UInt8:
      case Pwr.eType_UInt16:
      case Pwr.eType_UInt32:
      case Pwr.eType_UInt64:
      case Pwr.eType_Status:
      case Pwr.eType_NetStatus:
      case Pwr.eType_Enum:
      case Pwr.eType_Mask:
        Logg.logg("XttTree.changeValue: innan setObjectInfo(string, int);", 6);
        try
        {
          Integer i = new Integer(valueString);
          sts = gdh.setObjectInfo(name, i.intValue());
        }
        catch(NumberFormatException e)
        {
          Logg.loggToApplet(valueString + errMess[3] + " integer");
          return;
        }

        break;
      case Pwr.eType_Boolean:
        Logg.logg("XttTree.changeValue: innan setObjectInfo(string, boolean);", 6);
        try
        {
          Integer i = new Integer(valueString);
          if(i.intValue() == 1)
          {
            sts = gdh.setObjectInfo(name, true);
            //Logg.logg("XttTree.changeValue(" + valueString + ");" + "gav true", 6);
          }
          else if(i.intValue() == 0)
          {
            sts = gdh.setObjectInfo(name, false);
            //Logg.logg("XttTree.changeValue(" + valueString + ");" + "gav false", 6);
          }
          else
          {
            Logg.loggToApplet(valueString + errMess[3] + " boolean");
            return;
          }
        }
        catch(NumberFormatException e)
        {
          Logg.loggToApplet(valueString + errMess[3] + " boolean");
          return;
        }
        break;
      //the rest is treated like strings...
//        case Pwr.eType_String:
//          obj.setValue(en.gdh.getObjectRefInfoString(obj.refObj.id, obj.type));
//          break;
      default:
        Logg.logg("XttTree.changeValue: innan setObjectInfo(string, string);", 6);
        //qqq borde kolla så att det gick bra
        sts = gdh.setObjectInfo(name, valueString);
        Logg.logg("efter setObjectInfo(string, string)", 4);
        break;
    }
    if(sts.evenSts())
    {
      Logg.loggToApplet("Felstatus vid försök att ändra värde på " + name + "till " + valueString);
    }
  }

  /**
   *  Anropas då en nod, oavsett typ, skall stängas. Kontrollerar om noden är
   *  expanderad och i så fall stänger då noden. Om noden ej är expanderad så
   *  stängs föräldernoden.
   */
  public void collapseNode()
  {
    TreePath tpc = tree.getSelectionPath();
    if(tpc == null)
    {
      return;
    }
    TreePath tp = tpc.getParentPath();

    //om vi har en nod vars barn är är synliga
    if(!treeModel.isLeaf(tpc.getLastPathComponent()) && tree.isExpanded(tpc))
    {
      Logg.logg("XttTree: Förälderkollapsering " + tpc.toString(), 6);
      tree.collapsePath(tpc);
      tree.setSelectionPath(tpc);
      tree.scrollPathToVisible(tpc);
    }
    //vi är ej på rootnivån
    else if(tp.getPathCount() > 1)
    {
      tree.collapsePath(tp);
      tree.setSelectionPath(tp);
      tree.scrollPathToVisible(tp);
    }
  }

  /**
   *  Skapar ett XttObj genom några Gdh-anrop
   *
   *@param  c  CdhrObjid för det objektet som skall skapa XttObj.
   *@return    Det nya XttObj som skapats
   */
  public XttObj createXttObj(CdhrObjid c)
  {
    CdhrClassId clId = this.gdh.getObjectClass(c.objid);
    if(clId.evenSts())
    {
      Logg.logg("XttTree: Gdh returnerar fel vid getObjectClass, objekt: " + c.toString(), 3);
    }
    CdhrObjid classObj = this.gdh.classIdToObjid(clId.classId);
    if(classObj.evenSts())
    {
      Logg.logg("XttTree: Gdh returnerar fel vid classIdToObjId, objekt: " + c.toString(), 3);
    }
    String className = this.gdh.objidToName(classObj.objid, Cdh.mName_object).str;
    return new XttObj(this.gdh, this.engine, this.gdh.objidToName(c.objid, Cdh.mName_pathStrict).str,
        this.gdh.objidToName(c.objid, Cdh.mName_object).str, className, c, clId);
  }

  /**
   *  Anropas då användaren vill göra debug på en hierarki. Sätter debug-flaggan
   *  i XttObj-objektet som är userObject is treePath
   *
   *@param  treePath  Hierarki som skall "debuggas"
   */
  public void debugNode(TreePath treePath)
  {
    TreePath tp;
    if(treePath == null)
    {
      tp = this.tree.getSelectionPath();
      if(tp == null)
      {
        return;
      }
    }
    else
    {
      tp = treePath;
    }

    DefaultMutableTreeNode tN = (DefaultMutableTreeNode)(tp.getLastPathComponent());
    if(tN.getUserObject() instanceof XttObj)
    {
      XttObj obj = (XttObj)tN.getUserObject();
      obj.debug = true;
      this.tree.expandPath(tp);
    }
  }

//----------------code for searching the tree------------------------------------------------------- x 
  public void find(PwrtObjid objid)
  {
    String str = this.gdh.objidToName(objid, Cdh.mName_pathStrict).str;
    if(str != null)
      this.find(str);
    TreePath selectedPath = this.tree.getSelectionPath();
    System.out.println(selectedPath);
    if(selectedPath != null)
      this.tree.scrollPathToVisible(selectedPath);
  }
  
  public void find(String userStr)
  {
    this.findPathAndSetExpanded(userStr);
  }
            
  public void findPathAndSetExpanded(String objectName)
  {
    //save selected path so that we can set it selected if we don't find
    //what we are looking for
    TreePath selectedPath = this.tree.getSelectionPath();
    
    Vector pathVec = createPathVec(objectName);
    int ret = 1;
    DefaultMutableTreeNode tn = this.rootNode;
    TreePath tp;
    for(int i = 0;i < pathVec.size();i++)
    {
      tp = expandPath(tn,(String)pathVec.get(i));
      if(tp == null)
        break;
      tn = (DefaultMutableTreeNode)tp.getLastPathComponent();
    }
    if(tn == null)
    {
      Logg.loggToApplet("Kunde ej hitta:" + objectName);
    }      
  }

  public Vector createPathVec(String name)
  {
    Vector pathVec = new Vector();
    int strIndex;
    int i = 0;
    strIndex = name.indexOf('-');
    if(strIndex < 0)
    {
      strIndex = name.indexOf('.');
    }
    if(strIndex < 0)
    {
      pathVec.add(name);
      return pathVec;
    }
    while(strIndex >= 0)
    {
      pathVec.add(name.substring(0, strIndex));
      i = name.indexOf('-', strIndex+1);
      if(i < 0)
      {
        i = name.indexOf('.', strIndex+1);
      }
      if(i < 0)
      {
        pathVec.add(name);
      }
      strIndex = i;
    }
    return pathVec;
  }

  public TreePath expandPath(DefaultMutableTreeNode treeNode, String pathName)
  {
    int nrOfChildren = treeNode.getChildCount();
    DefaultMutableTreeNode tn;
    TreeObj obj;
    String str;
    for(int i = 0;i < nrOfChildren;i++)
    {
      tn = (DefaultMutableTreeNode)treeNode.getChildAt(i);
      try
      {
        obj = (TreeObj)tn.getUserObject();
      }
      catch(Exception e)
      {return null;}
      //cut the name before #(when the searched object is
      //an attribute ex rt-rtt-Dv1.ActualValue##Boolean)
      int ind = obj.fullName.indexOf('#');
      if(ind > 0)
      {
        str = obj.fullName.substring(0,ind);
      }
      else
        str = obj.fullName;
      System.out.println(str);
      if(str.compareToIgnoreCase(pathName) == 0)
      {
        
        System.out.println("Hittat sökväg");
	TreePath tp = new TreePath(tn.getPath());
//	if(tn.isLeaf())
//	{
//	  addObjectInfo(tp);
//	}
//	else
	  this.tree.expandPath(tp);
	this.tree.setSelectionPath(tp);
	return tp;
      }
    }
    return null;
  }
//----------------end code for searching the tree-------------------------------------------------------


  /**
   *  Description of the Method
   *
   *@param  tp  Description of the Parameter
   */
  public void handleTreeCollapsed(TreePath tp)
  {

    DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
    DefaultMutableTreeNode tc;
    try
    {
      tc = (DefaultMutableTreeNode)tn.getFirstChild();
    }
    catch(NoSuchElementException e)
    {
      tc = new DefaultMutableTreeNode("Dummy");
    }
    if((tn != null) &&
        (tree.getSelectionPath() != null))
    {

      Logg.logg("XttTree: Har kollapsat", 6);
      if(tn.getUserObject() instanceof XttObj && !(tc.getUserObject() instanceof XttObj))
      {
        Logg.logg("XttTree: Tar bort attrVector", 6);
        ((XttObj)tn.getUserObject()).removeAttrVector();
        ((XttObj)tn.getUserObject()).debug = false;
        tn.removeAllChildren();
        Logg.logg("XttTree: Barnen borttagna", 6);
        addFirstChild(tn);
        treeModel.reload(tn);
      }

      else if(tn.getUserObject() instanceof XttObj && (tc.getUserObject() instanceof XttObj))
      {
        Logg.logg("XttTree: XttObj-barnen borttagna", 6);
        XttObj obj = ((XttObj)tn.getUserObject());
        if(obj.debug)
        {
          Vector unref_vec = new Vector();
          obj.debug = false;
          Enumeration enum = tn.children();
          while(enum.hasMoreElements())
          {
            DefaultMutableTreeNode child = (DefaultMutableTreeNode)enum.nextElement();
            XttObj childObj = (XttObj)child.getUserObject();
            if(childObj.refObj != null)
            {
              unref_vec.add(childObj.refObj.getPwrtRefId());
              engine.remove(childObj.refObj);
	    }
            
          }
          engine.gdh.unrefObjectInfo_Vector(unref_vec);
        }
        tn.removeAllChildren();
        addFirstChild(tn);
        treeModel.reload(tn);
      }
    }
  }

  AbstractAction ADDOBJECTINFO = new AbstractAction("ADDOBJECTINFO")
                                 {
                                   public void actionPerformed(ActionEvent evt)
                                   {
                                     Logg.logg("XttTree: innan addObjectInfo();", 4);
                                     addObjectInfo((TreePath)null);
                                   }
                                 };

  AbstractAction COLLAPSENODE = new AbstractAction("COLLAPSENODE")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
                                    Logg.logg("XttTree: innan collapseNode();", 4);
                                    collapseNode();
                                  }
                                };

  AbstractAction CHANGEVALUE =  new AbstractAction("CHANGEVALUE")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
                                    Logg.logg("XttTree: innan changeValue();", 4);
                                    changeValue();
                                  }
                                };
  AbstractAction OPENPLC =      new AbstractAction("OPENPLC")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
                                    Logg.logg("XttTree: innan openPLC();", 4);
                                    openPLC();
                                  }
                                };

  AbstractAction SHOWCROSS =    new AbstractAction("SHOWCROSS")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
                                    Logg.logg("XttTree: innan showCross();", 4);
                                    showCross();
                                  }
                                };

  AbstractAction DEBUG =        new AbstractAction("DEBUG")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
                                    Logg.logg("XttTree: innan debug();", 0);
                                    debugNode((TreePath)null);
                                  }
                                };

  AbstractAction INCLOG =       new AbstractAction("INCLOG")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
                                    Logg.loggPrio++;
                                    Logg.logg("LoggPrio changed to: " + Logg.loggPrio, 0);
                                  }
                                };
  AbstractAction DECLOG =       new AbstractAction("DECLOG")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
				    if(Logg.loggPrio > 0);
                                      Logg.loggPrio--;
                                    Logg.logg("LoggPrio changed to: " + Logg.loggPrio, 0);
                                  }
                                };
  AbstractAction LAN_SW =       new AbstractAction("LAN_SW")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
				    setLanguageSwedish();
                                  }
                                };
  AbstractAction LAN_EN =       new AbstractAction("LAN_EN")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
				    setLanguageEnglish();
                                  }
                                };
  AbstractAction FIND =         new AbstractAction("FIND")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
				    find();
                                  }
                                };

    //Mats förändringar: Ny AbstractAction för enterComm
  AbstractAction COMM =         new AbstractAction("COMM")
                                {
                                  public void actionPerformed(ActionEvent evt)
                                  {
				    enterComm();
                                  }
                                };



  /**
   *  Creates menuitem and keyboardbinding for a "method"
   *
   *@param  name           The string that is to be shown in the menu's
   *@param  action         The action who should be associated with the method
   *@param  actionName     The name of the action
   *@param  toPopup        If true the method will be added to the popupmenu 
   *@param  toMenu         The index of the menu where the method will be, if -1 the method won't be added to any menu
   *@param  keyStroke      A string representing the key-combination that is to be associated with the method
   *@return                Void
   */

    // Mats förändringar: booelan toPopup borttagen.
    public void newMethod(String name, Action action, String actionName, /*boolean toPopup,*/ int toMenu, String keyStroke)
  {
    if(action != null)
    {
      actionMap.put(actionName, action);
    }
    if(keyStroke != null)
    {
      inputMap.put(KeyStroke.getKeyStroke(keyStroke), actionName);
    }
    /*
    if(toPopup)
    {
      popup.add(menuItem(name, action, null));
    }
    */
    if(toMenu >= 0)
    {
      menubar.getMenu(toMenu).add(menuItem(name, action, keyStroke));
    }
  }
  

  /**
  * Creates all input posibillites that the user can use
  */
  public void createUserInputs()
  {

    JMenu edit = new JMenu(functions);
    edit.setMnemonic('F');
    JMenu languageSel = new JMenu(language);
    languageSel.setMnemonic('S');

    // Skapa en menylist och lägg till ovan skapade "panes" i denna.
    menubar.add(edit);
    menubar.add(languageSel);
    
    this.getRootPane().setJMenuBar(menubar);
    
    //Mats förändringar: boolean  toPopup borttagen ny metod för enterComm
    // Create some keystrokes and bind them to an action
    this.newMethod(openObject, ADDOBJECTINFO, "ADDOBJECTINFO",/* true,*/ 0, "ctrl A");    this.newMethod(openObject, ADDOBJECTINFO, "ADDOBJECTINFO", /*false,*/ -1, "shift RIGHT");
    this.newMethod("COLLAPSENODE", COLLAPSENODE, "COLLAPSENODE",/* false,*/ -1, "LEFT");
    this.newMethod(openPlc, OPENPLC, "OPENPLC", /*true,*/ 0, "ctrl L");
    this.newMethod(showCross, SHOWCROSS, "SHOWCROSS", /*true,*/ 0, "ctrl R");
    this.newMethod(changeValue, CHANGEVALUE, "CHANGEVALUE", /*true,*/ 0, "ctrl Q");
    this.newMethod(debug, DEBUG, "DEBUG", /*true,*/ 0, "ctrl RIGHT");
    this.newMethod(find, FIND, "FIND",/* false,*/ 0, "ctrl F");
    this.newMethod(swedish, LAN_SW, "LAN_SW",/* false,*/ 1, null);
    this.newMethod(english, LAN_EN, "LAN_EN", /*false,*/ 1, null);
    this.newMethod("INCLOG", INCLOG, "INCLOG", /*false,*/ -1, "ctrl O");
    this.newMethod("DECLOG", DECLOG, "DECLOG", /*false,*/ -1, "ctrl P");
    this.newMethod(enterComm, COMM,"COMM",0,"ctrl B");

    inputMap.setParent(this.tree.getInputMap(JComponent.WHEN_FOCUSED));
    this.tree.setInputMap(JComponent.WHEN_FOCUSED, inputMap);

    actionMap.setParent(this.tree.getActionMap());
    this.tree.setActionMap(actionMap);
    
    
    // Ordna så att snabbmenyn visas då man klickar i huvudfönstret.
    //och så att SHIFT + musklick ger "öppna objekt" 
    this.tree.addMouseListener(
      new MouseAdapter()
      {
        public void mousePressed(MouseEvent e)
        {
          int selRow = tree.getRowForLocation(e.getX(), e.getY());
          TreePath selPath = tree.getPathForLocation(e.getX(), e.getY());
          if(selRow != -1)
          {
            if(e.getClickCount() == 1 && e.isShiftDown())
            {
              //TreePath oldSelectedPath = tree.getSelectionPath();
              selPath = tree.getPathForRow(selRow);
              tree.setSelectionPath(selPath);
              addObjectInfo(selPath);
            }
            else if(e.getClickCount() == 1 && e.isControlDown())
            {
              selPath = tree.getPathForRow(selRow);
              tree.setSelectionPath(selPath);
              debugNode(selPath);
            }
            else if(e.getClickCount() == 1)
            {
              selPath = tree.getPathForRow(selRow);
              tree.setSelectionPath(selPath);
            }
            if(e.isPopupTrigger())
            {
              System.out.println("mouse pressed isPopUpTrigger");
	      //Mats förändringar: popup borttagen, JopMethodsMenu tillagd.
	      TreePath tp = tree.getSelectionPath();
	      if(tp == null) return;
	      DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
	      if(tn == null) return;
	      try {
		TreeObj obj = (TreeObj)tn.getUserObject();
		String name = obj.fullName;

		if(name == null)
		  return;

		// Remove any type suffix
		String str;
		int idx1 = name.indexOf('#');
		if ( idx1 != -1) {
		  int idx2 = name.indexOf('[');
		  if ( idx2 != -1)
		    str = name.substring( 0, idx1) + name.substring(idx2);
		  else
		    str = name.substring( 0, idx1);
		}
		else
		  str = name;

		new JopMethodsMenu( session, str, 
				    JopUtility.NAVIGATOR, (Component) tree, 
				    e.getX(), e.getY());
		      
	      }
	      catch(Exception ex) {
		 Logg.logg("Error in showCross() " + ex.toString(),0);
	      }
	      Logg.loggToApplet("");
              //popup.show((Component)e.getSource(), e.getX(), e.getY());
            }
          }
        }


        public void mouseReleased(MouseEvent e)
        {
          //System.out.println("MusKlick " + e.getModifiers() + " " + MouseEvent.BUTTON3_MASK);
          // Kontrollera om detta är rätt typ av händelse för att visa en snabbmeny

          if(e.isPopupTrigger())
          {
            System.out.println("isPopUpTrigger");
	    //Mats förändringar: popup borttagen, JopMethodsMenu tillagd.
	    TreePath tp = tree.getSelectionPath();
	    if(tp == null) return;
	    DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
	    if(tn == null) return;
	    try {
	      TreeObj obj = (TreeObj)tn.getUserObject();
	      String name = obj.fullName;
	      if(name == null)
		return;

	      new JopMethodsMenu( session, name, 
				  JopUtility.NAVIGATOR,(Component) tree, 
				  e.getX(), e.getY());
	      
	    }
	    catch(Exception ex) {
	      Logg.logg("Error in showCross() " + ex.toString(),0);
	    }
	    Logg.loggToApplet("");
	    // popup.show((Component)e.getSource(), e.getX(), e.getY());
          }
        }
      });
  }

  // En hjälpmethod för att skapa menyobjekt.
  /**
   *  Description of the Method
   *
   *@param  label           Description of the Parameter
   *@param  listener        Description of the Parameter
   *@param  command         Description of the Parameter
   *@param  mnemonic        Description of the Parameter
   *@param  acceleratorKey  Description of the Parameter
   *@return                 Description of the Return Value
   */
  public static JMenuItem menuItem(String label,
      Action action,
      String keyStroke)
  {
    JMenuItem item = new JMenuItem(label);
    item.setAction(action);
    item.setText(label);
    if(keyStroke != null)
    {
      item.setAccelerator(KeyStroke.getKeyStroke(keyStroke));
    }
    return item;
  }

  public void showCross()
  {
    TreePath tp = tree.getSelectionPath();
    if(tp == null) return;
    DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
    if(tn == null) return;
    try
    {
      TreeObj obj = (TreeObj)tn.getUserObject();
      String name = obj.fullName;
      if(name == null)
      {
        Logg.loggToApplet("Inte rätt typ av objekt markerat");
        return;
      }
      session.openCrrFrame(name);
    }
    catch(Exception e)
    {
      Logg.logg("Error in showCross() " + e.toString(),0);
    }
  }

  
  public void openPLC()
  {
    TreePath tp = tree.getSelectionPath();
    DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
    GetPLCWindowObjRet ret = getPLCWindowObj(tn);
    if(ret.oid == null)
    {
      Logg.loggToApplet("Inte rätt typ av objekt markerat");
      return;
    }
    session.openFlowFrame(ret.oid, ret.name);
  }

    public PwrtObjid getPLCWindowObjid(DefaultMutableTreeNode tn)
    {
      DefaultMutableTreeNode tc = null;
      PwrtObjid oid = null;
      XttObj obj;
      try
      {
         obj = (XttObj)tn.getUserObject();
      }
      catch(ClassCastException e)
      {
        return oid;
      }
      switch(obj.classId.classId)
      {
        case Pwrb.cClass_windowplc:
	  oid = obj.objId.objid;
	  break;
	case Pwrb.cClass_plc:
	  try
	  {
	    CdhrObjid gdhr = gdh.getChild(obj.objId.objid);
	    if(gdhr.oddSts())
	    {
	      oid = gdhr.objid;
	    }
	  }
	  catch(NoSuchElementException e)
	  {
	  }
	  break;
	default:
	  DefaultMutableTreeNode tParent = (DefaultMutableTreeNode)tn.getParent();
	  if(tParent != null) 
	    return getPLCWindowObjid(tParent);
	  break;
      }
      return oid;
    }
    
    public String getPLCWindowObjName(DefaultMutableTreeNode tn)
    {
      XttObj obj;
      String ret = null;
      try
      {
         obj = (XttObj)tn.getUserObject();
	 ret = obj.name;
	 System.out.println(ret);
      }
      catch(ClassCastException e)
      {
      }
      return ret;
    }

    public GetPLCWindowObjRet getPLCWindowObj(DefaultMutableTreeNode tn)
    {
      DefaultMutableTreeNode tc = null;
      GetPLCWindowObjRet ret = new GetPLCWindowObjRet();      
      XttObj obj;
      try
      {
         obj = (XttObj)tn.getUserObject();
      }
      catch(ClassCastException e)
      {
        return ret;
      }
      switch(obj.classId.classId)
      {
        case Pwrb.cClass_windowplc:
	case Pwrb.cClass_plc:
	  ret.oid = getPLCWindowObjid(tn);
	  break;
	default:
            ret.oid = getPLCWindowObjid(tn);
	    ret.name = getPLCWindowObjName(tn);
	  break;
      }
      return ret;
    }
    
    private class GetPLCWindowObjRet
    {
      String name = null;
      PwrtObjid oid = null;
    }
  
  public void setLanguageSwedish()
  {
    this.functions = functions_SW;
    this.language = language_SW;
    this.openObject = openObject_SW;
    this.changeValue = changeValue_SW;
    this.debug = debug_SW;
    this.errMess = errMess_SW;
    this.openPlc = openPlc_SW;
    this.find = find_SW;
    this.showCross = showCross_SW;
    this.enterComm = enterComm_SW;
    this.currentLanguage = SWEDISH;
    this.updateMenuLabels();
  }

  public void setLanguageEnglish()
  {
    this.functions = functions_EN;
    this.language = language_EN;
    this.openObject = openObject_EN;
    this.changeValue = changeValue_EN;
    this.debug = debug_EN;
    this.errMess = errMess_EN;
    this.openPlc = openPlc_EN;
    this.find = find_EN;
    this.showCross = showCross_EN;
    this.enterComm = enterComm_EN;
    this.currentLanguage = ENGLISH;
    this.updateMenuLabels();
  }
  
  public void updateMenuLabels()
  {
    JMenuBar menuBar = this.getRootPane().getJMenuBar();

    JMenu menuFunctions = menuBar.getMenu(0);
    JMenu menuLan = menuBar.getMenu(1);
    menuFunctions.setText(functions);
    menuFunctions.getItem(0).setText(openObject);
    menuFunctions.getItem(1).setText(openPlc);
    menuFunctions.getItem(2).setText(showCross);
    menuFunctions.getItem(3).setText(changeValue);
    menuFunctions.getItem(4).setText(debug);
    menuFunctions.getItem(5).setText(find);
    menuFunctions.getItem(6).setText(enterComm);
    menuLan.setText(language);
    menuLan.getItem(0).setText(swedish);
    menuLan.getItem(1).setText(english);
    //Mats förändringar: Uppdatering av popup borttagen
    /*MenuElement[] menuElements = popup.getSubElements();
	
    ((JMenuItem)(menuElements[0])).setText(openObject);
    ((JMenuItem)(menuElements[1])).setText(openPlc);
    ((JMenuItem)(menuElements[2])).setText(showCross);
    ((JMenuItem)(menuElements[3])).setText(changeValue);
    ((JMenuItem)(menuElements[4])).setText(debug);*/
  }



  public void changeValue()
  {
    if(this.engine.gdh.isAuthorized(Pwr.mPrv_RtWrite | Pwr.mPrv_System))
    {
      Logg.loggToApplet(" ");
      Logg.logg("JopXttApplet: changeValue()", 6);
      userValue.setText(null);
      this.tree.setRequestFocusEnabled(false);
      if (enterFieldEnable){
	  enterFieldEnable = false;
	  this.messagePanel.remove(userCommandLabel);
      }
      else this.messagePanel.remove(labelMessage);
      this.messagePanel.add(this.userValueLabel, BorderLayout.WEST);
      this.messagePanel.add(this.userValue, BorderLayout.CENTER);
      messagePanel.doLayout();
      messagePanel.repaint();

      this.userValue.requestFocus();
    }
    else
    {
      Logg.loggToApplet("Användare har ej skrivrättigheter");
    }
  }

  public void find()
  {
      Logg.loggToApplet(" ");
      Logg.logg("JopXttApplet: find()", 6);
      userValue.setText(null);
      this.tree.setRequestFocusEnabled(false);
      //Mats förändringar: Hantering av om enterFieldEnable =true
      if (enterFieldEnable){
	  enterFieldEnable = false;
	  this.messagePanel.remove(userCommandLabel);
      }
      else this.messagePanel.remove(labelMessage);
      this.messagePanel.add(this.userValueLabel, BorderLayout.WEST);
      this.messagePanel.add(this.userValue, BorderLayout.CENTER);
      messagePanel.doLayout();
      messagePanel.repaint();

      this.userValue.requestFocus();
      this.findFieldEnable = true;
  }
    //Mats förändringar: Ny metod enterComm för att hantera manuellt inskrivna kommandon.
 public void enterComm()
  {
      Logg.loggToApplet(" ");
      Logg.logg("JopXttApplet: enterComm()", 6);
      userValue.setText(null);
      this.tree.setRequestFocusEnabled(false);
      if (userValueLabel.isVisible()){
	  findFieldEnable = false;
	  this.messagePanel.remove(userValueLabel);
      }
      else this.messagePanel.remove(labelMessage);
      this.messagePanel.add(this.userCommandLabel, BorderLayout.WEST);
      this.messagePanel.add(this.userValue, BorderLayout.CENTER);
      messagePanel.doLayout();
      messagePanel.repaint();

      this.userValue.requestFocus();
      this.enterFieldEnable = true;
  }
    //Mats förändringar: ny metod som exekverar kommandot com.
    public void enterComm(String com){
	session.executeCommand(com);
    }


  /**
   *  Description of the Class
   *
   *@author     JN3920
   *@created    November 12, 2002
   */
  class MyTreeExpansionListener implements TreeExpansionListener
  {
    /**
     *  Description of the Method
     *
     *@param  e  Description of the Parameter
     */
    public void treeCollapsed(TreeExpansionEvent e)
    {
      TreePath tp = e.getPath();
      handleTreeCollapsed(tp);
    }


    /**
     *  Description of the Method
     *
     *@param  e  Description of the Parameter
     */
    public void treeExpanded(TreeExpansionEvent e)
    {
      //pga bugg som jag ej kan lösa för tillfället
      try
      {
        TreePath tp = e.getPath();
        if(tree.getSelectionPath() == null)
        {
          tree.setSelectionPath(tp);
        }
        DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
        DefaultMutableTreeNode tc = (DefaultMutableTreeNode)tn.getFirstChild();
        if((tn != null) &&
            (tc != null) && tc.getUserObject() instanceof XttObj)
        {
          XttObj obj = (XttObj)tc.getUserObject();
          //XttObj parentObj = (XttObj) tn.getUserObject();
          if(obj.name.compareTo("DummyXttObj") == 0)
          {
            Logg.logg("XttTree: innan collapsePath() pga DummyXttObj", 1);
            tree.collapsePath(tp);
            treeModel.reload(tn);
          }
        }
      }
      catch(NoSuchElementException exception)
      {
      }
    }
  }


  /**
   *  Description of the Class
   *
   *@author     JN3920
   *@created    November 12, 2002
   */
  class MyTreeWillExpandListener implements TreeWillExpandListener
  {

    /**
     *  Ser till att alla grenar under den kollapserande noden också
     *  kollapseras.
     *
     *@param  e  Description of the Parameter
     */
    public void treeWillCollapse(TreeExpansionEvent e)
    {
      TreePath tp = e.getPath();
      if(tp != null)
      {
        int row = tree.getRowForPath(tp);
        row++;
        TreePath descendant = tree.getPathForRow(row);
        while(tp.isDescendant(descendant))
        {
          tree.collapsePath(descendant);
          DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(descendant.getLastPathComponent());
          Object obj = tn.getUserObject();
          if(obj instanceof XttObj)
          {
            ((XttObj)(obj)).removeAttrVector();
          }
          row++;
          descendant = tree.getPathForRow(row);
          //just in case...
          if(row > 50000)
          {
            break;
          }
        }
      }
    }


    /**
     *  Description of the Method
     *
     *@param  e  Description of the Parameter
     */
    public void treeWillExpand(TreeExpansionEvent e)
    {
      Logg.logg("XttTree.treeWillExpand", 7);
      //qqq bör läggas i en egen metod
      TreePath tp = e.getPath();
      if(tree.hasBeenExpanded(tp))
      {
        Logg.logg("XttTree: Har varit expanderad fel!!", 1);
        tree.collapsePath(tp);
        return;
      }
      Cursor defCursor = getCursor();
      setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

      DefaultMutableTreeNode tn = (DefaultMutableTreeNode)(tp.getLastPathComponent());
      DefaultMutableTreeNode tc = (DefaultMutableTreeNode)tn.getFirstChild();
      if((tn != null) && (tree.getSelectionPath() != null) &&
          (tc != null) && !tree.hasBeenExpanded(tp) && tc.getUserObject() instanceof XttObj )
      {
        XttObj obj = (XttObj)tc.getUserObject();
        Logg.logg("XttTree: Skall expanderas", 6);
        addChildren(tn, tc);
        Logg.logg("XttTree: Expandering klar", 6);
      }
      else if((tn != null) && (tree.getSelectionPath() != null)
           && tn.getUserObject() instanceof XttObjAttr)
      {
        String s = ((XttArrayAttr)(tc.getUserObject())).name;
        tn.remove(tc);
        GdhrRefObjectInfo ret;
        XttObjAttr obj = (XttObjAttr)tn.getUserObject();
        Vector ref_vec = new Vector();
        Vector ret_vec;
        Vector attr_vec = new Vector();
        //qqq borde läggas i egen tråd???
        for(int j = 0; j < obj.elements; j++)
        {
          XttArrayAttr arrayAttr = new XttArrayAttr(obj.name + "[" + j + "]", s + "#" + obj.elements + "[" + j + "]", obj.type,
              obj.elements);
          DefaultMutableTreeNode arrayChildNode = new DefaultMutableTreeNode(arrayAttr);
          arrayAttr.treeNode = arrayChildNode;
          arrayAttr.treeModel = obj.treeModel;
          obj.treeNode.add(arrayChildNode);
          Logg.logg("XttObj: arrayAttr.fullName= " + arrayAttr.fullName, 8);
          String str = arrayAttr.fullName;
          attr_vec.add(arrayAttr);
          ref_vec.add(str);

        }
        Logg.logg("refObjectInfo_Vector()", 6);
        ret_vec = gdh.refObjectInfo_Vector(ref_vec);
        if(ret_vec == null)
        {
          Logg.logg("XttTree : refObjectInfo_Vector returnerar null", 1);
          Logg.loggToApplet("XttTree : refObjectInfo_Vector returnerar null INTE BRA programmet kan ha havererat");
          setCursor(defCursor);
	  return;
        }
        for(int i = 0; i < attr_vec.size() && i < ret_vec.size(); i++)
        {
          ret = (GdhrRefObjectInfo)ret_vec.get(i);
          if(ret.evenSts())
          {
            Logg.logg("XttObj:  ObjectInfoError ", 4);
          }
          ((XttArrayAttr)(attr_vec.get(i))).refObj = ret;
        }
      }
      setCursor(defCursor);
    }
  }

  /**
   *  Description of the Class
   *
   *@author     JN3920
   *@created    November 12, 2002
   */
  private class MyRenderer extends DefaultTreeCellRenderer
  {
    /**  Description of the Field */
    ImageIcon ClosedLeafIcon;

    /**  Description of the Field */
    ImageIcon ClosedNodeIcon;
    /**  Description of the Field */
    ImageIcon ObjAttrArrayElementIcon;

    /**  Description of the Field */
    ImageIcon ObjAttrArrayIcon;
    /**  Description of the Field */
    ImageIcon ObjAttrIcon;
    /**  Description of the Field */
    ImageIcon ObjAttrOpenNodeIcon;
    /**  Description of the Field */
    ImageIcon ObjAttrPointerIcon;
    /**  Description of the Field */
    ImageIcon OpenLeafIcon;
    /**  Description of the Field */
    ImageIcon OpenNodeIcon;


    /**  Constructor for the MyRenderer object */
    public MyRenderer()
    {
        ClosedLeafIcon = new ImageIcon(XttClosedLeafIcon);
        OpenLeafIcon = new ImageIcon(XttOpenLeafIcon);

        ClosedNodeIcon = new ImageIcon(XttClosedNodeIcon);
        OpenNodeIcon = new ImageIcon(XttOpenNodeIcon);
        ObjAttrOpenNodeIcon = new ImageIcon(XttObjAttrOpenNodeIcon);

        ObjAttrIcon = new ImageIcon(XttObjAttrIcon);
        ObjAttrPointerIcon = new ImageIcon(XttObjAttrPointerIcon);

        ObjAttrArrayIcon = new ImageIcon(XttObjAttrArrayIcon);
        ObjAttrArrayElementIcon = new ImageIcon(XttObjAttrArrayElementIcon);
    }


    /**
     *  Gets the treeCellRendererComponent attribute of the MyRenderer object
     *
     *@param  tree      Description of the Parameter
     *@param  value     Description of the Parameter
     *@param  sel       Description of the Parameter
     *@param  expanded  Description of the Parameter
     *@param  leaf      Description of the Parameter
     *@param  row       Description of the Parameter
     *@param  hasFocus  Description of the Parameter
     *@return           The treeCellRendererComponent value
     */
    public Component getTreeCellRendererComponent(
        JTree tree,
        Object value,
        boolean sel,
        boolean expanded,
        boolean leaf,
        int row,
        boolean hasFocus)
    {

      super.getTreeCellRendererComponent(
          tree, value, sel,
          expanded, leaf, row,
          hasFocus);

      DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;
      Object userObject = node.getUserObject();

      if(userObject instanceof XttObj)
      {
        if(leaf)
        {
          setIcon(ClosedLeafIcon);
        }
        else if(((XttObj)(userObject)).hasBeenReferenced &&
            !((XttObj)(userObject)).hasChildren)
        {
          setIcon(OpenLeafIcon);
        }
        else if(((XttObj)(userObject)).hasBeenReferenced &&
            ((XttObj)(userObject)).hasChildren)
        {
          setIcon(ObjAttrOpenNodeIcon);
        }
        else if(expanded)
        {
          setIcon(OpenNodeIcon);
        }
        else
        {
          setIcon(ClosedNodeIcon);
        }
      }
      else if(userObject instanceof XttObjAttr)
      {
        int flags = ((XttObjAttr)(userObject)).flags;
        int type = ((XttObjAttr)(userObject)).type;
        if((flags & Pwr.mAdef_array) > 0)
        {
          setIcon(ObjAttrArrayIcon);
        }
        //qqq ändra flgga så att actual value visas rätt
        else if(((flags & (Pwr.mAdef_rtdbref | Pwr.mAdef_devbodyref)) > 0) || type == Pwr.eType_Objid)
        {
          setIcon(ObjAttrPointerIcon);
        }
        else
        {
          setIcon(ObjAttrIcon);
        }
      }
      else if(userObject instanceof XttArrayAttr)
      {
        setIcon(ObjAttrArrayElementIcon);
      }
      //just in case....
      else if(!leaf)
      {
        setIcon(ClosedNodeIcon);
      }
      else
      {
        setIcon(ClosedLeafIcon);
      }
      return this;
    }
  }

  public Object getRoot() {
    return root;
  }
}


















