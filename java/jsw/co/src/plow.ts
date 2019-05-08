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
  nc_name: string;
  group: number;
  node_open: number;
  constructor(ctx) {
    this.a = new PlowArray(ctx);
    this.ctx = ctx;
    this.nc_name = "";
    this.group = 0;
    this.node_open = 0;
  }

  draw(g, p, node, highlight) {
    this.a.draw(g, p, node, highlight);
  }

  insert(elem) {
    this.a.add(elem);
  }
}

class PlowArray {
  a: Array<PlowNode>;
  ctx: PlowCtx;
  constructor(ctx) {
    this.a = [];
    this.ctx = ctx;
  }

  add(elem) {
    this.a.push(elem);
  }

  insertNode(elem, destination, code) {
    let idx = this.find(elem);
    if (idx !== -1) {
      return;
    }

    if (destination === null) {
      switch (code) {
        case Dest.INTOLAST:
        case Dest.AFTER:
          this.a.push(elem);
          elem.level = 0;
          break;
        default:
          elem.level = 0;
          this.a.unshift(elem);
      }
    } else {
      let dest_idx = this.find(destination);
      if (dest_idx === -1) {
        return;
      }

      switch (code) {
        case Dest.INTOFIRST:
          if (dest_idx === this.a.length - 1) {
            this.a.push(elem);
          } else {
            this.a.splice(dest_idx + 1, 0, elem);
          }
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
            if (idx === this.a.length) {
              this.a.push(elem);
            } else {
              this.a.splice(idx, 0, elem);
            }
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
    let idx = this.find(elem);
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
    for (let i = 0; i < this.a.length; i++) {
      this.a[i].draw(g, p, node, highlight);
    }
  }

  set_borders(node) {
    for (let i = 0; i < this.a.length; i++) {
      this.a[i].set_borders(node);
    }
  }

  configure() {
    for (let i = 0; i < this.a.length; i++) {
      this.a[i].pos.x = this.a[i].level;
      this.a[i].pos.y = i;
    }
  }

  close_node(node) {
    let idx = this.find(node);
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
    let idx = this.find(node);
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
    let idx = this.find(node);
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
    for (let i = 0; i < this.a.length; i++) {
      if (this.a[i] === node) {
        if (i === this.a.length - 1) {
          return null;
        }
        return this.a[i + 1];
      }
    }
    return null;
  }

  get_previous_object(node) {
    for (let i = 0; i < this.a.length; i++) {
      if (this.a[i] === node) {
        if (i === 0) {
          return null;
        }
        return this.a[i - 1];
      }
    }
    return null;
  }

  get_first_object() {
    if (this.a.length === 0) {
      return null;
    }
    return this.a[0];
  }

  get_last_object() {
    if (this.a.length === 0) {
      return null;
    }
    return this.a[this.a.length - 1];
  }

  find(elem) {
    for (let i = 0; i < this.a.length; i++) {
      if (this.a[i] === elem) {
        return i;
      }
    }
    return -1;
  }
}

class PlowNode {
  ctx: PlowCtx;
  userdata: object;
  x_right: number;
  x_left: number;
  y_high: number;
  y_low: number;
  nc: PlowNodeClass;
  pos: Point;
  n_name: string;
  annotv: Array;
  annotsize: Array;
  pixmapv: Array;
  trace_object: string;
  trace_attribute: string;
  trace_attr_type: number;
  highlight: boolean;
  select: boolean;
  invert: boolean;
  level: number;
  node_open: number;
  fill_color: number;
  p: number;
  old_value: number;
  first_scan: boolean;
  relative_position: number;
  constructor(ctx, nc, level) {
    this.ctx = ctx;
    this.userdata = null;
    this.x_right = 0.0;
    this.x_left = 0.0;
    this.y_high = 0.0;
    this.y_low = 0.0;
    this.nc = nc;
    this.pos = new Point();
    this.n_name = "";
    this.annotv = [];
    this.annotsize = [];
    this.pixmapv = [];
    this.trace_object = "";
    this.trace_attribute = "";
    this.trace_attr_type = 0;
    this.highlight = false;
    this.select = false;
    this.invert = false;
    this.level = level;
    this.node_open = 0;
    this.fill_color = 0;
    this.p = 0;
    this.old_value = 0;
    this.first_scan = true;
    this.relative_position = 0;
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
    let x = this.x_left * this.ctx.zoom_factor;
    let y = this.y_low * this.ctx.zoom_factor - 1;
    let width = (this.x_right - this.x_left) * this.ctx.zoom_factor;
    let height = (this.y_high - this.y_low) * this.ctx.zoom_factor + 2;
    g.fillStyle = "white";
    if (this.select) {
      g.fillStyle = "lightblue";
    }
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
    this.x_left = 1.0e37;
    this.x_right = -1.0e37;
    this.y_low = 1.0e37;
    this.y_high = -1.0e37;
    this.nc.a.set_borders(this);
  }

  event_handler(event, x, y) {
    if ((x - this.ctx.offset_x) / this.ctx.zoom_factor >= this.x_left &&
      (x - this.ctx.offset_x) / this.ctx.zoom_factor <= this.x_right &&
      (y - this.ctx.offset_y) / this.ctx.zoom_factor >= this.y_low &&
      (y - this.ctx.offset_y) / this.ctx.zoom_factor <= this.y_high) {
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
    return x >= this.x_left * this.ctx.zoom_factor && x <=
      (this.x_left + 1.75) * this.ctx.zoom_factor;
  }

  measure() {
    return new Rect(
        this.x_left * this.ctx.zoom_factor,
        this.y_low * this.ctx.zoom_factor,
        (this.x_right - this.x_left) * this.ctx.zoom_factor,
        (this.y_high - this.y_low) * this.ctx.zoom_factor);
  };
}

class PlowAnnot {
  RELATIVE_OFFSET = 1;
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

    let tsize = 0;
    let idx = this.ctx.zoom_factor / this.ctx.base_zoom_factor *
      (this.text_size + 4) - 4;
    if (idx < 0) {
      return;
    }

    switch (idx) {
      case 0:
        tsize = 8;
        break;
      case 1:
        tsize = 10;
        break;
      case 2:
        tsize = 12;
        break;
      case 3:
        tsize = 14;
        break;
      case 4:
        tsize = 14;
        break;
      case 5:
        tsize = 18;
        break;
      case 6:
        tsize = 18;
        break;
      case 7:
        tsize = 18;
        break;
      default:
        tsize = idx * 3;
    }


    g.font = tsize + "px Arial";
    if (node.invert) {
      g.fillStyle = "white";
    } else {
      g.fillStyle = "black";
    }
    if (highlight) {
      g.fillStyle = "red";
    }
    g.lineWidth = 0.5;

    let x = (this.p.x + p0.x) * this.ctx.zoom_factor;
    let y = (this.p.y + p0.y) * this.ctx.zoom_factor - tsize / 4;

    if ((this.annot_type & RELATIVE_POSITION) !== 0) {
      let rel_x = (p0.x + node.relative_position + this.RELATIVE_OFFSET) *
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
          if (node.invert) {
            g.fillStyle = "black";
          } else {
            g.fillStyle = "white";
          }
          break;
        default:
          if (node.invert) {
            g.fillStyle = "white";
          } else {
            g.fillStyle = "black";
          }
      }
      g.fillRect(x, y, width, height);
    }
  }

  set_borders(node) {
    if (this.ll.x + node.pos.x < node.x_left) {
      node.x_left = this.ll.x + node.pos.x;
    }
    if (this.ur.x + node.pos.x > node.x_right) {
      node.x_right = this.ur.x + node.pos.x;
    }
    if (this.ll.y + node.pos.y < node.y_low) {
      node.y_low = this.ll.y + node.pos.y;
    }
    if (this.ur.y + node.pos.y > node.y_high) {
      node.y_high = this.ur.y + node.pos.y;
    }
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
  gdh: Gdh;
  debug: boolean;
  nodraw: number;
  zoom_factor: number;
  base_zoom_factor: number;
  offset_x: number;
  offset_y: number;
  x_right: number;
  x_left: number;
  y_high: number;
  y_low: number;
  a: PlowArray;
  a_nc: PlowArray;
  name: string;
  gdraw: GDraw;
  select_object: PlowNode;
  event_cb: (event: object, object: PlowNode, x: number, y: number) => void;
  event_object: PlowNode;
  constructor() {
    this.gdh = null;
    this.debug = false;
    this.nodraw = 0;
    this.zoom_factor = 20.0;
    this.base_zoom_factor = 20.0;
    this.offset_x = 0;
    this.offset_y = 0;
    this.x_right = 0.0;
    this.x_left = 0.0;
    this.y_high = 0.0;
    this.y_low = 0.0;
    this.a = new PlowArray(this);
    this.a_nc = new PlowArray(this);
    this.name = "Claes context";
    this.gdraw = new GDraw(this);
    this.select_object = null;
    this.event_cb = null;
    this.event_object = null;
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
    this.a.add(n);
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
    this.a_nc.add(nc);
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
    return (o.y_high * this.zoom_factor <= window.pageYOffset +
      window.innerHeight - this.gdraw.offset_top) &&
      (o.y_low * this.zoom_factor >= window.pageYOffset -
        this.gdraw.offset_top);
  }

  scroll(y, factor) {
    window.scrollTo(window.scrollX, y * this.zoom_factor - window.innerHeight *
      factor + this.gdraw.offset_top)
  };
}