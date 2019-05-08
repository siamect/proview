let CFormatC = {
  not_string: /[^s]/,
  not_bool: /[^t]/,
  not_type: /[^T]/,
  not_primitive: /[^v]/,
  number: /[diefg]/,
  numeric_arg: /[bcdiefguxXm]/,
  json: /[j]/,
  not_json: /[^j]/,
  text: /^[^\x25]+/,
  modulo: /^\x25{2}/,
  placeholder: /^\x25(?:([1-9]\d*)\$|\(([^\)]+)\))?(\+)?(0|'[^$])?(-)?(\d+)?(?:\.(\d+))?([b-gijostTuvxXm])/,
  key: /^([a-z_][a-z_\d]*)/i,
  key_access: /^\.([a-z_][a-z_\d]*)/i,
  index_access: /^\[(\d+)\]/,
  sign: /^[\+\-]/
};

class GlowCFormat {
  parse_tree: Array;

  constructor(key) {
    this.parse_tree = this.parse(key);
  }

  format() {
    let argv = arguments;
    let cursor = 0, tree_length = this.parse_tree.length, arg, output = '', i,
        k, ph, pad, pad_character, pad_length, is_positive, sign;
    for (i = 0; i < tree_length; i++) {
      if (typeof this.parse_tree[i] === 'string') {
        output += this.parse_tree[i];
      } else if (typeof this.parse_tree[i] === 'object') {
        ph = this.parse_tree[i]; // convenience purposes only
        if (ph.keys) { // keyword argument
          arg = argv[cursor];
          for (k = 0; k < ph.keys.length; k++) {
            if (arg == undefined) {
              throw new Error('[sprintf] Cannot access property ' + ph.keys[k] + ' of undefined value ' + ph.keys[k - 1]);
            }
            arg = arg[ph.keys[k]];
          }
        } else if (ph.param_no) { // positional argument (explicit)
          arg = argv[ph.param_no];
        } else { // positional argument (implicit)
          arg = argv[cursor++];
        }

        if (CFormatC.not_type.test(ph.type) && CFormatC.not_primitive.test(ph.type) && arg instanceof Function) {
          arg = arg();
        }

        if (CFormatC.numeric_arg.test(ph.type) && (typeof arg !== 'number' && isNaN(arg))) {
          throw new TypeError("[sprintf] expecting number but found");
        }

        if (CFormatC.number.test(ph.type)) {
          is_positive = arg >= 0;
        }

        switch (ph.type) {
          case 'b':
            arg = parseInt(arg, 10).toString(2);
            break;
          case 'c':
            arg = String.fromCharCode(parseInt(arg, 10));
            break;
          case 'd':
          case 'i':
            arg = parseInt(arg, 10);
            break;
          case 'j':
            arg = JSON.stringify(arg, null, ph.width ? parseInt(ph.width, 10) : 0);
            break;
          case 'e':
            arg = ph.precision ? parseFloat(arg).toExponential(ph.precision) : parseFloat(arg).toExponential();
            break;
          case 'f':
            arg = ph.precision ? parseFloat(arg).toFixed(ph.precision) : parseFloat(arg);
            break;
          case 'g':
            arg = ph.precision ? String(Number(arg.toPrecision(ph.precision))) : parseFloat(arg);
            break;
          case 'o':
            arg = (parseInt(arg, 10) >>> 0).toString(8);
            break;
          case 's':
            arg = String(arg);
            arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
            break;
          case 't':
            arg = String(!!arg);
            arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
            break;
          case 'T':
            arg = Object.prototype.toString.call(arg).slice(8, -1).toLowerCase();
            arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
            break;
          case 'u':
            arg = parseInt(arg, 10) >>> 0;
            break;
          case 'v':
            arg = arg.valueOf();
            arg = (ph.precision ? arg.substring(0, ph.precision) : arg);
            break;
          case 'x':
            arg = (parseInt(arg, 10) >>> 0).toString(16);
            break;
          case 'X':
            arg = (parseInt(arg, 10) >>> 0).toString(16).toUpperCase();
            break;
          case 'm':
            arg = "Msg: " + parseInt(arg, 10);
            break;
        }
        if (CFormatC.json.test(ph.type)) {
          output += arg;
        } else {
          if (CFormatC.number.test(ph.type) && (!is_positive || ph.sign)) {
            sign = is_positive ? '+' : '-';
            arg = arg.toString().replace(CFormatC.sign, '');
          } else {
            sign = '';
          }
          pad_character = ph.pad_char ? ph.pad_char === '0' ? '0' : ph.pad_char.charAt(1) : ' ';
          pad_length = ph.width - (sign + arg).length;
          let pad_rpt = '';
          for (let i = 0; i < pad_length; i++)
            pad_rpt += pad_character;
          pad = ph.width ? (pad_length > 0 ? pad_rpt : '') : '';
          output += ph.align ? sign + arg + pad : (pad_character === '0' ? sign + pad + arg : pad + sign + arg);
        }
      }
    }
    return output;
  };

  parse(fmt) {
    let _fmt = fmt, match, parse_tree = [], arg_names = 0;
    while (_fmt) {
      if ((match = CFormatC.text.exec(_fmt)) !== null) {
        parse_tree.push(match[0]);
      } else if ((match = CFormatC.modulo.exec(_fmt)) !== null) {
        parse_tree.push('%');
      } else if ((match = CFormatC.placeholder.exec(_fmt)) !== null) {
        if (match[2]) {
          arg_names |= 1;
          let field_list = [], replacement_field = match[2], field_match = [];
          if ((field_match = CFormatC.key.exec(replacement_field)) !== null) {
            field_list.push(field_match[1]);
            while ((replacement_field = replacement_field.substring(field_match[0].length)) !== '') {
              if ((field_match = CFormatC.key_access.exec(replacement_field)) !== null) {
                field_list.push(field_match[1]);
              } else if ((field_match = CFormatC.index_access.exec(replacement_field)) !== null) {
                field_list.push(field_match[1]);
              } else {
                throw new SyntaxError('[sprintf] failed to parse named argument key');
              }
            }
          } else {
            throw new SyntaxError('[sprintf] failed to parse named argument key');
          }
          match[2] = field_list;
        } else {
          arg_names |= 2;
        }
        if (arg_names === 3) {
          throw new Error('[sprintf] mixing positional and named placeholders is not (yet) supported');
        }

        parse_tree.push({
          placeholder: match[0],
          param_no: match[1],
          keys: match[2],
          sign: match[3],
          pad_char: match[4],
          align: match[5],
          width: match[6],
          precision: match[7],
          type: match[8]
        });
      } else {
        throw new SyntaxError('[sprintf] unexpected placeholder');
      }
      _fmt = _fmt.substring(match[0].length);
    }
    return parse_tree;
  };
}
