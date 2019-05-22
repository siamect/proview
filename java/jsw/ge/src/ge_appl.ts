let cliTable = [new CliTable("OPEN",
    ["cli_arg1", "cli_arg2", "/NAME", "/FILE", "/SCROLLBAR", "/WIDTH", "/HEIGHT",
      "/MENU", "/NAVIGATOR", "/CENTER", "/OBJECT", "/INSTANCE", "/NEW",
      "/CLASSGRAPH", "/ACCESS", "/PARENT"]), new CliTable("EXIT", null),
  new CliTable("HELP",
      ["cli_arg1", "cli_arg2", "cli_arg3", "cli_arg4", "/HELPFILE",
        "/POPNAVIGATOR", "/BOOKMARK", "/INDEX", "/BASE", "/RETURNCOMMAND",
        "/WIDTH", "/HEIGHT", "/VERSION"]), new CliTable("SET",
      ["cli_arg1", "cli_arg2", "/NAME", "/VALUE", "/BYPASS", "/SOURCE",
        "/OBJECT"]), new CliTable("EXAMPLE", ["/NAME", "/HIERARCHY"]),
  new CliTable("CHECK", ["cli_arg1", "/METHOD", "/OBJECT"]),
  new CliTable("CALL", ["cli_arg1", "/METHOD", "/OBJECT"]),
  new CliTable("SET", ["cli_arg1", "dcli_arg2", "/CONTINUE"])];

class Appl {
  graph: Graph;

  constructor() {
    this.graph = new Graph(this);
  }

  command(cmd) {
    console.log("Command: " + cmd);

    if (!this.graph.isAuthorized(Access.AllRt)) {
      console.log("Not authorized");
      return 0;
    }

    if (cmd.charAt(0) === '@') {
      // Execute a script TODO
      // new JopgCcm( this, gdh, cmd.substring(1),  null);
      return 1;
    }

    let cli = new Cli(cliTable);
    let command = cli.parse(cmd);
    if (odd(cli.status)) {
      let getQualIfExists = (qual) => (cli.qualifierFound(qual)) ? cli.getQualValue(qual) : null;

      if (command === ("OPEN")) {
        let cli_arg1 = getQualIfExists("cli_arg1");
        if (cli_arg1) {
          let jgraph = "JGRAPH";
          let graph = "GRAPH";
          let url = "URL";
          let trend = "TREND";
          let fast = "FAST";
          cli_arg1 = cli_arg1.toUpperCase();
          if (jgraph.length >= cli_arg1.length &&
              jgraph.substring(0, cli_arg1.length) === cli_arg1) {
            // Command is "OPEN JGRAPH"
            console.log("Command is not implemented, \"OPEN JGRAPH\"");
          } else if (graph.length >= cli_arg1.length &&
              graph.substring(0, cli_arg1.length) === cli_arg1) {
            // Command is "OPEN GRAPH"
            let objectValue = getQualIfExists("/OBJECT");
            if (objectValue) {
              return 1;
            }

            let instanceValue = getQualIfExists("/INSTANCE");
            let classGraph = false;
            if (instanceValue) {
              classGraph = cli.qualifierFound("/CLASSGRAPH");
              let parent = cli.qualifierFound("/PARENT");
              if (parent) {
                let idx = instanceValue.lastIndexOf('.');
                if (idx !== -1 && idx !== 0) {
                  instanceValue = instanceValue.substring(0, idx);
                }
                console.log("open graph /parent: " + instanceValue);
              }
            }
            if (classGraph) {
              console.log("Cmd classGraph");
              this.graph.gdh.getObjectFromName(instanceValue,
                  GdhOp.GET_OP_METHOD_OBJECTGRAPH).then(this.open_objectgraph_cb);
            } else {
              let graphName = cli.getQualValue("cli_arg2").toLowerCase();
              if (!graphName) {
                console.log("Syntax error");
                return 0;
              }

              graphName = graphName.toLowerCase();
              if (!graphName.endsWith(".pwg")) {
                graphName += ".pwg";
              }

              let href = "ge.html?graph=" + graphName;
              if (instanceValue) {
                href += "&instance=" + instanceValue
              }
              if (this.graph.frame.nogdh) {
                href += "&gdh=no";
              }
              console.log("Cmd found: open graph", graphName, instanceValue);
              let newwindow = window.open(href, "_blank");
            }
          } else if (url.length >= cli_arg1.length &&
              url.substring(0, cli_arg1.length) === cli_arg1) {
            // Command is "OPEN URL"
            let newFrame = true;
            let frameName = null;
            let urlValue = cli.getQualValue("cli_arg2");

            let arrTmp = ["pwrb", "pwrs", "nmps", "profibus", "otherio", "opc",
              "basecomponent", "abb", "siemens", "ssabox"];
            if (arrTmp.some(e => urlValue.startsWith(e + "_"))) { // Object reference manual
              urlValue = "$pwr_doc/en_us/orm/" + urlValue;
            }

            console.log("open url " + urlValue);
            this.openURL(urlValue, null);
          } else if (trend.length >= cli_arg1.length &&
              trend.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "OPEN TREND"
            console.log("Cmd open trend");
          } else if (fast.length >= cli_arg1.length &&
              fast.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "OPEN FAST"
            console.log("Cmd open fast");
          } else if (command === ("HELP")) {
            console.log("Cmd help");
          }
        }
      } else if (command === ("SET")) {
        let cli_arg1 = getQualIfExists("cli_arg1");
        if (cli_arg1) {
          let cli_arg1 = cli_arg1.toUpperCase();
          let subwindow = "SUBWINDOW";
          if (subwindow.length >= cli_arg1.length &&
              subwindow.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "SET SUBWINDOW"
            let name = getQualIfExists("/NAME");
            if (!name) {
              console.log("Cmd: Name is missing\n");
              return 0;
            }
            let source = getQualIfExists("/SOURCE");
            if (!source) {
              console.log("Cmd: source is missing\n");
              return 0;
            }
            let object = getQualIfExists("/OBJECT");
            let graphstr = getQualIfExists("cli_arg2");
            if (graphstr) {
              graphstr = graphstr.toLowerCase();
            } else {
              console.log("Syntax error");
              return 0;
            }

            if (source.indexOf('.') === -1) {
              source += ".pwg";
            }

            this.graph.setSubwindowSource(name, source, object);
          }
        }
      } else if (command === ("HELP")) {
        let fileName = "/pwrp_web/xtt_help_";
        let bookmarkValue = null;

        if (cli.qualifierFound("/VERSION")) {
          fileName = window.location.hostname + "/pwr_doc/xtt_version_help_version.html";
          this.openURL(fileName, null);
        } else {
          if (cli.qualifierFound("/BASE")) { // Not language dependent !! TODO
            fileName = "/pwr_doc/help/xtt_help_";
          }

          for (let i = 0; i < 4; i++) {
            let tmp = getQualIfExists("cli_arg" + String(i+1));
            fileName += tmp ? tmp.toLowerCase() : "";
          }

          let arrTmp = ["pwrb", "pwrs", "nmps", "profibus", "otherio", "opc",
            "basecomponent", "abb", "siemens", "ssabox"];
          if (arrTmp.some(e => fileName.startsWith(e + "_"))) { // Object reference manual
            fileName = "/pwr_doc/orm/" + fileName;
          }

          bookmarkValue = getQualIfExists("/BOOKMARK");

          fileName += ".html";
          console.log("Loading helpfile \"" + fileName + "\"");
          this.openURL(window.location.hostname + fileName, bookmarkValue);
        }
      } else if (command === ("CHECK")) {
        let cli_arg1 = getQualIfExists("cli_arg1");
        if (cli_arg1) {
          let methodstr = "METHOD";
          let isattributestr = "ISATTRIBUTE";
          cli_arg1 = cli_arg1.toUpperCase();
          if (methodstr.length >= cli_arg1.length &&
              methodstr.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "CHECK METHOD"
            let method = getQualIfExists("/METHOD");
            if (!method) {
              console.log("Cmd: Method is missing\n");
              return 0;
            }

            let object = getQualIfExists("/OBJECT");
            if (!object) {
              console.log("Cmd: Object is missing\n");
              return 0;
            }
          } else if (isattributestr.length >= cli_arg1.length &&
              isattributestr.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "CHECK ISATTRIBUTE"
            let object = getQualIfExists("/OBJECT");
            if (!object) {
              console.log("Cmd: Object is missing\n");
              return 0;
            }
          }
        } else if (command === ("CALL")) {
          let cli_arg1 = getQualIfExists("cli_arg1");
          if (cli_arg1) {
            let parameter = "METHOD";
            cli_arg1 = cli_arg1.toUpperCase();
            if (parameter.length >= cli_arg1.length &&
                parameter.substring(0, cli_arg1.length) === (cli_arg1)) {
              // Command is "CHECK METHOD"
              let method = getQualIfExists("/METHOD");
              if (!method) {
                console.log("Cmd: Method is missing\n");
                return 0;
              }

              let object = getQualIfExists("/OBJECT");
              if (!object) {
                console.log("Cmd: Object is missing\n");
                return 0;
              }
            }
          }
        } else if (command === ("SET")) {
          return 1;
        }
      } else {
        console.log("JopSpider: Parse error " + cli.getStsString());
        return 0;
      }
    }
    return 1;
  }

  script(script) {
    console.log("Appl.script", script);
  }

  openURL(url, bookmark) {
    console.log("URL: ", url);
    window.open(url, "_blank");
  }

  openConfirmDialog(dyn, text, object) {
    if (confirm(text)) {
      dyn.confirmedAction(Event.MB1Click, object);
    }
  }

  open_objectgraph_cb(res) {
    let w = window.open("", "_blank");
    if ((res.sts & 1) === 0) {
      w.document.write("Error status " + res.sts);
    } else {
      let result = res.value;
      console.log("param1", result.param1);
      w.location.href =
          "ge.html?graph=" + result.param1 + "&instance=" + result.fullname;
      w.document.title = result.param1 + " " + result.fullname;
    }
  }
}

let appl = new Appl();
