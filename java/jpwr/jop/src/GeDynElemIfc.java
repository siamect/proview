package jpwr.jop;
import jpwr.rt.*;
import java.awt.event.*;

public interface GeDynElemIfc {
    public int getDynType();
    public int getActionType();
    public void connect();
    public void disconnect();
    public void scan();
    public void action( int eventType, MouseEvent e);
}
