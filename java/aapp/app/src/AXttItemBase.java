package jpwr.app;
import jpwr.rt.*;

public class AXttItemBase {
	PlowNode node;
	
	public AXttItemBase() {}
	public boolean hasChildren() { return false;}
	public void openChildren(AXtt axtt) {}
	public void openAttributes(AXtt axtt) {}
	public void openCrossreferences(AXtt axtt) {}
	public void scan(AXtt axtt) {}
	public void close(AXtt axtt) {}
}
