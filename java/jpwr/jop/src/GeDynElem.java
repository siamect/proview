package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;

public class GeDynElem implements GeDynElemIfc {
    public GeDyn dyn;
    public int dynType;
    public int actionType;

    public GeDynElem( GeDyn dyn, int dynType, int actionType) {
	this.dyn = dyn;
	this.dynType = dynType;
	this.actionType = actionType;
    }
    public int getDynType() {
	return dynType;
    }
    public int getActionType() {
	return actionType;
    }
    public void connect() {
    }
    public void disconnect() {
    }
    public void scan() {
    }
    public void action( int eventType, MouseEvent e) {
    }
}

