package jpwr.rt;

public class CliTable {
  String command;
  String[] qualifier;
  
  public CliTable( String command, String[] qualifier) {
    this.command = command;
    this.qualifier = qualifier;
  }
}
