package jpwr.jop;

public interface JopDynamic {
  public void dynamicUpdate( boolean animationOnly);
  public void dynamicOpen();
  public void dynamicClose();
  public Object dynamicGetRoot();
}
