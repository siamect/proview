package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

public class GeDynPulldownMenu extends GeDynElem {
  String[] itemsText;
  GeDyn[] itemsDyn;

  public GeDynPulldownMenu( GeDyn dyn, String itemsText[], GeDyn[] itemsDyn) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_PulldownMenu);
    this.itemsText = itemsText;
    this.itemsDyn = itemsDyn;
  }
  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      break;
    case GeDyn.eEvent_MB1Up:
      break;
    case GeDyn.eEvent_MB1Click:
      new PulldownMenu((Component)dyn.comp, 
		       // ((Component)dyn.comp).getX() - ((Component)dyn.comp).getWidth()/2, 
		       // ((Component)dyn.comp).getY() + ((Component)dyn.comp).getHeight() - 2
		       0, ((Component)dyn.comp).getHeight() - 2
		       );
      break;
    }
  }

  class PulldownMenu implements ActionListener, PopupMenuListener {
    Component invoker;

    PulldownMenu( Component invoker, int x, int y) {
      this.invoker = invoker;
      JMenuItem item;
      JPopupMenu popup = new JPopupMenu();

      for ( int i = 0; i < 32; i++) {
        if ( itemsText[i] != null && itemsDyn[i] != null) {
	  if ( (itemsDyn[i].actionType & GeDyn.mActionType_PulldownMenu) != 0) {
	      System.out.println( itemsText[i] + " " + i + " " + itemsDyn[i].actionType + " " +
				  GeDyn.mActionType_PulldownMenu);
	    if ( itemsDyn[i].elements.length == 1 &&
		 itemsDyn[i].elements[0] instanceof GeDynPulldownMenu)
	      popup.add( subMenu( itemsText[i], (GeDynPulldownMenu) itemsDyn[i].elements[0])); // Check which element...
	  }
	  else {
	    popup.add( item = new JMenuItem( itemsText[i]));
	    item.addActionListener( this);
	  }
        }
      }
      popup.addPopupMenuListener( this);
      popup.show( invoker, x, y);
    }

    JMenu subMenu( String text, GeDynPulldownMenu pulldown) {
      JMenu menu = new JMenu( text);
      JMenuItem item;

      for ( int i = 0; i < 32; i++) {
        if ( pulldown.itemsText[i] != null && pulldown.itemsDyn[i] != null) {
	  if ( (pulldown.itemsDyn[i].actionType & GeDyn.mActionType_PulldownMenu) != 0) {
	    if ( pulldown.itemsDyn[i].elements.length == 1 &&
		 pulldown.itemsDyn[i].elements[0] instanceof GeDynPulldownMenu)
	      menu.add( subMenu( pulldown.itemsText[i], (GeDynPulldownMenu) pulldown.itemsDyn[i].elements[0]));
	  }
	  else {
	    menu.add( item = new JMenuItem( pulldown.itemsText[i]));
	    item.addActionListener( this);
	  }
        }
      }
      return menu;
    }
    public void popupMenuWillBecomeVisible( PopupMenuEvent e) {}
    public void popupMenuWillBecomeInvisible( PopupMenuEvent e) {}
    public void popupMenuCanceled( PopupMenuEvent e) {}

    public void actionPerformed( ActionEvent event) {

      for ( int i = 0; i < 32; i++) {
        if ( itemsText[i] != null && itemsText[i].equals( event.getActionCommand())) {
	  if ( itemsDyn[i] != null) {
	    MouseEvent e = new MouseEvent( invoker, 0, 0, 0, invoker.getX(), invoker.getY(), 1, false);

	    System.out.println("Action " + event.getActionCommand());
	    itemsDyn[i].action( GeDyn.eEvent_MB1Click, e);
	  }
	  break;
        }
	else if ( itemsDyn[i] != null &&
		  (itemsDyn[i].actionType & GeDyn.mActionType_PulldownMenu) != 0) {
	  if ( itemsDyn[i].elements.length == 1 && 
	       itemsDyn[i].elements[0] instanceof GeDynPulldownMenu)
	    subMenuAction( event, (GeDynPulldownMenu) itemsDyn[i].elements[0]);
	}
      }
    }  

    public void subMenuAction( ActionEvent event, GeDynPulldownMenu pulldown) {
      for ( int i = 0; i < 32; i++) {
        if ( pulldown.itemsText[i] != null && pulldown.itemsText[i].equals( event.getActionCommand())) {
	  if ( pulldown.itemsDyn[i] != null) {
	    MouseEvent e = new MouseEvent( invoker, 0, 0, 0, invoker.getX(), invoker.getY(), 1, false);

	    System.out.println("Action " + event.getActionCommand());
	    pulldown.itemsDyn[i].action( GeDyn.eEvent_MB1Click, e);
	  }
	  break;
        }
	else if ( pulldown.itemsDyn[i] != null && 
		  (pulldown.itemsDyn[i].actionType & GeDyn.mActionType_PulldownMenu) != 0) {
	  if ( pulldown.itemsDyn[i].elements.length == 1)
	    subMenuAction( event, (GeDynPulldownMenu) pulldown.itemsDyn[i].elements[0]);
	}
      }
    }
  }
}













