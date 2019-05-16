class GrowFrame {
  appl: Graph;
  timer = null;
  vars_object = null;
  tmp_ctx = null;
  nogdh: boolean;

  constructor(appl) {
    this.appl = appl;
    this.nogdh = this.get_nogdh();
    console.log("Nogdh:", this.nogdh);

    this.appl.ctx = new GrowCtx(null);
    this.appl.baseCtx = this.appl.ctx;
    this.appl.ctx.appl = appl;
    this.appl.ctx.setOwner(this.get_instance());
  }

  get_nogdh() {
    let vars = window.location.search.substring(1).split('&');
    return vars.length > 2 && vars[2].substring(4) === "no";
  }

  readGrowWeb(fname, read_cb) {
    let n = window.location.href.lastIndexOf('/');
    let target = window.location.href.substring(0, n + 1);
    target += fname;

    // console.log("Target: ", target);

    let req = new XMLHttpRequest();
    req.open("GET", target, true);
    req.addEventListener("load", function () {
      let lines = req.responseText.split('\n');
      read_cb(lines, 0);
    });
    req.send(null);
  }

  read_func(ctx, lines, row) {
    if (ctx === this.appl.baseCtx) {
      if (lines[0].substring(3, 15) === "DefaultWidth" &&
          lines[1].substring(3, 16) === "DefaultHeight") {
        let width = parseInt(lines[0].substring(16), 10);
        let height = parseInt(lines[1].substring(17), 10);
        ctx.gdraw.canvas.width = width;
        ctx.gdraw.canvas.height = height;
      }
    }

    ctx.open(lines, row);

    ctx.draw();
  }

  register_events(ctx) {
    this.appl.ctx.gdraw.canvas.addEventListener("click", function (event) {
      let y = event.pageY - this.appl.ctx.gdraw.offset_top;
      let x = event.pageX - 10; // Correction for left offset
      this.event_handler(x, y, Event.MB1Click);
    });

    this.appl.ctx.gdraw.canvas.addEventListener("mousedown", function (event) {
      let y = event.pageY - this.appl.ctx.gdraw.offset_top;
      let x = event.pageX - 10;
      this.event_handler(x, y, Event.MB1Down);
    });
    this.appl.ctx.gdraw.canvas.addEventListener("mouseup", function (event) {
      let y = event.pageY - this.appl.ctx.gdraw.offset_top;
      let x = event.pageX - 10;
      this.event_handler(x, y, Event.MB1Up);
    });
    this.appl.ctx.gdraw.canvas.addEventListener("mousemove", function (event) {
      let y = event.pageY - this.appl.ctx.gdraw.offset_top;
      let x = event.pageX - 10;
      let button_motion = 0;
      if (event.buttons === null) {
        button_motion = (event.which === 1);
      } else {
        button_motion = (event.buttons === 1);
      }
      if (button_motion) {
        this.event_handler(x, y, Event.ButtonMotion);
      } else {
        this.event_handler(x, y, Event.CursorMotion);
      }
    });
  }

  readGrowWindow(fname, read_cb) {
    let nctx = new GrowCtx(this.appl.baseCtx);
    nctx.appl = this.appl;

    let n = window.location.href.lastIndexOf('/');
    let target = window.location.href.substring(0, n + 1);
    target += fname;

    // console.log("Target: ", target);

    let req = new XMLHttpRequest();
    req.open("GET", target, true);
    req.addEventListener("load", function () {
      let lines = req.responseText.split('\n');
      read_cb(lines, 0);
    });
    req.send(null);

    return nctx;
  }

  grow_open() {
    console.log("grow_open");
  }

  grow_scan() {
    // this.appl.ctx.scan();
  }

  grow_cyclic() {
    // this.appl.ctx.gdh.getRefObjectInfoAll( this.flow_scan);
    // this.appl.ctx.draw();
    // this.timer = setTimeout( this.flow_cyclic, 2000);
  }

  grow_close() {
    console.log("Close function", this.timer);
    // clearTimeout(this.timer);
    // for ( let i in this.appl.ctx.gdh.pending)
    //  delete this.ctx.gdh.pending[i];
  }

  get_filename() {
    return window.location.search.substring(1).split('&')[0].substring(6) + ".pwg";
  }

  get_instance() {
    let query = window.location.search.substring(1);

    let vars = query.split('&');
    if (vars.length > 1) {
      return vars[1].substring(9);
    }
    return "";
  }

  event_handler(x, y, event) {
    let e = new GlowEvent();
    e.event = event;
    e.x = (x + this.appl.ctx.mw.offset_x) / this.appl.ctx.mw.zoom_factor_x;
    e.y = (y + this.appl.ctx.mw.offset_y) / this.appl.ctx.mw.zoom_factor_y;
    this.appl.ctx.event_handler(e);
  }
}