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
    let local_cmd = false;

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
      if (command === ("OPEN")) {
        if (cli.qualifierFound("cli_arg1")) {

          let jgraph = "JGRAPH";
          let graph = "GRAPH";
          let url = "URL";
          let trend = "TREND";
          let fast = "FAST";
          let cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
          if (jgraph.length >= cli_arg1.length &&
              jgraph.substring(0, cli_arg1.length) === cli_arg1) {
            // Command is "OPEN JGRAPH"
            console.log("Command is not implemented, \"OPEN JGRAPH\"");
          } else if (graph.length >= cli_arg1.length &&
              graph.substring(0, cli_arg1.length) === cli_arg1) {
            // Command is "OPEN GRAPH"
            let graphName = null;
            let instanceValue = null;
            let classGraph = false;
            let objectValue = null;

            if (cli.qualifierFound("/OBJECT")) {
              objectValue = cli.getQualValue("/OBJECT");
              return 1;
            }
            if (cli.qualifierFound("/INSTANCE")) {
              instanceValue = cli.getQualValue("/INSTANCE");
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
              let newwindow = window.open("", "_blank");
              this.graph.gdh.getObjectFromName(instanceValue,
                  GdhOp.GET_OP_METHOD_OBJECTGRAPH, this.open_objectgraph_cb,
                  newwindow);
            } else {
              if (!cli.qualifierFound("cli_arg2")) {
                console.log("Syntax error");
                return 0;
              }

              graphName = cli.getQualValue("cli_arg2").toLowerCase();
              if (graphName.charAt(".pwg") === -1) {
                graphName = graphName + ".pwg";
              }

              let href;
              if (this.graph.frame.nogdh) {
                if (instanceValue === null) {
                  href = "ge.html?graph=" + graphName + "&instance=no&gdh=no";
                } else {
                  href = "ge.html?graph=" + graphName + "&instance=" +
                      instanceValue + "&gdh=no";
                }
              } else {
                if (instanceValue === null) {
                  href = "ge.html?graph=" + graphName;
                } else {
                  href =
                      "ge.html?graph=" + graphName + "&instance=" + instanceValue;
                }
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

            if (urlValue.substring(0, 5) === "pwrb_" ||
                urlValue.substring(0, 5) === "pwrs_" ||
                urlValue.substring(0, 5) === "nmps_" ||
                urlValue.substring(0, 9) === "profibus_" ||
                urlValue.substring(0, 8) === "otherio_" ||
                urlValue.substring(0, 4) === "opc_" ||
                urlValue.substring(0, 14) === "basecomponent_" ||
                urlValue.substring(0, 4) === "abb_" ||
                urlValue.substring(0, 8) === "siemens_" ||
                urlValue.substring(0, 7) === "ssabox_")
            // Object reference manual
            {
              urlValue = "$pwr_doc/" + getLang() + "/orm/" + urlValue;
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
        if (cli.qualifierFound("cli_arg1")) {
          let cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
          let subwindow = "SUBWINDOW";
          if (subwindow.length >= cli_arg1.length &&
              subwindow.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "SET SUBWINDOW"

            let name;
            let graphstr;
            let source;
            let object;
            let sts;

            local_cmd = true;
            if (cli.qualifierFound("/NAME")) {
              name = cli.getQualValue("/NAME");
            } else {
              console.log("Cmd: name is missing\n");
              return 0;
            }
            if (cli.qualifierFound("/SOURCE")) {
              source = cli.getQualValue("/SOURCE");
            } else {
              console.log("Cmd: source is missing\n");
              return 0;
            }
            if (cli.qualifierFound("/OBJECT")) {
              object = cli.getQualValue("/OBJECT");
            } else {
              object = null;
            }
            if (cli.qualifierFound("cli_arg2")) {
              graphstr = cli.getQualValue("cli_arg2").toLowerCase();
            } else {
              console.log("Syntax error");
              return 0;
            }

            if (source.indexOf('.') === -1) {
              source = source + ".pwg";
            }

            this.graph.setSubwindowSource(name, source, object);
          }
        }
      } else if (command === ("HELP")) {
        let fileName = "xtt_help_";
        let bookmarkValue = null;

        if (cli.qualifierFound("/VERSION")) {
          fileName = this.pwrHost + "xtt_version_help_version.html";
          this.openURL(fileName, null);
        } else {
          if (cli.qualifierFound("/BASE"))
          // Not language dependent !! TODO
          {
            fileName = this.pwrHost + "help/xtt_help_";
          } else {
            fileName = "xtt_help_";
          }

          if (cli.qualifierFound("cli_arg1")) {
            fileName += cli.getQualValue("cli_arg1").toLowerCase();
          }
          if (cli.qualifierFound("cli_arg2")) {
            fileName += "_" + cli.getQualValue("cli_arg2").toLowerCase();
          }
          if (cli.qualifierFound("cli_arg3")) {
            fileName += "_" + cli.getQualValue("cli_arg3").toLowerCase();
          }
          if (cli.qualifierFound("cli_arg4")) {
            fileName += "_" + cli.getQualValue("cli_arg4").toLowerCase();
          }

          if (fileName.substring(0, 5) === "pwrb_" ||
              fileName.substring(0, 5) === "pwrs_" ||
              fileName.substring(0, 5) === "nmps_" ||
              fileName.substring(0, 9) === "profibus_" ||
              fileName.substring(0, 8) === "otherio_" ||
              fileName.substring(0, 4) === "opc_" ||
              fileName.substring(0, 14) === "basecomponent_" ||
              fileName.substring(0, 4) === "abb_" ||
              fileName.substring(0, 8) === "siemens_" ||
              fileName.substring(0, 7) === "ssabox_")
          // Object reference manual
          {
            fileName = "$pwr_doc/orm/" + fileName;
          }

          if (cli.qualifierFound("/BOOKMARK")) {
            bookmarkValue = cli.getQualValue("/BOOKMARK");
          }

          fileName += ".html";
          console.log("Loading helpfile \"" + fileName + "\"");
          this.openURL(fileName, bookmarkValue);
        }
        local_cmd = true;
      } else if (command === ("CHECK")) {
        if (cli.qualifierFound("cli_arg1")) {

          let methodstr = "METHOD";
          let isattributestr = "ISATTRIBUTE";
          let cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
          if (methodstr.length >= cli_arg1.length &&
              methodstr.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "CHECK METHOD"
            let method;
            let object;

            if (cli.qualifierFound("/METHOD")) {
              method = cli.getQualValue("/METHOD");
            } else {
              console.log("Cmd: Method is missing\n");
              return 0;
            }

            if (cli.qualifierFound("/OBJECT")) {
              object = cli.getQualValue("/OBJECT");
            } else {
              console.log("Cmd: Object is missing\n");
              return 0;
            }
          } else if (isattributestr.length >= cli_arg1.length &&
              isattributestr.substring(0, cli_arg1.length) === (cli_arg1)) {
            // Command is "CHECK ISATTRIBUTE"
            let method;
            let object;

            if (cli.qualifierFound("/OBJECT")) {
              object = cli.getQualValue("/OBJECT");
            } else {
              console.log("Cmd: Object is missing\n");
              return 0;
            }
          }
        } else if (command === ("CALL")) {
          if (cli.qualifierFound("cli_arg1")) {

            let parameter = "METHOD";
            let cli_arg1 = cli.getQualValue("cli_arg1").toUpperCase();
            if (parameter.length >= cli_arg1.length &&
                parameter.substring(0, cli_arg1.length) === (cli_arg1)) {
              // Command is "CHECK METHOD"
              let method;
              let object;

              if (cli.qualifierFound("/METHOD")) {
                method = cli.getQualValue("/METHOD");
              } else {
                console.log("Cmd: Method is missing\n");
                return 0;
              }

              if (cli.qualifierFound("/OBJECT")) {
                object = cli.getQualValue("/OBJECT");
              } else {
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
    let res = confirm(text);
    if (res) {
      dyn.confirmedAction(Event.MB1Click, object);
    }
  }

  open_objectgraph_cb(id, data, sts, result) {
    if ((sts & 1) === 0) {
      data.document.write("Error status " + sts);
    } else {
      console.log("param1", result.param1);
      data.location.href =
          "ge.html?graph=" + result.param1 + "&instance=" + result.fullname;
      data.document.title = result.param1 + " " + result.fullname;
    }
  }
}

let appl = new Appl();
