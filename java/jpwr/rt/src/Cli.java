/**
 * Title:	<p>
 * Description:	<p>
 * Copyright:	<p>
 * Company	SSAB<p>
 * @author	Claes Sjofors
 * @version	1.0
 */

package jpwr.rt;
import java.io.*;

/**
 * Command line interpreter. Parses a command line and detects
 * verbs and qualifiers described in a CliTable class.
 */

public class Cli {
  public static final int SUCCESS = 1;
  public static final int SYNTAX_ERROR = 2;
  public static final int UNKNOWN_COMMAND = 4;
  public static final int QUALNOTFOUND = 6;
  public static final int VERB_VECT_SIZE = 5;
  
  static final int STATE_INIT = 0;
  static final int STATE_VERB = 1;
  static final int STATE_QUAL = 2;
  static final int STATE_QUALVALUE = 3;
  static final int STATE_SPACE = 4;
  static final int STATE_EQUAL = 5;
  static final int STATE_ERROR = 6;
  static final int STATE_QUOTE_VERB = 7;
  static final int STATE_QUOTE_QUALVALUE = 8;
  static final int STATE_QUALVALUE_EXACT = 9;
  static final int STATE_VERB_EXACT = 10;
  static final char TAB = '	';
  static final char SPACE = ' ';
  String[] verb = new String[VERB_VECT_SIZE];
  int verbCount = 0;
  String[] qualifier = new String[30];
  String[] qualValue = new String[30];
  int qualifierCount = 0; 
  int status;
  int cliTableIndex;
  int cliQualifierIndex[] = new int[30];
  int configuredVerbs;
  CliTable[] cliTable;

  /**
   * Create a Cli object.
   * @param cliTable Table that describes the verbs with qualifiers.
   */
  public Cli( CliTable[] cliTable) {
    this.cliTable = cliTable;
  }
   
  /**
   * Return the status of the last operation as a string.
   * @return The status of the last operation.
   */
  public String getStsString() {
    switch ( status) {
      case SUCCESS:		return "Success";
      case SYNTAX_ERROR:	return "Syntax error";
      case UNKNOWN_COMMAND:	return "Unknown command verb";
      case QUALNOTFOUND:	return "Unknown qualifier";
      default:			return "Unknown command interpreter error";
    }
  }
  /**
   * Check if status of last operation is even. An error or warning
   * will result in an even status.
   * @return Returns true if status of last operation is even.
   */
  public boolean evenSts() {
    return (status % 2 == 0);
  }

  /**
   * Check if status of last operation is odd. A success or information
   * will result in an odd status.
   * @return Returns true if status of last operation is odd.
   */
  public boolean oddSts() {
    return (status % 2 != 0);
  }

  /**
   * Parse a command line and detect verbs and qualifiers.
   * @param cmd Command line.
   */
  public String parse( String cmd) {
  
    // Parse command string
    
    int state = STATE_INIT;
    int start_pos = 0;
    status = SUCCESS;
    verbCount = 0;
    qualifierCount = 0;
    int i;
    for ( i = 0; i < cmd.length(); i++) {
      char c = cmd.charAt(i);
      switch ( state) {
        case STATE_INIT:
	  if ( c == SPACE || c == TAB)
	    break;
	  else {
	    state = STATE_VERB;
	    start_pos = i;
          }
	  break;
	case STATE_SPACE:
	  if ( c == SPACE || c == TAB)
	    break;
	  if ( c == '/') {
	    state = STATE_QUAL;
	    start_pos = i;
	  }
	  else if ( c == '=') {
	    if ( qualifierCount == 0) {
	      state = STATE_ERROR;
	      status = SYNTAX_ERROR;
              break;
	    }
	    state = STATE_EQUAL;
          }
	  else if ( c == '"') {
	    state = STATE_QUOTE_VERB;
            break;
          }
	  else {
	    state = STATE_VERB;
	    start_pos = i;
	  }
	  break;
	case STATE_VERB:
	  if ( c == SPACE || c == TAB) {
	    if ( verbCount == VERB_VECT_SIZE) {
	      state = STATE_ERROR;
	      break;
	    }
            if ( verbCount == 0)
              verb[verbCount++] = cmd.substring( start_pos, i).toUpperCase();
            else
              verb[verbCount++] = cmd.substring( start_pos, i);
            state = STATE_SPACE;
	  }
	  else if ( c == '/') {
	    if ( verbCount == VERB_VECT_SIZE) {
	      state = STATE_ERROR;
	      break;
	    }
            verb[verbCount++] = cmd.substring( start_pos, i).toUpperCase();
	    state = STATE_QUAL;
	    start_pos = i;
	  }
	  break;
	case STATE_VERB_EXACT:
	  if ( c == '"') {
	    if ( verbCount == VERB_VECT_SIZE) {
	      state = STATE_ERROR;
	      break;
	    }
            verb[verbCount++] = cmd.substring( start_pos, i);
            state = STATE_SPACE;
	  }
	  break;
	case STATE_QUAL:
	  if ( c == SPACE || c == TAB) {
            qualifier[qualifierCount++] = cmd.substring( start_pos, i).toUpperCase();
            state = STATE_SPACE;
	  }
	  else if ( c == '=') {
            qualifier[qualifierCount++] = cmd.substring( start_pos, i).toUpperCase();
            state = STATE_EQUAL;
	  }
	  else if ( c == '/') {
            qualifier[qualifierCount++] = cmd.substring( start_pos, i).toUpperCase();
	    state = STATE_QUAL;
	    start_pos = i;
	  }
	  break;
	case STATE_QUALVALUE:
	  if ( c == SPACE || c == TAB) {
            qualValue[qualifierCount-1] = cmd.substring( start_pos, i);
            state = STATE_SPACE;
	  }
	  else if ( c == '/') {
            qualValue[qualifierCount-1] = cmd.substring( start_pos, i);
	    state = STATE_QUAL;
	    start_pos = i;
	  }
	  break;
	case STATE_QUALVALUE_EXACT:
	  if ( c == '"') {
            qualValue[qualifierCount-1] = cmd.substring( start_pos, i);
	    state = STATE_SPACE;
	  }
	  break;
	case STATE_QUOTE_VERB:
	  state = STATE_VERB_EXACT;
	  start_pos = i;
	  break;
	case STATE_QUOTE_QUALVALUE:
	  state = STATE_QUALVALUE_EXACT;
	  start_pos = i;
	  break;
	case STATE_EQUAL:
	  if ( c == SPACE || c == TAB)
	    break;
          if ( c == '"') {
	    state = STATE_QUOTE_QUALVALUE;
	  }
	  else {
            state = STATE_QUALVALUE;
	    start_pos = i;
	  }
	  break;

      }	      
      if ( state == STATE_ERROR)
        break;
    }
    switch ( state) {
      case STATE_INIT:
      case STATE_ERROR:
        return "";
      case STATE_VERB:
	if ( verbCount == VERB_VECT_SIZE) {
	  state = STATE_ERROR;
          break;
	}
        if ( verbCount == 0)
          verb[verbCount++] = cmd.substring( start_pos, i).toUpperCase();
        else
          verb[verbCount++] = cmd.substring( start_pos, i);
	break;
      case STATE_VERB_EXACT:
	if ( verbCount == VERB_VECT_SIZE) {
	  state = STATE_ERROR;
          break;
	}
        verb[verbCount++] = cmd.substring( start_pos, i);
	break;
      case STATE_QUAL:
        qualifier[qualifierCount++] = cmd.substring( start_pos, i).toUpperCase();
	break;
      case STATE_QUALVALUE:
        qualValue[qualifierCount-1] = cmd.substring( start_pos, i);
	break;
      case STATE_QUALVALUE_EXACT:
        qualValue[qualifierCount-1] = cmd.substring( start_pos, i);
	break;
      case STATE_QUOTE_VERB:
      case STATE_QUOTE_QUALVALUE:
      case STATE_EQUAL:
        status = SYNTAX_ERROR;
	return "";
    }

    if ( verbCount == 0) {
      status = SYNTAX_ERROR;
      return "";
    }
    
    // for ( i = 0; i < verbCount; i++)
    //   System.out.println("verb: \"" + verb[i] + "\"");
    // for ( i = 0; i < qualifierCount; i++)
    //  System.out.println("qual: \"" + qualifier[i] + "\" , \"" + qualValue[i] + "\"");

    // Identify verbs and qualifiers
    boolean found = false;
    for ( i = 0; i < cliTable.length; i++) {
      if ( verb[0].length() > cliTable[i].command.length())
        continue;
      if ( verb[0].equals( cliTable[i].command.substring( 0, verb[0].length()))) {
//        System.out.println("Verb found: " + cliTable[i].command);
	verb[0] = cliTable[i].command;
        found = true;
        break;
      }
    }
    if ( !found) {
      status = UNKNOWN_COMMAND;
      return "";
    }
    cliTableIndex = i;

    configuredVerbs = 0;
    if ( cliTable[cliTableIndex].qualifier != null)
    {
      for ( i = 0; i < cliTable[cliTableIndex].qualifier.length; i++) {
        if ( cliTable[cliTableIndex].qualifier[i] == null)
          break;
        if ( cliTable[cliTableIndex].qualifier[i].equals("cli_arg1"))
          configuredVerbs++;
        if ( cliTable[cliTableIndex].qualifier[i].equals("cli_arg2"))
          configuredVerbs++;
        if ( cliTable[cliTableIndex].qualifier[i].equals("cli_arg3"))
          configuredVerbs++;
        if ( cliTable[cliTableIndex].qualifier[i].equals("cli_arg4"))
          configuredVerbs++;
       if ( cliTable[cliTableIndex].qualifier[i].equals("cli_arg5"))
          configuredVerbs++;
      }

      for ( int j = 0; j < qualifierCount; j++) {
        found = false;
        for ( i = 0; i < cliTable[cliTableIndex].qualifier.length; i++) {
          if ( cliTable[cliTableIndex].qualifier[i] == null)
  	  break;
	  if ( qualifier[j].length() > cliTable[cliTableIndex].qualifier[i].length())
            continue;
          if ( qualifier[j].equals( cliTable[cliTableIndex].qualifier[i].substring( 0, qualifier[j].length()))) {
//            System.out.println("Qualifier found: " + cliTable[cliTableIndex].qualifier[i]);
            cliQualifierIndex[j] = i;
            found = true;
	    qualifier[j] = cliTable[cliTableIndex].qualifier[i];
	  }
        }
        if ( !found) {
          status = QUALNOTFOUND;
	  return "";
        }
      }
    }
    else if ( qualifierCount > 0) {
      status = QUALNOTFOUND;
      return "";
    }
    return verb[0];
  }

  /**
   * Check if a qualifier was present in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns true if the qualifier is present.
   */
  public boolean qualifierFound( String qual) {

    if ( qual.equals("cli_arg1")) {
      if ( verbCount < 2 || configuredVerbs < 1)
        return false;
      return true;
    }
    if ( qual.equals("cli_arg2")) {
      if ( verbCount < 3 || configuredVerbs < 2)
        return false;
      return true;
    }
    if ( qual.equals("cli_arg3")) {
      if ( verbCount < 4 || configuredVerbs < 3)
        return false;
      return true;
    }
    if ( qual.equals("cli_arg4")) {
      if ( verbCount < 5 || configuredVerbs < 4)
        return false;
      return true;
    }
    for ( int i = 0; i < qualifierCount; i++) {
      if ( qual.equals(qualifier[i]))
        return true;
    }
    return false;
  }

  /**
   * Get the value of a qualifier in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns the value of the qualifier.
   */
  public String getQualValue( String qual) {
    if ( qual.equals("cli_arg1")) {
      if ( verbCount < 2 || configuredVerbs < 1)
        return "";
      return verb[1];
    }
    if ( qual.equals("cli_arg2")) {
      if ( verbCount < 3 || configuredVerbs < 2)
        return "";
      return verb[2];
    }
    if ( qual.equals("cli_arg3")) {
      if ( verbCount < 4 || configuredVerbs < 3)
      return verb[3];
    }
    if ( qual.equals("cli_arg4")) {
      if ( verbCount < 5 || configuredVerbs < 4)
        return "";
      return verb[4];
    }
    for ( int i = 0; i < qualifierCount; i++) {
      if ( qual.equals(qualifier[i])) {
        if ( qualValue[i] == null)
	  return "";
	else
          return qualValue[i];
      }
    }
    return "";
  }
}
