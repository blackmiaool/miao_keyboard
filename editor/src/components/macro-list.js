import MacroLine from "@/components/macro-line";
import ExpressionComp from "@/components/expression";
import Expression from "@/expression";
import RuleEditor from "@/components/rule-editor";
import {
    shortModifierMap,
    modifierMap
} from "@/common";

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


function modifier2PlainText(modifier) {
    let ret = "";
    if (modifier[0] === "<") {
        ret += "L";
    } else if (modifier[0] === ">") {
        ret += "R";
    }
    ret += shortModifierMap[modifier[modifier.length - 1]];
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
            expression: (new Expression(match[3])).data,
            editing: {},
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
    components: {
        MacroLine,
        ExpressionComp,
        RuleEditor
    }
};
