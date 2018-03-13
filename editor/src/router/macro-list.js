// import MacroLine from "@/components/macro-line";
// import Vue from "vue";
import draggable from "vuedraggable";
import { createNamespacedHelpers } from 'vuex';
import ExpressionComp from "@/components/expression";
// import Expression from "@/expression";
import RuleEditor from "@/components/rule-editor";
import Undo from "../undo";
import Rule from "../rule";

const { mapState, mapActions, mapMutations } = createNamespacedHelpers('MacroList');

// eslint-disable-next-line no-new
// const listUndo = new Undo({ data: list });

// console.log(luaScript);

export default {
    name: "MacroList",
    data() {
        return { output: "" };
    },
    computed: {
        ...mapState(['list', 'canUndo', 'canRedo'])
    },
    mounted() {
        window.a = () => {
            console.log(this.list);
        };
        // this.listUndo
        //     .register({
        //         name: "delete line",
        //         exec(lineNum) {
        //             const line = this.data.splice(lineNum, 1);
        //             return line;
        //         },
        //         undo(lineNum, line) {
        //             this.data.splice(lineNum, 0, ...line);
        //         }
        //     })
        //     .register({
        //         name: "edit",
        //         exec({ index, data }) {
        //             console.log("splice", data);
        //             return this.data.splice(index, 1, data);
        //         },
        //         undo({ index }, last) {
        //             this.data.splice(index, 1, ...last);
        //         }
        //     })
        //     .register({
        //         name: "add",
        //         exec() {
        //             this.data.push(new Rule());
        //         },
        //         undo() {
        //             this.data.pop();
        //         }
        //     });
    },
    methods: {
        ...mapActions(['undo', 'redo']),
        ...mapMutations(['remove', 'add', 'set']),
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
        getIndexOfRow(row) {
            return this.list.indexOf(row);
        },
        deleteLine(row) {
            const index = this.getIndexOfRow(row);
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
            this.$store.commit("setAHK", map);
            this.output += this.$store.getters.script;
            download("main.lua", this.$store.getters.script);
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
