"use strict";

let Bitmaps = {
  leaf: 0,
  map: 2,
  openmap: 4,
  object: 6,
  attrenum: 8,
  attrarra: 10,
  attrarel: 12,
  attr: 14,
  crrwrite: 16,
  crrread: 18,
  ack: 20,
  alarm: 22,
  eventacked: 24,
  eventalarm: 26,
  eventreturn: 28,
  info: 30,
  system: 32,
  maintenance: 34,
  blockl: 36,
  blockr: 38,
  img: [null, null, null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null, null, null,
    null, null],
  pending: [null, null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null, null, null,
    null, null, null, null, null, null, null, null, null, null, null, null,
    null, null, null],
  images: [// leaf
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAASElEQVQokWP4jwMwMDBgYBR5XBpwGYZVIzYNGDZB+QyEFOBiM+CyCacGBI0hgEGjsxkYGCiwkSI/4tKMz0DqxCM2A4hOOcQCAObFEQyI2PpKAAAAAElFTkSuQmCC', // leaf inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAQUlEQVQokWNgYGD4jw1jA2hqsGvAZRhWjdg0oIsh8QkqwMXGbhMuDXAxdAFsNDobyifTRor8SFGoUhSPFKUcYjEAMsMz2y6w8kgAAAAASUVORK5CYII=', // map
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAKElEQVQokWP4DwUMDAwYGB9gwKUQm0FoGL/JOGwb1TgINZKFSbYOCgD1JxQJG0vK9AAAAABJRU5ErkJggg==', // map inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAK0lEQVQokWNgYGD4z8DA8B8bgMlhxbgUEgIMBE3Ggkc1Dk6N5AAGUm2DYQAkYTDe0vu7CAAAAABJRU5ErkJggg==', // openmap
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAQklEQVQokaXOwQ4AIAgCUP7/p+nUlpMSzY2bDwWHA5IEkFJCtaiKxE7dvsue8HZNJEPneoAuSq+OYAf9wy4K0Mk5C+d++RWimsw3AAAAAElFTkSuQmCC', // openmap inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAPUlEQVQokaXRQQoAQAgCQP//6brKZmSt0M2hIACI4yBURqiKXQp0ThuhGwmt7Vy00XvqCa7QN1wjhtYLCCYyCkvDVnkJOQAAAABJRU5ErkJggg==', // object
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAH0lEQVQokWP4TyJgYGBg+E8iZiDFdHrZMKqBGA2kYAD8gaJsjwzf9wAAAABJRU5ErkJggg==', //  object inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIklEQVQokWNgYGD4TxL+TyJg+P//P9GmwzXQ3oZRDdSOBwAGOSrkrXppgQAAAABJRU5ErkJggg==', // attrenum
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAPklEQVQokWP4////fwYGBgxMCDBgU4jNICyYsOlYbENoJNIW7BqJsYm2NuJyBVE2EqWRfjbiUoQ3oAgpwgUANLqccvbgec0AAAAASUVORK5CYII=', // attrenum inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAPElEQVQokWNgYGD4jw0wMDDgx9gUEgMYiDIdDaNoJBZg1UiUTTS1EZcriLKRKI30sxGXIgKBhF8RrqgBAOTOqGZ5aiCnAAAAAElFTkSuQmCC', // attrarra
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAAMElEQVQokWP4////fwYGBgxMCDBgU4jNICwYv+nY5InWOGojXW0khLEahtc6PDYCAB9hxkjBPICvAAAAAElFTkSuQmCC', // attrarray inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAMCAYAAABSgIzaAAAALUlEQVQokWNgYGD4jw0wMDDgx9gUEgMYCJmOTZ5ojaM20tVGQgCHK/AnLVwAAPonfpBwU5f4AAAAAElFTkSuQmCC', // attrarel
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIklEQVQokWP4////fwYGBpwYHTDANGADeDWM2jAEbSAFAADB26JsIjYj1AAAAABJRU5ErkJggg==', // attrarel inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIElEQVQokWNgYGD4jw8wMDCgYwZcEvg1jNowBG0gBQMAQN8q5COZl1cAAAAASUVORK5CYII=', // attr
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAIUlEQVQokWP4TyJgGIQaGBgY/pOIibcEroEkTLIfhoEGADzs8B5gcUg/AAAAAElFTkSuQmCC', // attr inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAJUlEQVQokWNgYGD4TyIedBr+kwgY/v//T7TpcA0k2TAIQ4nmGgDFzt0jExR7hgAAAABJRU5ErkJggg==', // crrwrite
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAN0lEQVQokaWNQQoAMAjD8v9P633WVWhBkJIotQQoYPYbrHYrqC9D+MG54OBMuMC54GApuPBed9OxiMNLGke1JwAAAABJRU5ErkJggg==', // crrwrite inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAANElEQVQokWNgYGD4jw3DABY57IqxsQlqwGELSYop1ECEYgo0EKmYQg1EKMbUQEAxRAMpAABRMgoFjbTzXgAAAABJRU5ErkJggg==', // crrread
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWNgYGD4j4yRATY5BmyKkGks4qgS2BSj2UYFDdgAdW2gvpOwhDW6ItwaCGI8JuHWgMOtWD0PACufaaWhXDFDAAAAAElFTkSuQmCC', // crrread inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWP4jwYYGBjgGKscNkXINLo4hgZsipFto44GbJi6NlDfSehhjUxjyGMVxQMYcJmEVwPB5ICEAdcbY6vf9TVAAAAAAElFTkSuQmCC', // ack
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAS0lEQVQokZ2Q2w4AIAhCz///ND11M3RVmxuDUBRAtw8QHRyC4SSJSDjDht1Yhxdudks+bFNxYsX9G6rz2qVHxqRspGi2Wpoji/dqaLh22DbO2VuXAAAAAElFTkSuQmCC', // ack inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAASklEQVQokWP4////fwYGBqIwVC2cgSKBTQzKxhDA0ICmGau12BRCMLJp2BQgy8H9gM9ELGJkasAXvFg9TQjgDCVc8YBXAwFMmgYASkT1C9E5Ya0AAAAASUVORK5CYII=', // alarm
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAARklEQVQokZ2QQQoAMAjD+v9Pu9PA1ThQQRBtoFUBJSkk0SlwOwKykCAEcn+BK8hih/aAe++y7IDuhWgXfcKHCuBBfX6ASR3Vn8ZINQzCrQAAAABJRU5ErkJggg==', // alarm inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAQ0lEQVQokWNgYGD4j45hAJscA8UakBXi0ITddDy2YJqOrBiLJjI1oLsdj1/I0IAnCLFpwtSALYQwNKB7FJ2NooEUDAAtGwcI+Svs4gAAAABJRU5ErkJggg==', // eventacked
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAL0lEQVQokWP4TyJgGCkaGBgQyhhwSeASw9CAYho2A3CZiE0xVg34FOPUgA+QrAEA1FYi+tWeG/cAAAAASUVORK5CYII=', // eventacked inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAMElEQVQokWNgYGD4TyIeGRr+//+PXQOaBC4xVElkBdgMwHASTBEOxdj9gEcxHYIVAC5kqlZXl5JMAAAAAElFTkSuQmCC', // eventalarm
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAJ0lEQVQokWP4jwMwMDBgF8elGIZHNeDSANOEVRyXBpwGUcUGqvoBAK+H8xt0qXFWAAAAAElFTkSuQmCC', // eventalarm inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAJ0lEQVQokWNgYGD4jw3///8fqzgDLsUwMKoBlwaSg5UAHoiIw2cDAFMz2iY65DAoAAAAAElFTkSuQmC', // eventreturn
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAASUlEQVQokZ2RUQoAIAjFdv9L11dhskeUEIQ2mcYIAXjeisA+EahFA9ad/siAA0wqSUsnu87wDZh3VEodO6Rr1c51rc8fd9OoMQHLk7dXak3qLwAAAABJRU5ErkJggg==', // eventreturn inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAR0lEQVQokZ2S2woAIAhD9/8/vV6tjgMNhLxMN1SSTGYb46JkfS2gJglQ/ncRAZ5JTCXQYsFRwxpAvFtKoeMvmoqDv1jc5DQONycV+bfOetgAAAAASUVORK5CYII=', // info
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAOUlEQVQokWNgYGD4///////E0AwMDP8hPDQAk8QhjlMCuzg2G3AB6tlAez8MglAiCZNiw/////8DACmmtFrq9aGNAAAAAElFTkSuQmCC', // info inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAN0lEQVQokWP4////fwYGhv/E0FA2w390jCSJIc6ASwKnBmw24MLUs4H2fhgEoUQKIMkGBgaG/wDZFBj2pVi3HgAAAABJRU5ErkJggg==', // system
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAN0lEQVQokWNgYGD4TyJm+E8sgGugig3YxHFqwGcIQQ3obAwN6G7GawMuhVg1EFKMUwM+QJ+IAwDD1Gyi1EZc6gAAAABJRU5ErkJggg==', // system inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAO0lEQVQokWP4TyJg+P///38GBgaiMFwDVWzAJo5TA15DCGnAYKNrwHAzPhtwKcSqgZBinBqIigeaRhwAPuZgrlZXy70AAAAASUVORK5CYII=', // maintenance
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAP0lEQVQokZWPQQoAMAzC8v9Pu8soDOxaBS82QkWNgPKTJ/BYsLekAOxfquwHWyfws6ED7OgtfNk9bAuTSGBJOufqnHIsmYkzAAAAAElFTkSuQmCC', // maintenance inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAQUlEQVQokZWPQQ4AMAQE5/+f1kuTCkuROGAmADCVPsJsBfdCsX0u3N7sJFfXsAqW8BMaID+9gE3SJayEFo7CFwY7GtAw3ouVj50AAAAASUVORK5CYII=', // blockl
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAQElEQVQokZ2QQQ4AMAjC+v9Pu9MWl5AJ42a0IlKBgCIadgGgQ8imqo9D3zDBvJoyR+zwlWHS9SUHaOf4QOywtQCQGucn0g2dEQAAAABJRU5ErkJggg==', // blockl inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAOklEQVQokWNgYGD4Tyz+////fwZSFBOtAaoQQsM46JLY+HAbUEwgrJmwyZTZQJYfSAolmsQDWTENwwByoOYaWbAqegAAAABJRU5ErkJggg==', // blockr
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWP4jwMwMDBgF8enAZsmvBqw2cSARQBDIQqbGEUoaggpQBcj3Qay/IAN4AwlfBrIigeiNeACADpU5yeYXn+XAAAAAElFTkSuQmCC', // blockr inverted
    'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAwAAAAMCAYAAABWdVznAAAAPElEQVQokWNgYGD4jw3///8fqzgDPg04NOG3AUMTugA2hWhqiFKEzCeoAF2MRBvI8gNJoUT1eCBaAy4MAMhm5hr5iTaWAAAAAElFTkSuQmCC']
};

const DRAWOFFSET = 2;
const TREE_INDENTATION = 1.0;
const RELATIVE_POSITION = 1;
const NEXT_RELATIVE_POSITION = 2;

enum Dest {
  INTOLAST,
  INTOFIRST,
  AFTER,
  BEFORE
}

enum Open {
  ATTRIBUTES = 1,
  CHILDREN = 2,
  CROSSREFERENCES = 4,
  ALL = 7
}

enum Color {
  BLACK = 1,
  RED,
  GRAY,
  DARKGRAY,
  LIGHTGRAY,
  WHITE,
  YELLOW,
  GREEN,
  LIGHTBLUE,
  BLUE,
  VIOLET
}

enum Event {
  MB1Click,
  MB1ClickShift,
  Key_Up,
  Key_Down,
  Key_Right,
  Key_Left,
  Key_ShiftRight,
  Key_CtrlR,
  Key_CtrlL,
  Key_CtrlG,
  ObjectDeleted
}

class PlowNodeClass {
  a: PlowArray;
  ctx: PlowCtx;
  nc_name = "";
  group = 0;
  node_open = 0;

  constructor(ctx) {
    this.a = new PlowArray(ctx);
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
    this.a.draw(g, p, node, highlight);
  }

  insert(elem) {
    this.a.push(elem);
  }
}

class PlowArray {
  a: Array<PlowNode> = [];
  ctx: PlowCtx;

  constructor(ctx) {
    this.ctx = ctx;
  }

  push(elem) {
    this.a.push(elem);
  }

  insertNode(elem, destination, code) {
    let idx = this.a.indexOf(elem);
    if (idx !== -1) {
      return;
    }

    if (destination === null) {
      elem.level = 0;
      if (code == Dest.INTOLAST || code == Dest.AFTER) {
        this.a.push(elem);
      } else {
        this.a.unshift(elem);
      }
    } else {
      let dest_idx = this.a.indexOf(destination);
      if (dest_idx === -1) {
        return;
      }

      switch (code) {
        case Dest.INTOFIRST:
          this.a.splice(dest_idx + 1, 0, elem);
          elem.level = destination.level + 1;
          break;
        case Dest.INTOLAST:
          if (dest_idx === this.a.length - 1) {
            this.a.push(elem);
          } else {
            idx = this.a.length;
            for (let i = dest_idx + 1; i < this.a.length; i++) {
              if (this.a[i].level <= destination.level) {
                idx = i;
                break;
              }
            }
            this.a.splice(idx, 0, elem);
          }
          elem.level = destination.level + 1;
          break;
        case Dest.AFTER:
          if (dest_idx === this.a.length - 1) {
            this.a.push(elem);
          } else {
            let i;
            for (i = idx; i < this.a.length; i++) {
              if (this.a[i].level < destination.level) {
                break;
              }
            }
            this.a.splice(i, 0, elem);
          }
          elem.level = destination.level;
          break;
        case Dest.BEFORE:
          if (idx > 0) {
            idx--;
          }
          this.a.splice(idx, 0, elem);
          elem.level = destination.level;
          break;
      }
    }
  }

  remove(elem) {
    let idx = this.a.indexOf(elem);
    if (idx === -1) {
      return;
    }
    this.ctx.event_cb(Event.ObjectDeleted, this.a[idx + 1], 0, 0);
    this.a.splice(idx, 1);
  }

  size() {
    return this.a.length;
  }

  get(idx) {
    return this.a[idx];
  }

  draw(g, p, node, highlight) {
    this.a.forEach(e => e.draw(g, p, node, highlight));
  }

  set_borders(node) {
    this.a.forEach(e => e.set_borders(node));
  }

  configure() {
    for (let i = 0; i < this.a.length; i++) {
      this.a[i].pos.x = this.a[i].level;
      this.a[i].pos.y = i;
    }
  }

  close_node(node) {
    let idx = this.a.indexOf(node);
    if (idx === -1) {
      return;
    }
    let level = node.level;
    let i;
    for (i = idx + 1; i < this.a.length; i++) {
      if (this.a[i].level <= level) {
        break;
      }
    }
    let next_idx = i;
    if (next_idx === idx + 1) {
      return;
    }
    for (i = idx + 1; i < next_idx; i++) {
      // Event backcall
      if (this.ctx.select_object === this.a[idx + 1]) {
        this.ctx.select_object = null;
      }
      this.ctx.event_cb(Event.ObjectDeleted, this.a[idx + 1], 0, 0);
      this.a.splice(idx + 1, 1);
    }
  }

  get_parent_object(node) {
    let idx = this.a.indexOf(node);
    if (idx === -1) {
      return null;
    }

    for (let i = idx; i >= 0; i--) {
      if (this.a[i].level < node.level) {
        return this.a[i];
      }
    }
    return null;
  }

  get_first_child(node) {
    let idx = this.a.indexOf(node);
    if (idx === -1) {
      return null;
    }

    if (this.a.length === idx - 1) {
      return null;
    }

    if (this.a[idx + 1].level > node.level) {
      return this.a[idx + 1];
    }
    return null;
  }

  get_next_sibling(node) {
    let found = false;
    for (let i = 0; i < this.a.length; i++) {
      if (this.a[i] === node) {
        found = true;
        continue;
      }
      if (found) {
        if (this.a[i].level === node.level) {
          return this.a[i];
        }
        if (this.a[i].level < node.level) {
          return null;
        }
      }
    }
    return null;
  }

  get_next_object(node) {
    let idx = this.a.indexOf(node);
    return this.a[idx + 1] || null;
  }

  get_previous_object(node) {
    let idx = this.a.indexOf(node);
    return this.a[idx - 1] || null;
  }

  get_first_object() {
    return this.a[0] || null;
  }

  get_last_object() {
    return this.a[this.a.length - 1] || null;
  }

  indexOf(elem) {
    return this.a.indexOf(elem);
  }

  forEach(callback) {
    this.a.forEach(callback);
  }
}

class PlowNode extends Rect {
  ctx: PlowCtx;
  userdata: object = null;
  nc: PlowNodeClass;
  pos = new Point();
  n_name = "";
  annotv = [];
  annotsize = [];
  pixmapv = [];
  trace_object = "";
  trace_attribute = "";
  trace_attr_type = 0;
  highlight = false;
  select = false;
  invert = false;
  level: number;
  node_open = 0;
  fill_color = 0;
  p = 0;
  old_value = 0;
  first_scan = true;
  relative_position = 0;

  constructor(ctx, nc, level) {
    super();
    this.ctx = ctx;
    this.nc = nc;
    this.level = level;
  }

  set_annotation(number, text) {
    if (number >= 10) {
      return;
    }
    this.annotv[number] = text;
  }

  set_annotation_pixmap(number, pixmap) {
    if (number >= 10) {
      return;
    }
    this.pixmapv[number] = pixmap;
  }

  draw_object() {
    this.draw(this.ctx.gdraw.gctx, null, null, false);
  }

  draw(g, p, node, highlight) {
    let x = this.ll_x * this.ctx.zoom_factor;
    let y = this.ll_y * this.ctx.zoom_factor - 1;
    let width = this.width() * this.ctx.zoom_factor;
    let height = this.height() * this.ctx.zoom_factor + 2;
    g.fillStyle = (this.select) ? "lightblue" : "white";
    g.fillRect(x, y, width, height);

    this.nc.draw(g, this.pos, this, this.highlight);
  }

  connect() {
    if (this.trace_object === "" || this.trace_attribute === "") {
      return;
    }

    let n = this.trace_attribute.indexOf('#');
    if (n !== -1) {
      this.trace_attribute = this.trace_attribute.substring(0, n);
    }

    let o = this.trace_object + "." + this.trace_attribute;
    this.p = this.ctx.gdh.refObjectInfo(o, this.trace_attr_type, null);
    console.log("connecting", o, this.p);
  }

  scan() {
    if (this.p === 0) {
      return;
    }
    let v1 = this.ctx.gdh.getObjectRefInfo(this.p);
    let evaluate = true;
    if (this.first_scan) {
      this.first_scan = false;
    } else if (v1 === this.old_value) {
      return;
    }

    this.highlight = !!v1;

    this.old_value = v1;
    this.draw_object();
  }

  set_borders(node) {
    this.set(new Rect(1.0e37, 1.0e37, -1.0e37, -1.0e37));
    this.nc.a.set_borders(this);
  }

  event_handler(event, x, y) {
    let p = new Point(
        (x - this.ctx.offset_x) / this.ctx.zoom_factor,
        (y - this.ctx.offset_y) / this.ctx.zoom_factor);
    if (this.hit(p)) {
      this.ctx.event_object = this;
      return 1;
    }
    return 0;
  }

  set_select(select) {
    if (select) {
      this.select = true;
      this.ctx.select_object = this;
    } else {
      this.select = false;
      if (this.ctx.select_object === this) {
        this.ctx.select_object = null;
      }
    }
    if (select !== this.select) {
      this.draw_object();
    }
  }

  set_invert(invert) {
    this.invert = invert;
    this.draw_object();
  }

  set_userdata(userdata) {
    this.userdata = userdata;
  }

  get_userdata() {
    return this.userdata;
  }

  in_icon(x, y) {
    return x >= this.ll_x * this.ctx.zoom_factor &&
        x <= (this.ll_x + 1.75) * this.ctx.zoom_factor;
  }

  measure() {
    return this;
  };
}

class PlowAnnot {
  static RELATIVE_OFFSET = 1;
  ctx: PlowCtx;
  p: Point;
  draw_type: number;
  text_size: number;
  annot_type: number;
  number: number;

  constructor(ctx, x, y, text_size, text_color, annot_type, number) {
    this.p = new Point(x, y);
    this.draw_type = text_color;
    this.text_size = text_size;
    this.annot_type = annot_type;
    this.number = number;
    this.ctx = ctx;
  }

  draw(g, p0, node, highlight) {
    if (node === null) {
      return;
    }
    if (node.annotv[this.number] === null) {
      return;
    }

    let idx = this.ctx.zoom_factor / this.ctx.base_zoom_factor *
        (this.text_size + 4) - 4;
    if (idx < 0) {
      return;
    }

    let tsize = (idx < 0 || idx > 7) ? (idx * 3) : ([8, 10, 12, 14, 14, 18, 18, 18][idx]);

    g.font = tsize + "px Arial";
    if (highlight) {
      g.fillStyle = "red";
    } else {
      g.fillStyle = (node.invert) ? "white" : "black";
    }
    g.lineWidth = 0.5;

    let x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    let y = (this.p.y + p0.y) * this.ctx.zoom_factor - tsize / 4;

    if ((this.annot_type & RELATIVE_POSITION) !== 0) {
      let rel_x = (p0.x + node.relative_position + PlowAnnot.RELATIVE_OFFSET) *
          this.ctx.zoom_factor;
      if (x < rel_x) {
        x = rel_x;
      }
    }

    let tokens = node.annotv[this.number].split('\n');
    for (let i = 0; i < tokens.length; i++) {
      g.fillText(tokens[i], x, y);
      y += tsize * 1.4;
    }
    if ((this.annot_type & NEXT_RELATIVE_POSITION) !== 0 ||
        (this.annot_type & RELATIVE_POSITION) !== 0) {
      node.relative_position =
          (x + g.measureText(node.annotv[this.number]).width) /
          this.ctx.zoom_factor - p0.x;
    }
  }

  set_borders(node) {
  }
}

class PlowAnnotPixmap {
  ctx: PlowCtx;
  p: Point;
  number: number;

  constructor(ctx, x, y, number) {
    this.p = new Point(x, y);
    this.number = number;
    this.ctx = ctx;
  }

  draw(gctx, p0, node, highlight) {
    let x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    let y = (this.p.y + p0.y) * this.ctx.zoom_factor;

    let img;
    let bix = node.pixmapv[this.number];
    if (typeof bix === 'undefined' || bix === null) {
      return;
    }
    if (node.invert) {
      bix++;
    }

    img = Bitmaps.img[bix];
    if (img === null) {
      let img = new Image();
      img.src = Bitmaps.images[bix];
      Bitmaps.img[bix] = img;
      Bitmaps.pending[bix] = [];
      Bitmaps.pending[bix].push(new Point(x, y));

      img.onload = function () {
        for (let i = 0; i < Bitmaps.pending[bix].length; i++) {
          gctx.drawImage(img, Bitmaps.pending[bix][i].x,
              Bitmaps.pending[bix][i].y);
        }
        Bitmaps.pending[bix] = null;
      }
    } else {
      if (img.complete) {
        gctx.drawImage(img, x, y);
      } else {
        Bitmaps.pending[bix].push(new Point(x, y));
      }
    }
  }

  set_borders(node) {
  }
}

class PlowRect {
  ctx: PlowCtx;
  ll: Point;
  ur: Point;
  border_color: number;
  fill_color: number;
  fill: number;
  fix_color: number;

  constructor(ctx, x, y, width, height, fill_color, border_color, fill, fix_color) {
    this.ll = new Point(x, y);
    this.ur = new Point(x + width, y + height);
    this.border_color = border_color;
    this.fill_color = fill_color;
    this.fill = fill;
    this.fix_color = fix_color;
    this.ctx = ctx;
  }

  draw(g, p, node, highlight) {
    let x = (this.ll.x + p.x) * this.ctx.zoom_factor;
    let y = (this.ll.y + p.y) * this.ctx.zoom_factor;
    let width = (this.ur.x - this.ll.x) * this.ctx.zoom_factor;
    let height = (this.ur.y - this.ll.y) * this.ctx.zoom_factor;

    g.lineWidth = 1;
    switch (this.border_color) {
      case Color.GRAY:
        g.strokeStyle = "grey";
        break;
      case Color.RED:
        g.strokeStyle = "red";
        break;
      case Color.WHITE:
        g.strokeStyle = "white";
        break;
      default:
        g.strokeStyle = "black";
    }
    if (highlight) {
      g.strokeStyle = "red";
    }
    g.strokeRect(x, y, width, height);

    if (this.fill) {
      switch (this.fill_color) {
        case Color.GRAY:
          g.fillStyle = "grey";
          break;
        case Color.RED:
          g.fillStyle = "red";
          break;
        case Color.YELLOW:
          g.fillStyle = "yellow";
          break;
        case Color.GREEN:
          g.fillStyle = "lightgreen";
          break;
        case Color.WHITE:
          g.fillStyle = (node.invert) ? "black" : "white";
          break;
        default:
          g.fillStyle = (node.invert) ? "white" : "black";
      }
      g.fillRect(x, y, width, height);
    }
  }

  set_borders(node) {
    node.set(Rect.union(node, new Rect(this.ll.x + node.pos.x, this.ll.y + node.pos.y, this.ur.x + node.pos.x, this.ur.y + node.pos.y)));
  }
}

class GDraw {
  ctx: PlowCtx;
  canvas: HTMLCanvasElement;
  gctx: CanvasRenderingContext2D;
  offset_top: number;

  constructor(ctx) {
    this.ctx = ctx;
    this.canvas = document.querySelector("canvas");
    this.gctx = this.canvas.getContext("2d");
    this.offset_top = this.canvas.offsetTop;
  }
}

class PlowCtx {
  gdh: Gdh = null;
  debug = false;
  nodraw = 0;
  zoom_factor = 20.0;
  base_zoom_factor = 20.0;
  offset_x = 0;
  offset_y = 0;
  rect: Rect;
  a: PlowArray;
  a_nc: PlowArray;
  name = "Claes context";
  gdraw: GDraw;
  select_object: PlowNode = null;
  event_cb: (event: object, object: PlowNode, x: number, y: number) => void = null;
  event_object: PlowNode = null;

  constructor() {
    this.a = new PlowArray(this);
    this.a_nc = new PlowArray(this);
    this.gdraw = new GDraw(this);
    this.rect = new Rect();
  }

  draw() {
    if (this.nodraw > 0) {
      return;
    }
    this.gdraw.gctx.fillStyle = "white";
    this.gdraw.gctx.fillRect(0, 0, this.gdraw.canvas.width,
        this.gdraw.canvas.height);
    this.a.draw(this.gdraw.gctx, null, null, false);
  }

  connect() {
    for (let i = 0; i < this.a.size(); i++) {
      this.a.get(i).connect();
    }
  }

  scan() {
    console.log("ctx scan", this.a.size());
    for (let i = 0; i < this.a.size(); i++) {
      this.a.get(i).scan();
    }
  }

  set_nodraw() {
    this.nodraw++;
  }

  reset_nodraw() {
    this.nodraw--;
  }

  size() {
    return this.a.size()
  }

  event_handler(event, x, y) {
    let sts = 0;
    switch (event) {
      case Event.MB1Click:
      case Event.MB1ClickShift:
        for (let i = 0; i < this.a.size(); i++) {
          if (this.a.get(i) instanceof PlowNode) {
            sts = this.a.get(i).event_handler(event, x, y);
            if (sts === 1) {
              break;
            }
          }
        }
        if (sts === 1) {
          this.event_cb(event, this.event_object, x, y);
          this.draw();
        }
        break;
      case Event.Key_Up:
      case Event.Key_Down:
      case Event.Key_Left:
      case Event.Key_Right:
      case Event.Key_ShiftRight:
      case Event.Key_CtrlR:
      case Event.Key_CtrlL:
      case Event.Key_CtrlG:
        this.event_cb(event, null, 0, 0);
        break;
    }
  }

  set_select(select) {
    for (let i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof PlowNode) {
        this.a.get(i).set_select(select);
      }
    }
  }

  set_invert(invert) {
    for (let i = 0; i < this.a.size(); i++) {
      if (this.a.get(i) instanceof PlowNode) {
        this.a.get(i).set_invert(invert);
      }
    }
  }

  get_select() {
    return this.select_object;
  }

  insert(n, dest) {
    this.a.push(n);
  }

  insertNode(n, destination, destCode) {
    this.a.insertNode(n, destination, destCode);
  }

  remove(n) {
    if (this.select_object === n) {
      this.select_object = null;
    }
    this.a.remove(n);
  }

  insert_nc(nc) {
    this.a_nc.push(nc);
  }

  configure() {
    this.a.configure();
    this.a.set_borders(null);
    this.gdraw.canvas.height = this.a.a.length * this.zoom_factor;
  }

  get_parent_object(o) {
    return this.a.get_parent_object(o);
  }

  get_next_object(o) {
    return this.a.get_next_object(o);
  }

  get_last_object() {
    return this.a.get_last_object();
  }

  get_first_object() {
    return this.a.get_first_object();
  }

  get_previous_object(o) {
    return this.a.get_previous_object(o);
  }

  close_node(o) {
    this.a.close_node(o);
  }

  is_visible(o) {
    return (o.ur_y * this.zoom_factor <= window.pageYOffset +
        window.innerHeight - this.gdraw.offset_top) &&
        (o.ll_y * this.zoom_factor >= window.pageYOffset -
            this.gdraw.offset_top);
  }

  scroll(y, factor) {
    window.scrollTo(window.scrollX, y * this.zoom_factor - window.innerHeight *
        factor + this.gdraw.offset_top)
  };
}