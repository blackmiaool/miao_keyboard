// import MacroLine from "@/components/macro-line";
// import Vue from "vue";
import draggable from "vuedraggable";
import ExpressionComp from "@/components/expression";
// import Expression from "@/expression";
import RuleEditor from "@/components/rule-editor";
import store from "@/store";
import { modifierMap } from "@/common";
import Undo from "../undo";
import Rule from "../rule";

let config = `
<+6::694421865
!<+a::blackmiaool
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

// console.log(luaScript);

export default {
    name: "MacroList",
    data() {
        return { list, modifierMap, listUndo, output: "" };
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
            })
            .register({
                name: "add",
                exec() {
                    this.data.push(new Rule());
                },
                undo() {
                    this.data.pop();
                }
            });
    },
    methods: {
        connect() {
            let device;
            navigator.usb
                .requestDevice({ filters: [{ vendorId: 0x0483 }] })
                .then(deviceThis => {
                    device = deviceThis;
                    console.log(device.productName);
                    console.log(device.manufacturerName);
                    window.usbdevice = device;
                    return device.open();
                })
                // .then(() => device.selectConfiguration(1)) // Select configuration #1 for the device.
                .then(() => device.claimInterface(2)) // Request exclusive control over interface #2.
                .then(() => device.transferOut(4, new ArrayBuffer(64)))
                // .then(() => {
                //     // setInterval(() => {
                //     return device.transferIn(3, 64).then(() => {
                //         console.log("!!!!");
                //     });
                //     // }, 10);
                // })
                // .then(() =>
                //     device.controlTransferOut({
                //         requestType: "class",
                //         recipient: "interface",
                //         request: 0x22,
                //         value: 0x01,
                //         index: 0x01
                //     })
                // )
                // .then(() => device.transferIn(4, 64)) // Waiting for 64 bytes of data from endpoint #5.
                .then(result => {
                    const decoder = new TextDecoder();
                    console.log(`Received: ${decoder.decode(result.data)}`);
                })
                .catch(error => {
                    console.log(error);
                });
        },
        add() {
            this.listUndo.exec("add");
        },
        deleteLine(row) {
            const index = this.list.indexOf(row);
            this.listUndo.exec("delete line", index);
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
            const map = {};
            this.list.forEach(rule => {
                const modifiers = rule.getModifersUSB();
                const keyCode = rule.getKeyUSB();

                if (!map[keyCode]) {
                    map[keyCode] = {};
                }
                map[keyCode][modifiers] = rule.expression.toPlainText();
            });
            store.commit("setAHK", map);
            this.output += store.getters.script;
            download("main.lua", store.getters.script);
            function download(filename, text) {
                const element = document.createElement("a");
                element.setAttribute(
                    "href",
                    `data:text/plain;charset=utf-8,${encodeURIComponent(text)}`
                );
                element.setAttribute("download", filename);

                element.style.display = "none";
                document.body.appendChild(element);

                element.click();

                document.body.removeChild(element);
            }
        }
    },
    components: {
        // MacroLine,
        ExpressionComp,
        RuleEditor,
        draggable
    }
};
