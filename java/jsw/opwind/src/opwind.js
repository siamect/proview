"use strict";

function OpWindMenu() {
  this.gdh = 0;
  this.buttons = null;
  this.priv = 0;
  this.user = "";
  this.user_text = null;
  this.host;

  var self = this;

  this.init = function () {
    this.host = window.location.hostname;
    if (this.host == "") {
      this.host = "localhost";
    }

    this.gdh = new Gdh();
    this.gdh.open_cb = this.gdh_init_cb;
    this.gdh.init();
  };

  this.is_authorized = function (access) {
    return (this.priv & access) ? true : false;
  }

  this.get_opplace = function () {
    var query = window.location.search.substring(1);

    console.log("query", query);
    var vars = query.split('&');
    console.log("vars", vars.length, vars[0].substring(8));
    return vars[0].substring(8);
  };

  this.gdh_init_cb = function () {
    var oid = new PwrtObjid(0, 0);
    self.user = "Default";
    self.gdh.login("", "", self.login_cb, self);
    self.gdh.getOpwindMenu(self.get_opplace(), self.get_menu_cb, 999);

  };

  this.add_menu_button = function (context, text) {
    var button = document.createElement("input");
    button.type = "button";
    button.className = "leftmenu-button";
    button.value = text;
    button.addEventListener('click',
      new Function("menu.button_cb(\"" + button.value + "\")"));
    context.appendChild(button);
    context.appendChild(document.createElement("br"));
    return button;
  };

  this.get_menu_cb = function (id, data, sts, result) {
    self.info = result;
    console.log("Menu received", sts, data, result.buttons.length);
    var context = document.getElementById("opwindmenu");

    document.getElementById("opwind_title").innerHTML = result.title;
    document.getElementById("opwind_text").innerHTML = result.text;

    if (result.enable_login) {
      self.user_text = document.createTextNode(self.user + " on " + self.host);
      context.appendChild(self.user_text);
      context.appendChild(document.createElement("hr"));

      document.getElementById("login_button")
        .addEventListener("click", function (event) {
          if (document.getElementById("login_frame").style.visibility ==
            'hidden') {
            document.getElementById("login_user").value = "";
            document.getElementById("login_passw").value = "";
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
          var user = document.getElementById("login_user").value;
          var passwd = document.getElementById("login_passw").value;
          if (user.trim() == "") {
            return;
          }
          document.getElementById("login_frame").style.visibility = 'hidden';
          document.getElementById("login_frame").style.height = '0px';
          var c = new JopCrypt();
          passwd = c.crypt("aa", passwd);

          self.user = user;
          self.gdh.login(user, passwd, self.login_cb, self);
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
          self.priv = 0;
          self.user = "Default";
          self.gdh.login("", "", self.login_cb, self);
        });

      document.getElementById("login_user").value = "";
      document.getElementById("login_passw").value = "";
      //document.getElementById("login_frame").setAttribute("style", "visibility:hidden;height:10px";
      document.getElementById("login_frame").style.visibility = 'hidden';
      document.getElementById("login_frame").style.height = '0px';
    } else {
      document.getElementById("login_button").remove();
      document.getElementById("login_frame").remove();
    }
    if (result.enable_language) {
      self.add_menu_button(context, "Language");
    }
    if (result.enable_alarmlist) {
      self.add_menu_button(context, "AlarmList");
      self.add_menu_button(context, "EventList");
    }
    if (result.enable_eventlog) {
      self.add_menu_button(context, "EventLog");
    }
    if (result.enable_navigator) {
      self.add_menu_button(context, "Navigator");
    }
    if (!result.disable_help) {
      self.add_menu_button(context, "Help");
    }
    if (!result.disable_proview) {
      self.add_menu_button(context, "ProviewR");
    }

    context.appendChild(document.createElement("hr"));


    var button;
    for (var i = 0; i < result.buttons.length; i++) {
      self.add_menu_button(context, result.buttons[i].text);
    }
  };

  this.button_cb = function (text) {

    if (self.info.enable_language && text == "Language") {
      console.log("Language activated");
    } else if (self.info.enable_alarmlist && text == "AlarmList") {
      console.log("AlarmList activated");
      if (!(self.is_authorized(Pwr.mAccess_RtRead | Pwr.mAccess_RtWrite |
          Pwr.mAccess_AllOperators | Pwr.mAccess_System |
          Pwr.mAccess_Maintenance | Pwr.mAccess_Process |
          Pwr.mAccess_Instrument))) {
        window.alert("Not authorized for this operation");
      } else {
        window.open("ev.html?list=alarm", "_blank");
      }
    } else if (self.info.enable_alarmlist && text == "EventList") {
      console.log("EventList activated");
      if (!(self.is_authorized(Pwr.mAccess_RtRead | Pwr.mAccess_RtWrite |
          Pwr.mAccess_AllOperators | Pwr.mAccess_System |
          Pwr.mAccess_Maintenance | Pwr.mAccess_Process |
          Pwr.mAccess_Instrument))) {
        window.alert("Not authorized for this operation");
      } else {
        window.open("ev.html?list=event", "_blank");
      }
    } else if (self.info.enable_eventlog && text == "EventLog") {
      console.log("EventLog activated");
      if (!(self.is_authorized(Pwr.mAccess_RtRead | Pwr.mAccess_RtWrite |
          Pwr.mAccess_AllOperators | Pwr.mAccess_System |
          Pwr.mAccess_Maintenance | Pwr.mAccess_Process |
          Pwr.mAccess_Instrument))) {
        window.alert("Not authorized for this operation");
      } else {
        window.alert("Not yet implemented");
      }
    } else if (self.info.enable_navigator && text == "Navigator") {
      console.log("Navigator activated");
      if (!(self.is_authorized(Pwr.mAccess_RtNavigator | Pwr.mAccess_System |
          Pwr.mAccess_Maintenance | Pwr.mAccess_Process |
          Pwr.mAccess_Instrument))) {
        window.alert("Not authorized for this operation");
      } else {
        window.open("xtt.html", "_blank");
      }
    } else if (!self.info.disable_help && text == "Help") {
      console.log("Help activated");
      window.open("xtt_help_index.html", "_blank");
    } else if (!self.info.disable_proview && text == "ProviewR") {
      console.log("ProviewR activated");
      window.open("http://www.proview.se", "_blank");
    } else {
      if (!(self.is_authorized(Pwr.mAccess_RtRead | Pwr.mAccess_RtWrite |
          Pwr.mAccess_AllOperators | Pwr.mAccess_System |
          Pwr.mAccess_Maintenance | Pwr.mAccess_Process |
          Pwr.mAccess_Instrument))) {
        window.alert("Not authorized for this operation");
      } else {
        for (var i = 0; i < self.info.buttons.length; i++) {
          if (self.info.buttons[i].text == text) {
            console.log("Found", self.info.buttons[i].text);
            var name = self.info.buttons[i].name;
            var n = name.indexOf(".pwg");
            if (n != -1) {
              name = name.substring(0, n);
            }
            var url = "ge.html?graph=" + name;
            console.log("url", url);
            window.open(url, "_blank");
            break;
          }
        }
      }
    }
  };

  this.login_cb = function (id, data, sts, result) {
    console.log("Login:", sts, result);
    if (sts & 1) {
      self.priv = result;
      sessionStorage.setItem("pwr_privilege", self.priv);
      if (self.user_text != null) {
        self.user_text.textContent = self.user + " on " + self.host;
      }

      console.log("Login", self.user, "Priv", self.priv);
    } else {
      self.priv = 0;
      self.user = "none";
      sessionStorage.setItem("pwr_privilege", self.priv);
      if (self.user_text != null) {
        self.user_text.textContent = "None on " + self.host;
      }
      console.log("Login failure", "Priv", self.priv);
    }
  };
}

var menu = new OpWindMenu();
menu.init();
