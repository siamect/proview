package jpwr.jop;
import java.awt.*;

/* The Proportion class is used when using the RatioLayout layoutmanager. It 
   makes sure that the object associated with it  keeps its proportions 
   when the frame is resized. */
public class Proportion {
    
    //The ratios in x,y position and width, height
    public double rx,ry,rw,rh;	
    public Dimension previous;
    public boolean firstDraw =true;

    // constructor. area= the size and place of an object. framesize
    //is the size of the frame in which it is placed.
    public Proportion(Rectangle area, Dimension framesize){
	rx= (double) (1.0*area.x/framesize.width);
	ry= (double) (1.0*area.y/framesize.height);
	rw= (double) (1.0*area.width/framesize.width);
	rh= (double) (1.0*area.height/framesize.height);
	previous= new Dimension(1,1);
    }

    // For debuging purposes...
    public String toString(){
	return ""+rx+" "+ry+" "+rw+" "+rh+" "+previous+" "+firstDraw;
    }
}
