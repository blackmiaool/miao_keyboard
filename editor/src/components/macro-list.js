import MacroLine from "@/components/macro-line";
import ExpressionComp from "@/components/expression";
import Expression from "@/expression";
import RuleEditor from "@/components/rule-editor";
import {
    shortModifierMap,
    modifierMap,
    ascii2usb,
    code2usb
} from "@/common";

let config = `!<+a::blackmiaool
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
>+q::~
>+w::\` 
<+i::@xiaomi.com
<+b::blackmiaool
<+c::console.log(){left}
<+v::console.warn(){left}
<+t::this.state.
<+w::ilwt.science
<+o::const{space}
<+u::sudo{space}apt-get{space}install
<+n::(()=>{leftbracket}{rightbracket});{left}{left}{left}{enter}
140::{<^-down}{left}
141::{<^-down}{right}
129::{VolumeDown}
128::{VolumeUp}
127::{Mute}
124::{BrightDown}
125::{BrightUp}
# <: left modifier key
# >: right modifier key
# ^: ctrl
# !: alt
# +: shift
# #: win`;
config = config
    .replace(/{space}/g, "{Space}")
    .replace(/{esc}/g, "{Escape}")
    .replace(/{down}/g, "{ArrowDown}")
    .replace(/{up}/g, "{ArrowUp}")
    .replace(/{right}/g, "{ArrowRight}")
    .replace(/{left}/g, "{ArrowLeft}")
    .replace(/{home}/g, "{Home}")
    .replace(/{end}/g, "{End}")
    .replace(/{deleteforward}/g, "{Delete}")
    .replace(/{pagedown}/g, "{PageDown}")
    .replace(/{pageup}/g, "{PageUp}")
    .replace(/{printscreen}/g, "{PrintScreen}")
    .replace(/{enter}/g, "{Enter}");

function modifier2PlainText(modifier) {
    let ret = '';
    if (modifier[0] === "<") {
        ret = "Left";
    } else if (modifier[0] === ">") {
        ret = "Right";
    }
    ret = shortModifierMap[modifier[modifier.length - 1]] + ret;
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
            expression: (new Expression(match[3])),
        };

        return ret;
    })
    .filter(line => line);
console.log('list2', list);
export default {
    name: "MacroList",
    data() {
        return {
            msg: "Welcome to Your Vue.js App",
            list,
            modifierMap,
        };
    },
    mounted() {
        window.a = () => {
            console.log(this.list);
        };
    },
    methods: {
        edit(row) {
            this.$refs.table.toggleRowExpansion(row);
        },
        onSave(oldRow, newRow) {
            Object.assign(oldRow, newRow);
            this.$refs.table.toggleRowExpansion(oldRow);
        },
        exportConfig() {
            console.log(this.list);
            const txt = this.list.map((li) => {
                const modifiers = li.modifiers.reduce((p, modifier) => {
                    // eslint-disable-next-line no-bitwise
                    return p | code2usb[modifier];
                }, 0);
                let keyCode;
                if (li.key.match(/^\d{2,3}$/)) {
                    keyCode = li.key;
                } else {
                    keyCode = ascii2usb[li.key.charCodeAt(0)];
                }
                return `${modifiers}@${keyCode}@${li.expression.toPlainText()}`;
            }).join('\n');
            // console.log(JSON.stringify(this.list));
            console.log(txt);
        }
    },
    components: {
        MacroLine,
        ExpressionComp,
        RuleEditor
    }
};
