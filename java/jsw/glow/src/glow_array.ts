class GlowArray {
  a = [];
  ctx: GrowCtx;

  constructor(ctx) {
    this.ctx = ctx;
  }

  add(elem) {
    this.a.push(elem);
  }

  remove(elem) {
    let idx = this.a.indexOf(elem);
    if (idx != -1) {
      this.a.splice(idx, 1);
    }
    return (idx != -1);
  }

  size() {
    return this.a.length;
  }

  get(idx) {
    return this.a[idx];
  }

  open(lines, row) {
    let i;
    for (i = row; i < lines.length; i++) {
      let tokens = lines[i].split(' ');
      let key = parseInt(tokens[0], 10);
      let o;

      switch (key) {
        case GlowSave.Array:
          break;
        case GlowSave.NodeClass:
          o = new GlowNodeClass(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.NodeGroup:
          o = new GlowNodeGroup(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.ConClass:
          o = new GlowConClass(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.Con:
          o = new GlowCon(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowNode:
          o = new GrowNode(this.ctx);
          i = o.open(lines, i + 1);
          if (o.nc === null) {
            break;
          }

          this.add(o);
          break;
        case GlowSave.GrowSlider:
          o = new GrowSlider(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowGroup:
          o = new GrowGroup(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowConGlue:
          o = new GrowConGlue(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowRect:
          o = new GrowRect(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowRectRounded:
          o = new GrowRectRounded(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.Line:
          o = new GlowLine(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.Arc:
          o = new GlowArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.Rect:
          o = new GlowRect(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.Text:
          o = new GlowText(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.Arrow:
          o = new GlowArrow(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowArc:
          o = new GrowArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowLine:
          o = new GrowLine(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowPolyLine:
          o = new GrowPolyline(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowText:
          o = new GrowText(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.ConPoint:
          o = new GlowConPoint(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowConPoint:
          o = new GrowConPoint(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowAnnot:
          o = new GrowAnnot(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowBar:
          o = new GrowBar(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowBarArc:
          o = new GrowBarArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowTrend:
          o = new GrowTrend(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowXYCurve:
          o = new GrowXYCurve(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowTable:
          o = new GrowTable(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowAxis:
          o = new GrowAxis(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowAxisArc:
          o = new GrowAxisArc(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowImage:
          o = new GrowImage(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowWindow:
          o = new GrowWindow(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowFolder:
          o = new GrowFolder(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowPie:
          o = new GrowPie(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowBarChart:
          o = new GrowBarChart(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.GrowToolbar:
          o = new GrowToolbar(this.ctx);
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.Point:
          o = new GlowPoint();
          i = o.open(lines, i + 1);
          this.add(o);
          break;
        case GlowSave.End:
          return i;
        default:
          console.log("Syntax error in GlowVector");
          break;
      }
    }

    return i;
  }

  clear() {
    this.a.length = 0;
  }

  forEach(callback) {
    this.a.forEach(callback);
  }

  slice(start, end) {
    return this.a.slice(start, end);
  }

  find(callback) {
    return this.a.find(callback);
  }
}