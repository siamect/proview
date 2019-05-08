"use strict";

class OpWindMenu {
  priv: number;
  user: string;
  user_text: Text;
  host: string;
  gdh: Gdh;
  info: OpwindMenuInfo;
  constructor() {
    this.priv = 0;
    this.user = "";
    this.user_text = null;
    this.host = window.location.hostname;
    if (this.host === "") {
      this.host = "localhost";
    }

    this.gdh = new Gdh();
    this.gdh.open_cb = this.gdh_init_cb;
    this.gdh.init();
  }

  is_authorized(access) {
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
    this.gdh.login("", "", this.login_cb, this);
    this.gdh.getOpwindMenu(this.get_opplace(), this.get_menu_cb, 999);
  }

  add_menu_button(context, text) {
    let button = document.createElement("input");
    button.type = "button";
    button.className = "leftmenu-button";
    button.value = text;
    button.addEventListener('click', function() {
      menu.button_cb(button.value);
    });
    context.appendChild(button);
    context.appendChild(document.createElement("br"));
    return button;
  }

  get_menu_cb(id, data, sts, result) {
    this.info = result;
    console.log("Menu received", sts, data, result.buttons.length);
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
          this.gdh.login(user, passwd, this.login_cb, this);
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
          this.gdh.login("", "", this.login_cb, this);
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


    let button;
    for (let i = 0; i < result.buttons.length; i++) {
      this.add_menu_button(context, result.buttons[i].text);
    }
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
      } else {
        window.alert("Not authorized for this operation");
      }
    } else if (this.info.enable_alarmlist && text === "EventList") {
      console.log("EventList activated");
      if (this.is_authorized(Access.RtRead | Access.RtWrite |
          Access.AllOperators | Access.System |
          Access.Maintenance | Access.Process |
          Access.Instrument)) {
        window.open("ev.html?list=event", "_blank");
      } else {
        window.alert("Not authorized for this operation");
      }
    } else if (this.info.enable_eventlog && text === "EventLog") {
      console.log("EventLog activated");
      if (this.is_authorized(Access.RtRead | Access.RtWrite |
          Access.AllOperators | Access.System |
          Access.Maintenance | Access.Process |
          Access.Instrument)) {
        window.alert("Not yet implemented");
      } else {
        window.alert("Not authorized for this operation");
      }
    } else if (this.info.enable_navigator && text === "Navigator") {
      console.log("Navigator activated");
      if (this.is_authorized(Access.RtNavigator | Access.System |
          Access.Maintenance | Access.Process |
          Access.Instrument)) {
        window.open("xtt.html", "_blank");
      } else {
        window.alert("Not authorized for this operation");
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
        for (let i = 0; i < this.info.buttons.length; i++) {
          if (this.info.buttons[i].text === text) {
            console.log("Found", this.info.buttons[i].text);
            let name = this.info.buttons[i].name;
            let n = name.indexOf(".pwg");
            if (n !== -1) {
              name = name.substring(0, n);
            }
            let url = "ge.html?graph=" + name;
            console.log("url", url);
            window.open(url, "_blank");
            return;
          }
        }
      } else {
        window.alert("Not authorized for this operation");
      }
    }
  }

  login_cb(id, data, sts, result) {
    console.log("Login:", sts, result);
    if (sts & 1) {
      this.priv = result;
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
