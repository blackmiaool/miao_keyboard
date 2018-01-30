import MacroLine from "@/components/macro-line";

const config = `!<+a::blackmiaool
^!+a::qk333333
<+d::sll{space}l
>^w::{esc}
>^n::{down}
>^p::{up}
>^f::{right}
>^b::{left}
>^a::{home}
>^e::{end}
>^d::{deleteforward}
>^v::{pagedown}
<!v::{pageup}
^!p::{printscreen}
<+p::git{space}pull{space}--rebase
<+f::function
<+l::local{space} 
<+r::return{space}
<+q::qk333333
>+q::~
>+w::\` 
<+s::sunqimin
<+i::@xiaomi.com
<+b::blackmiaool
<+c::console.log(){left}
<+v::console.warn(){left}
<+t::this.state.
<+w::ilwt.science
<+o::const{space}
<+u::sudo{space}apt-get{space}install
<^<!r::{restart}
<+n::(()=>{leftbracket}{rightbracket});{left}{left}{left}{enter}
140::{<^-down}{left}
141::{<^-down}{right}
# <: left modifier key
# >: right modifier key
# ^: ctrl
# !: alt
# +: shift
# #: win`;
const modifierMap = {
    "^": "Ctrl",
    "!": "Alt",
    "+": "Shift",
    "#": "Win"
};
const printableKeyMap = {
    leftbracket: '{',
    rightbracket: '}',
};
function match2arr(str, reg) {
    const ret = [];
    let result;
    // eslint-disable-next-line no-cond-assign
    while (result = reg.exec(str)) {
        ret.push(result.slice(1));
    }
    return ret;
}
function matchSubPattern(expression, patterns) {
    const output = [];
    let pos = 0;
    expression = expression.trim();
    while (pos !== expression.length) {
        const strThis = expression.substr(pos);
        const found = patterns.some(({ reg, mode }) => {
            const match = strThis.match(reg);
            if (match) {
                let foundPrint = false;
                const last = output[output.length - 1];

                if (mode === 'print') {
                    if (last && last.mode === 'print') {
                        last.value += match[0];
                        foundPrint = true;
                    }
                } else if (mode === 'press-key') {
                    const toPrint = printableKeyMap[match[1]];

                    if (toPrint) {
                        foundPrint = true;
                        if (last && last.mode === 'print') {
                            last.value += toPrint;
                        } else {
                            output.push({ value: match[0], mode: 'print' });
                        }
                    }
                }
                if (!foundPrint) {
                    if (mode === 'print') {
                        output.push({ value: match[0], mode });
                    } else if (mode === 'press-key') {
                        output.push({ value: match[1], mode });
                    } else if (mode === 'press-toggle') {
                        const modifiers = match2arr(match[1], /([<>]?)([\^+!#])/g);
                        const action = match[2];
                        const text = modifiers.map((arr) => {
                            let modifier = "";
                            if (arr[0] === '<') {
                                modifier += "L";
                            } else if (arr[0] === '>') {
                                modifier += 'R';
                            } else {
                                modifier += modifierMap[arr[0]];
                            }
                            if (arr[1]) {
                                modifier += modifierMap[arr[1]];
                            }
                            return modifier;
                        }).join('+');

                        output.push({
                            value: {
                                modifiers,
                                action,
                                text
                            },
                            mode
                        });
                    }
                }


                pos += match[0].length;
                return true;
            }
            return false;
        });
        if (!found) {
            console.warn('invalid expression', expression, pos);
            break;
        }
    }
    return output;
}
function modifier2PlainText(modifier) {
    let ret = "";
    if (modifier[0] === "<") {
        ret += "L";
    } else if (modifier[0] === ">") {
        ret += "R";
    }
    ret += modifierMap[modifier[modifier.length - 1]];
    return ret;
}
const list = config
    .split(/\r?\n/)
    .map(line => {
        const match = line.match(/([\^+!#<>]*)([\da-zA-Z]+)::([\s\S]+)/);
        if (!match) {
            return null;
        }
        const ret = {
            modifiers: (match[1].match(/[<>]?[\^+!#]/g) || []).map(
                modifier2PlainText
            ),
            key: match[2].toUpperCase(),
            // local key_press_pattern="{([^-]+)-([%d%a]+)}";
            // "[^{}]+","{%a+}",
            expression: matchSubPattern(match[3], [
                { reg: /^[^{}]+/, mode: "print" },
                { reg: /^{([A-Za-z]+)}/, mode: 'press-key' },
                { reg: /^{([^-]+)-([\da-zA-Z]+)}/, mode: 'press-toggle' }])
        };

        return ret;
    })
    .filter(line => line);

export default {
    name: "MacroList",
    data() {
        return {
            msg: "Welcome to Your Vue.js App",
            list
        };
    },
    components: {
        MacroLine
    }
};
