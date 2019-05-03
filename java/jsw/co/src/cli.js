"use strict";

function CliTable(command, qualifier) {
  this.command = command;
  this.qualifier = qualifier;
}

var CliC = {
  SUCCESS: 1,
  SYNTAX_ERROR: 2,
  UNKNOWN_COMMAND: 4,
  QUALNOTFOUND: 6,
  VERB_VECT_SIZE: 5,

  STATE_INIT: 0,
  STATE_VERB: 1,
  STATE_QUAL: 2,
  STATE_QUALVALUE: 3,
  STATE_SPACE: 4,
  STATE_EQUAL: 5,
  STATE_ERROR: 6,
  STATE_QUOTE_VERB: 7,
  STATE_QUOTE_QUALVALUE: 8,
  STATE_QUALVALUE_EXACT: 9,
  STATE_VERB_EXACT: 10,
  TAB: '	',
  SPACE: ' '
};

function Cli(cliTable) {
  this.verb = new Array(CliC.VERB_VECT_SIZE);
  this.verbCount = 0;
  this.qualifier = new Array(30);
  this.qualValue = new Array(30);
  this.qualifierCount = 0;
  this.status;
  this.cliTableIndex;
  this.cliQualifierIndex = new Array(30);
  this.configuredVerbs;
  this.cliTable = cliTable;

  /**
   * Return the status of the last operation as a string.
   * @return The status of the last operation.
   */
  this.getStsString = function () {
    switch (this.status) {
      case CliC.SUCCESS:
        return "Success";
      case CliC.SYNTAX_ERROR:
        return "Syntax error";
      case CliC.UNKNOWN_COMMAND:
        return "Unknown command verb";
      case CliC.QUALNOTFOUND:
        return "Unknown qualifier";
      default:
        return "Unknown command interpreter error";
    }
  };
  /**
   * Check if status of last operation is even. An error or warning
   * will result in an even status.
   * @return Returns true if status of last operation is even.
   */
  this.evenSts = function () {
    return (this.status % 2 === 0);
  };

  /**
   * Check if status of last operation is odd. A success or information
   * will result in an odd status.
   * @return Returns true if status of last operation is odd.
   */
  this.oddSts = function () {
    return (this.status % 2 !== 0);
  };

  /**
   * Parse a command line and detect verbs and qualifiers.
   * @param cmd Command line.
   */
  this.parse = function (cmd) {

    // Parse command string

    var state = CliC.STATE_INIT;
    var start_pos = 0;
    this.status = CliC.SUCCESS;
    this.verbCount = 0;
    this.qualifierCount = 0;
    var i;
    for (i = 0; i < cmd.length; i++) {
      var c = cmd.charAt(i);
      switch (state) {
        case CliC.STATE_INIT:
          if (c == CliC.SPACE || c == CliC.TAB)
            break;
          else {
            state = CliC.STATE_VERB;
            start_pos = i;
          }
          break;
        case CliC.STATE_SPACE:
          if (c == CliC.SPACE || c == CliC.TAB)
            break;
          if (c == '/') {
            state = CliC.STATE_QUAL;
            start_pos = i;
          }
          else if (c == '=') {
            if (this.qualifierCount === 0) {
              state = CliC.STATE_ERROR;
              this.status = CliC.SYNTAX_ERROR;
              break;
            }
            state = CliC.STATE_EQUAL;
          }
          else if (c == '"') {
            state = CliC.STATE_QUOTE_VERB;
            break;
          }
          else {
            state = CliC.STATE_VERB;
            start_pos = i;
          }
          break;
        case CliC.STATE_VERB:
          if (c == CliC.SPACE || c == CliC.TAB) {
            if (this.verbCount == CliC.VERB_VECT_SIZE) {
              state = CliC.STATE_ERROR;
              break;
            }
            if (this.verbCount === 0)
              this.verb[this.verbCount++] = cmd.substring(start_pos, i).toUpperCase();
            else
              this.verb[this.verbCount++] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          }
          else if (c == '/') {
            if (this.verbCount == CliC.VERB_VECT_SIZE) {
              state = CliC.STATE_ERROR;
              break;
            }
            this.verb[this.verbCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_QUAL;
            start_pos = i;
          }
          break;
        case CliC.STATE_VERB_EXACT:
          if (c == '"') {
            if (this.verbCount == CliC.VERB_VECT_SIZE) {
              state = CliC.STATE_ERROR;
              break;
            }
            this.verb[this.verbCount++] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          }
          break;
        case CliC.STATE_QUAL:
          if (c == CliC.SPACE || c == CliC.TAB) {
            this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_SPACE;
          }
          else if (c == '=') {
            this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_EQUAL;
          }
          else if (c == '/') {
            this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
            state = CliC.STATE_QUAL;
            start_pos = i;
          }
          break;
        case CliC.STATE_QUALVALUE:
          if (c == CliC.SPACE || c == CliC.TAB) {
            this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          }
          else if (c == '/') {
            this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
            state = CliC.STATE_QUAL;
            start_pos = i;
          }
          break;
        case CliC.STATE_QUALVALUE_EXACT:
          if (c == '"') {
            this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
            state = CliC.STATE_SPACE;
          }
          break;
        case CliC.STATE_QUOTE_VERB:
          state = CliC.STATE_VERB_EXACT;
          start_pos = i;
          break;
        case CliC.STATE_QUOTE_QUALVALUE:
          state = CliC.STATE_QUALVALUE_EXACT;
          start_pos = i;
          break;
        case CliC.STATE_EQUAL:
          if (c == CliC.SPACE || c == CliC.TAB)
            break;
          if (c == '"') {
            state = CliC.STATE_QUOTE_QUALVALUE;
          }
          else {
            state = CliC.STATE_QUALVALUE;
            start_pos = i;
          }
          break;

      }
      if (state == CliC.STATE_ERROR)
        break;
    }
    switch (state) {
      case CliC.STATE_INIT:
      case CliC.STATE_ERROR:
        return "";
      case CliC.STATE_VERB:
        if (this.verbCount == CliC.VERB_VECT_SIZE) {
          state = CliC.STATE_ERROR;
          break;
        }
        if (this.verbCount === 0)
          this.verb[this.verbCount++] = cmd.substring(start_pos, i).toUpperCase();
        else
          this.verb[this.verbCount++] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_VERB_EXACT:
        if (this.verbCount == CliC.VERB_VECT_SIZE) {
          state = CliC.STATE_ERROR;
          break;
        }
        this.verb[this.verbCount++] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_QUAL:
        this.qualifier[this.qualifierCount++] = cmd.substring(start_pos, i).toUpperCase();
        break;
      case CliC.STATE_QUALVALUE:
        this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_QUALVALUE_EXACT:
        this.qualValue[this.qualifierCount - 1] = cmd.substring(start_pos, i);
        break;
      case CliC.STATE_QUOTE_VERB:
      case CliC.STATE_QUOTE_QUALVALUE:
      case CliC.STATE_EQUAL:
        this.status = CliC.SYNTAX_ERROR;
        return "";
    }

    if (this.verbCount === 0) {
      this.status = CliC.SYNTAX_ERROR;
      return "";
    }

    // for ( i = 0; i < this.verbCount; i++)
    //   console.log("verb: \"" + this.verb[i] + "\"");
    // for ( i = 0; i < this.qualifierCount; i++)
    //  console.log("qual: \"" + this.qualifier[i] + "\" , \"" + this.qualValue[i] + "\"");

    // Identify verbs and qualifiers
    var found = false;
    for (i = 0; i < this.cliTable.length; i++) {
      if (this.verb[0].length > this.cliTable[i].command.length)
        continue;
      if (this.verb[0] == (this.cliTable[i].command.substring(0, this.verb[0].length))) {
        this.verb[0] = this.cliTable[i].command;
        found = true;
        break;
      }
    }
    if (!found) {
      this.status = CliC.UNKNOWN_COMMAND;
      return "";
    }
    this.cliTableIndex = i;

    this.configuredVerbs = 0;
    if (this.cliTable[this.cliTableIndex].qualifier !== null) {
      for (i = 0; i < this.cliTable[this.cliTableIndex].qualifier.length; i++) {
        if (this.cliTable[this.cliTableIndex].qualifier[i] === null)
          break;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg1"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg2"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg3"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg4"))
          this.configuredVerbs++;
        if (this.cliTable[this.cliTableIndex].qualifier[i] == ("cli_arg5"))
          this.configuredVerbs++;
      }

      for (var j = 0; j < this.qualifierCount; j++) {
        found = false;
        for (i = 0; i < this.cliTable[this.cliTableIndex].qualifier.length; i++) {
          if (this.cliTable[this.cliTableIndex].qualifier[i] === null)
            break;
          if (this.qualifier[j].length > this.cliTable[this.cliTableIndex].qualifier[i].length)
            continue;
          if (this.qualifier[j] == (this.cliTable[this.cliTableIndex].qualifier[i].substring(0, this.qualifier[j].length))) {
            this.cliQualifierIndex[j] = i;
            found = true;
            this.qualifier[j] = this.cliTable[this.cliTableIndex].qualifier[i];
          }
        }
        if (!found) {
          this.status = CliC.QUALNOTFOUND;
          return "";
        }
      }
    }
    else if (this.qualifierCount > 0) {
      this.status = CliC.QUALNOTFOUND;
      return "";
    }
    return this.verb[0];
  };

  /**
   * Check if a qualifier was present in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns true if the qualifier is present.
   */
  this.qualifierFound = function (qual) {

    if (qual == ("cli_arg1")) {
      if (this.verbCount < 2 || this.configuredVerbs < 1)
        return false;
      return true;
    }
    if (qual == ("cli_arg2")) {
      if (this.verbCount < 3 || this.configuredVerbs < 2)
        return false;
      return true;
    }
    if (qual == ("cli_arg3")) {
      if (this.verbCount < 4 || this.configuredVerbs < 3)
        return false;
      return true;
    }
    if (qual == ("cli_arg4")) {
      if (this.verbCount < 5 || this.configuredVerbs < 4)
        return false;
      return true;
    }
    for (var i = 0; i < this.qualifierCount; i++) {
      if (qual == (this.qualifier[i]))
        return true;
    }
    return false;
  };

  /**
   * Get the value of a qualifier in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns the value of the qualifier.
   */
  this.getQualValue = function (qual) {
    if (qual == ("cli_arg1")) {
      if (this.verbCount < 2 || this.configuredVerbs < 1)
        return "";
      return this.verb[1];
    }
    if (qual == ("cli_arg2")) {
      if (this.verbCount < 3 || this.configuredVerbs < 2)
        return "";
      return this.verb[2];
    }
    if (qual == ("cli_arg3")) {
      if (this.verbCount < 4 || this.configuredVerbs < 3)
        return this.verb[3];
    }
    if (qual == ("cli_arg4")) {
      if (this.verbCount < 5 || this.configuredVerbs < 4)
        return "";
      return this.verb[4];
    }
    for (var i = 0; i < this.qualifierCount; i++) {
      if (qual == (this.qualifier[i])) {
        if (this.qualValue[i] === null)
          return "";
        else
          return this.qualValue[i];
      }
    }
    return "";
  };
}