// import MacroLine from "@/components/macro-line";
import Vue from "vue";
import draggable from "vuedraggable";
import ExpressionComp from "@/components/expression";
import Expression from "@/expression";
import RuleEditor from "@/components/rule-editor";
import { shortModifierMap, modifierMap, ascii2usb, code2usb } from "@/common";
import Undo from "../undo";
import Rule from "../rule";

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

const list = config
    .split(/\r?\n/)
    .map(line => {
        if (!Rule.format0Reg.test(line)) {
            return null;
        }

        return Rule.fromFormat0(line);
    })
    .filter(line => line);
// list.forEach(line => {
//     line.expression.data.map(assignUid);
// });
// eslint-disable-next-line no-new
const listUndo = new Undo({ data: list });

export default {
    name: "MacroList",
    data() {
        return { list, modifierMap, listUndo };
    },
    mounted() {
        window.a = () => {
            console.log(this.list);
        };
        this.listUndo
            .register({
                name: "delete line",
                exec(lineNum) {
                    const line = this.data.splice(lineNum, 1);
                    return line;
                },
                undo(lineNum, line) {
                    this.data.splice(lineNum, 0, ...line);
                }
            })
            .register({
                name: "edit",
                exec({ index, data }) {
                    console.log("splice", data);
                    return this.data.splice(index, 1, data);
                },
                undo({ index }, last) {
                    this.data.splice(index, 1, ...last);
                }
            });
    },
    methods: {
        add() {
            this.list.push({});
        },
        deleteLine(row) {
            const index = this.list.indexOf(row);
            console.log("index", index);
            this.listUndo.exec("delete line", index);
            // this.listUndo.
            // this.list.splice()
        },
        edit(row) {
            this.$refs.table.toggleRowExpansion(row);
        },
        onSave(oldRow, newRow) {
            // Object.assign(oldRow, newRow);
            this.$refs.table.toggleRowExpansion(oldRow);
            const index = this.list.indexOf(oldRow);
            this.listUndo.exec("edit", { index, data: newRow });
        },
        exportConfig() {
            const txt = this.list
                .map(li => {
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
                })
                .join("\n");
            // console.log(JSON.stringify(this.list));
            console.log(txt);
        }
    },
    components: {
        // MacroLine,
        ExpressionComp,
        RuleEditor,
        draggable
    }
};
