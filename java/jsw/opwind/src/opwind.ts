"use strict";

class OpWindMenu {
  priv = 0;
  user = "";
  user_text: Text = null;
  host: string;
  gdh: Gdh;
  info: OpwindMenuInfo;

  constructor() {
    this.host = window.location.hostname || "localhost";
    this.gdh = new Gdh(this.gdh_init_cb);
  }

  is_authorized(access) {
    if (!(this.priv & access)) {
      window.alert("Not authorized for this operation");
    }
    return !!(this.priv & access);
  }

  get_opplace() {
    let query = window.location.search.substring(1);

    console.log("query", query);
    let vars = query.split('&');
    console.log("vars", vars.length, vars[0].substring(8));
    return vars[0].substring(8);
  }

  gdh_init_cb() {
    let oid = new PwrtObjid(0, 0);
    this.user = "Default";
    this.gdh.login("", "").then(this.login_cb);
    this.gdh.getOpwindMenu(this.get_opplace()).then(this.get_menu_cb);
  }

  add_menu_button(context, text) {
    let button = document.createElement("input");
    button.type = "button";
    button.className = "leftmenu-button";
    button.value = text;
    button.addEventListener('click', function () {
      menu.button_cb(button.value);
    });
    context.appendChild(button);
    context.appendChild(document.createElement("br"));
    return button;
  }

  get_menu_cb(res) {
    let result = res.value;
    this.info = result;
    let context = document.getElementById("opwindmenu");

    document.getElementById("opwind_title").innerHTML = result.title;
    document.getElementById("opwind_text").innerHTML = result.text;

    if (result.enable_login) {
      this.user_text = document.createTextNode(this.user + " on " + this.host);
      context.appendChild(this.user_text);
      context.appendChild(document.createElement("hr"));

      document.getElementById("login_button")
          .addEventListener("click", function (event) {
            if (document.getElementById("login_frame").style.visibility ===
                'hidden') {
              document.getElementById("login_user").innerHTML = "";
              document.getElementById("login_passw").innerHTML = "";
              document.getElementById("login_frame").style.visibility = 'visible';
              document.getElementById("login_frame").style.height = '120px';
              document.getElementById("login_user").focus();
            } else {
              document.getElementById("login_frame").style.visibility = 'hidden';
              document.getElementById("login_frame").style.height = '0px';
            }
          });
      document.getElementById("apply_button")
          .addEventListener("click", function (event) {
            let user = document.getElementById("login_user").innerHTML;
            let passwd = document.getElementById("login_passw").innerHTML;
            if (user.trim() === "") {
              return;
            }
            document.getElementById("login_frame").style.visibility = 'hidden';
            document.getElementById("login_frame").style.height = '0px';
            let c = new JopCrypt();
            passwd = c.crypt("aa", passwd);

            this.user = user;
            this.gdh.login(user, passwd).then(this.login_cb);
          });
      document.getElementById("cancel_button")
          .addEventListener("click", function (event) {
            document.getElementById("login_frame").style.visibility = 'hidden';
            document.getElementById("login_frame").style.height = '0px';
          });
      document.getElementById("logout_button")
          .addEventListener("click", function (event) {
            document.getElementById("login_frame").style.visibility = 'hidden';
            document.getElementById("login_frame").style.height = '0px';
            this.priv = 0;
            this.user = "Default";
            this.gdh.login("", "").then(this.login_cb);
          });

      document.getElementById("login_user").innerHTML = "";
      document.getElementById("login_passw").innerHTML = "";
      //document.getElementById("login_frame").setAttribute("style", "visibility:hidden;height:10px";
      document.getElementById("login_frame").style.visibility = 'hidden';
      document.getElementById("login_frame").style.height = '0px';
    } else {
      document.getElementById("login_button").remove();
      document.getElementById("login_frame").remove();
    }
    if (result.enable_language) {
      this.add_menu_button(context, "Language");
    }
    if (result.enable_alarmlist) {
      this.add_menu_button(context, "AlarmList");
      this.add_menu_button(context, "EventList");
    }
    if (result.enable_eventlog) {
      this.add_menu_button(context, "EventLog");
    }
    if (result.enable_navigator) {
      this.add_menu_button(context, "Navigator");
    }
    if (!result.disable_help) {
      this.add_menu_button(context, "Help");
    }
    if (!result.disable_proview) {
      this.add_menu_button(context, "ProviewR");
    }

    context.appendChild(document.createElement("hr"));

    result.buttons.forEach(e => this.add_menu_button(context, e.text));
  }

  button_cb(text) {
    if (this.info.enable_language && text === "Language") {
      console.log("Language activated");
    } else if (this.info.enable_alarmlist && text === "AlarmList") {
      console.log("AlarmList activated");
      if (this.is_authorized(Access.RtRead | Access.RtWrite |
              Access.AllOperators | Access.System |
              Access.Maintenance | Access.Process |
              Access.Instrument)) {
        window.open("ev.html?list=alarm", "_blank");
      }
    } else if (this.info.enable_alarmlist && text === "EventList") {
      console.log("EventList activated");
      if (this.is_authorized(Access.RtRead | Access.RtWrite |
              Access.AllOperators | Access.System |
              Access.Maintenance | Access.Process |
              Access.Instrument)) {
        window.open("ev.html?list=event", "_blank");
      }
    } else if (this.info.enable_eventlog && text === "EventLog") {
      console.log("EventLog activated");
      if (this.is_authorized(Access.RtRead | Access.RtWrite |
              Access.AllOperators | Access.System |
              Access.Maintenance | Access.Process |
              Access.Instrument)) {
        window.alert("Not yet implemented");
      }
    } else if (this.info.enable_navigator && text === "Navigator") {
      console.log("Navigator activated");
      if (this.is_authorized(Access.RtNavigator | Access.System |
              Access.Maintenance | Access.Process |
              Access.Instrument)) {
        window.open("xtt.html", "_blank");
      }
    } else if (!this.info.disable_help && text === "Help") {
      console.log("Help activated");
      window.open("xtt_help_index.html", "_blank");
    } else if (!this.info.disable_proview && text === "ProviewR") {
      console.log("ProviewR activated");
      window.open("http://www.proview.se", "_blank");
    } else {
      if (this.is_authorized(Access.RtRead | Access.RtWrite |
              Access.AllOperators | Access.System |
              Access.Maintenance | Access.Process |
              Access.Instrument)) {
        let button = this.info.buttons.find(e => e.text === text);
        if (button) {
          console.log("Found", button.text);
          let name = button.name;
          let n = name.indexOf(".pwg");
          if (n !== -1) {
            name = name.substring(0, n);
          }
          let url = "ge.html?graph=" + name;
          console.log("url", url);
          window.open(url, "_blank");
        }
      }
    }
  }

  login_cb(res) {
    if (res.sts & 1) {
      this.priv = res.value;
      sessionStorage.setItem("pwr_privilege", String(this.priv));
      if (this.user_text !== null) {
        this.user_text.textContent = this.user + " on " + this.host;
      }

      console.log("Login", this.user, "Priv", this.priv);
    } else {
      this.priv = 0;
      this.user = "none";
      sessionStorage.setItem("pwr_privilege", String(this.priv));
      if (this.user_text !== null) {
        this.user_text.textContent = "None on " + this.host;
      }
      console.log("Login failure", "Priv", this.priv);
    }
  }
}

let menu = new OpWindMenu();
