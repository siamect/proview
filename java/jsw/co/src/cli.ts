"use strict";

class CliTable {
  command: string;
  qualifier: Array<string>;

  constructor(command, qualifier) {
    this.command = command;
    this.qualifier = qualifier;
  }
}

enum CliC {
  SUCCESS = 1,
  SYNTAX_ERROR = 2,
  UNKNOWN_COMMAND = 4,
  VERB_VECT_SIZE = 5,
  QUALNOTFOUND = 6
}

enum State {
  INIT,
  VERB,
  QUAL,
  QUALVALUE,
  SPACE,
  EQUAL,
  ERROR,
  QUOTE_VERB,
  QUOTE_QUALVALUE,
  QUALVALUE_EXACT,
  VERB_EXACT
}

class Cli {
  verb: Array = new Array(CliC.VERB_VECT_SIZE);
  qualifier: Array = new Array(30);
  qualValue: Array = new Array(30);
  status: number;
  cliTableIndex: number;
  cliQualifierIndex: Array = new Array(30);
  configuredVerbs: number;
  cliTable: Array<CliTable>;

  constructor(cliTable) {
    this.cliTable = cliTable;
  }

  /**
   * Return the status of the last operation as a string.
   * @return The status of the last operation.
   */
  getStsString() {
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
  }

  /**
   * Parse a command line and detect verbs and qualifiers.
   * @param cmd Command line.
   */
  parse(cmd) {
    let state = State.INIT;
    let start_pos = 0;
    this.status = CliC.SUCCESS;
    let i;
    for (i = 0; i < cmd.length; i++) {
      let c = cmd.charAt(i);
      switch (state) {
        case State.INIT:
          if (c === ' ' || c === '\t') {
            break;
          } else {
            state = State.VERB;
            start_pos = i;
          }
          break;
        case State.SPACE:
          if (c === ' ' || c === '\t') {
            break;
          }
          if (c === '/') {
            state = State.QUAL;
            start_pos = i;
          } else if (c === '=') {
            if (this.qualifier.length === 0) {
              state = State.ERROR;
              this.status = CliC.SYNTAX_ERROR;
              break;
            }
            state = State.EQUAL;
          } else if (c === '"') {
            state = State.QUOTE_VERB;
            break;
          } else {
            state = State.VERB;
            start_pos = i;
          }
          break;
        case State.VERB:
          if (c === ' ' || c === '\t') {
            if (this.verb.length === CliC.VERB_VECT_SIZE) {
              state = State.ERROR;
              break;
            }
            if (this.verb.length === 0) {
              this.verb.push(cmd.substring(start_pos, i).toUpperCase());
            } else {
              this.verb.push(cmd.substring(start_pos, i));
            }
            state = State.SPACE;
          } else if (c === '/') {
            if (this.verb.length === CliC.VERB_VECT_SIZE) {
              state = State.ERROR;
              break;
            }
            this.verb.push(cmd.substring(start_pos, i).toUpperCase());
            state = State.QUAL;
            start_pos = i;
          }
          break;
        case State.VERB_EXACT:
          if (c === '"') {
            if (this.verb.length === CliC.VERB_VECT_SIZE) {
              state = State.ERROR;
              break;
            }
            this.verb.push(cmd.substring(start_pos, i));
            state = State.SPACE;
          }
          break;
        case State.QUAL:
          if (c === ' ' || c === '\t') {
            this.qualifier.push(cmd.substring(start_pos, i).toUpperCase());
            state = State.SPACE;
          } else if (c === '=') {
            this.qualifier.push(cmd.substring(start_pos, i).toUpperCase());
            state = State.EQUAL;
          } else if (c === '/') {
            this.qualifier.push(cmd.substring(start_pos, i).toUpperCase());
            state = State.QUAL;
            start_pos = i;
          }
          break;
        case State.QUALVALUE:
          if (c === ' ' || c === '\t') {
            this.qualValue[this.qualifier.length - 1] =
                cmd.substring(start_pos, i);
            state = State.SPACE;
          } else if (c === '/') {
            this.qualValue[this.qualifier.length - 1] =
                cmd.substring(start_pos, i);
            state = State.QUAL;
            start_pos = i;
          }
          break;
        case State.QUALVALUE_EXACT:
          if (c === '"') {
            this.qualValue[this.qualifier.length - 1] =
                cmd.substring(start_pos, i);
            state = State.SPACE;
          }
          break;
        case State.QUOTE_VERB:
          state = State.VERB_EXACT;
          start_pos = i;
          break;
        case State.QUOTE_QUALVALUE:
          state = State.QUALVALUE_EXACT;
          start_pos = i;
          break;
        case State.EQUAL:
          if (c === ' ' || c === '\t') {
            break;
          }
          if (c === '"') {
            state = State.QUOTE_QUALVALUE;
          } else {
            state = State.QUALVALUE;
            start_pos = i;
          }
          break;

      }
      if (state === State.ERROR) {
        break;
      }
    }
    switch (state) {
      case State.INIT:
      case State.ERROR:
        return "";
      case State.VERB:
        if (this.verb.length === CliC.VERB_VECT_SIZE) {
          state = State.ERROR;
          break;
        }
        if (this.verb.length === 0) {
          this.verb.push(cmd.substring(start_pos, i).toUpperCase());
        } else {
          this.verb.push(cmd.substring(start_pos, i));
        }
        break;
      case State.VERB_EXACT:
        if (this.verb.length === CliC.VERB_VECT_SIZE) {
          state = State.ERROR;
          break;
        }
        this.verb.push(cmd.substring(start_pos, i));
        break;
      case State.QUAL:
        this.qualifier.push(cmd.substring(start_pos, i).toUpperCase());
        break;
      case State.QUALVALUE:
        this.qualValue[this.qualifier.length - 1] = cmd.substring(start_pos, i);
        break;
      case State.QUALVALUE_EXACT:
        this.qualValue[this.qualifier.length - 1] = cmd.substring(start_pos, i);
        break;
      case State.QUOTE_VERB:
      case State.QUOTE_QUALVALUE:
      case State.EQUAL:
        this.status = CliC.SYNTAX_ERROR;
        return "";
    }

    if (this.verb.length === 0) {
      this.status = CliC.SYNTAX_ERROR;
      return "";
    }

    // Identify verbs and qualifiers
    let found = false;
    for (i = 0; i < this.cliTable.length; i++) {
      if (this.verb[0].length > this.cliTable[i].command.length) {
        continue;
      }
      if (this.verb[0] ===
          (this.cliTable[i].command.substring(0, this.verb[0].length))) {
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
        if (this.cliTable[this.cliTableIndex].qualifier[i] === null) {
          break;
        }
        if (this.cliTable[this.cliTableIndex].qualifier[i] === ("cli_arg1")) {
          this.configuredVerbs++;
        }
        if (this.cliTable[this.cliTableIndex].qualifier[i] === ("cli_arg2")) {
          this.configuredVerbs++;
        }
        if (this.cliTable[this.cliTableIndex].qualifier[i] === ("cli_arg3")) {
          this.configuredVerbs++;
        }
        if (this.cliTable[this.cliTableIndex].qualifier[i] === ("cli_arg4")) {
          this.configuredVerbs++;
        }
        if (this.cliTable[this.cliTableIndex].qualifier[i] === ("cli_arg5")) {
          this.configuredVerbs++;
        }
      }

      for (let j = 0; j < this.qualifier.length; j++) {
        found = false;
        for (i = 0; i < this.cliTable[this.cliTableIndex].qualifier.length;
             i++) {
          if (this.cliTable[this.cliTableIndex].qualifier[i] === null) {
            break;
          }
          if (this.qualifier[j].length >
              this.cliTable[this.cliTableIndex].qualifier[i].length) {
            continue;
          }
          if (this.qualifier[j] ===
              (this.cliTable[this.cliTableIndex].qualifier[i].substring(0,
                  this.qualifier[j].length))) {
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
    } else if (this.qualifier.length > 0) {
      this.status = CliC.QUALNOTFOUND;
      return "";
    }
    return this.verb[0];
  }

  /**
   * Check if a qualifier was present in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns true if the qualifier is present.
   */
  qualifierFound(qual) {
    if (qual === ("cli_arg1")) {
      return !(this.verb.length < 2 || this.configuredVerbs < 1);
    }
    if (qual === ("cli_arg2")) {
      return !(this.verb.length < 3 || this.configuredVerbs < 2);
    }
    if (qual === ("cli_arg3")) {
      return !(this.verb.length < 4 || this.configuredVerbs < 3);
    }
    if (qual === ("cli_arg4")) {
      return !(this.verb.length < 5 || this.configuredVerbs < 4);
    }
    for (let i = 0; i < this.qualifier.length; i++) {
      if (qual === (this.qualifier[i])) {
        return true;
      }
    }
    return false;
  }

  /**
   * Get the value of a qualifier in the last parse operation.
   * @param qual Qualifier to check.
   * @return Returns the value of the qualifier.
   */
  getQualValue(qual) {
    if (qual === ("cli_arg1")) {
      if (this.verb.length < 2 || this.configuredVerbs < 1) {
        return "";
      }
      return this.verb[1];
    }
    if (qual === ("cli_arg2")) {
      if (this.verb.length < 3 || this.configuredVerbs < 2) {
        return "";
      }
      return this.verb[2];
    }
    if (qual === ("cli_arg3")) {
      if (this.verb.length < 4 || this.configuredVerbs < 3) {
        return this.verb[3];
      }
    }
    if (qual === ("cli_arg4")) {
      if (this.verb.length < 5 || this.configuredVerbs < 4) {
        return "";
      }
      return this.verb[4];
    }
    for (let i = 0; i < this.qualifier.length; i++) {
      if (qual === (this.qualifier[i])) {
        if (this.qualValue[i] === null) {
          return "";
        } else {
          return this.qualValue[i];
        }
      }
    }
    return "";
  }
}