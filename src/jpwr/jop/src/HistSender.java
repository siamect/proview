package jpwr.jop;
import java.awt.*;
import java.net.*;
import java.io.*;
import javax.swing.*;
import java.util.Vector;
import jpwr.rt.*;

/* The HistSender initiates a socket to the server and can then perform a
 *SearchRequest given a HistQuery using an ObjectInput- and an 
 *ObjectOutputStream.*/

public class HistSender {
    
    private Socket socket;
    private ObjectInputStream in; 
    private ObjectOutputStream out;
    Object root;
    final static int PORT = 4447;
    
    public HistSender(Object r){
    root=r;
    setup();
    }
    
    // initiate the socket to the server
    private void setup(){
    
    
    try {
        URL url;
            String urlString = "192.168.60.16";
        try {
        url = ((JApplet)root).getCodeBase();
        if(url != null)
            urlString = url.getHost();
        }
        catch(Exception e)
                {
            System.out.println("Program not run from applet..."+e);
                }
        
        
        socket = new Socket(urlString, PORT);
        }
    
    catch(UnknownHostException e) {
        System.err.println("Don't know about host: taranis.");
        
        //System.exit(1);
    }
    catch(IOException e) {
        JOptionPane.showMessageDialog(null,"Couldn't get contact with the server (HistServer). \n Kunde inte ansluta till servern(HistServer).","I/O Error",JOptionPane.ERROR_MESSAGE);
        
        }
    
    
    }
    
    public MhData SearchRequest(HistQuery search){
    
    
        
    // Open output and input streams.
    try
        { 
                out = new ObjectOutputStream( socket.getOutputStream() );
                out.flush();
                //varför???
                in = new ObjectInputStream( socket.getInputStream() );
        }
        catch(Exception e)
        {
                System.out.println("IOException vid skapande av strömmar mot server");
                //errh.error("DataStream failed");
                return new MhData(100,100);
        }
    
    //Send search request
    
    try
        {
        out.writeObject(search);
        out.flush();
        }
    catch(Exception e)
        {
        System.out.println("Search send Exception");
        }
    
    
    // Start receiving data
    MhData answer = new MhData(0,0);
    
    try
        {
        MhrEvent[] result = (MhrEvent[])in.readObject();
        int i = 0;
        while(i < result.length)
            {
            answer.addMessToVectorInSortedOrder(answer.eventVec, result[i++]);
            }
        }
    catch(Exception e) {
        System.out.println("#"+e.toString());
    }
    
    //Close input and output streams
    try
        {
        in.close();
        out.close();
        }
    catch(Exception e)
        {
        System.out.println("Error while closing streams: " + e);
        }
    
    return answer;    
    }
    
}
