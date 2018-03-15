export default class VuexList {
    constructor(params) {
        const parts = ['state', 'actions', 'mutations', 'getters'];
        parts.forEach((part) => {
            this[part] = {};
            if (params[part]) {
                Object.assign(this[part], params[part]);
            }
        });
        this.namespaced = true;
        this.actions.undo = this.undo.bind(this);
        this.actions.redo = this.redo.bind(this);
        this.state.canUndo = false;
        this.state.canRedo = false;

        const random = `-undo${Date.now()}${Math.random()}`;
        VuexList.commands.forEach((command) => {
            this.mutations[command.name] = (state, payload) => {
                const result = command.exec.call(this, state.list, payload);
                if (!this.inRedo) {
                    this.stackPosition++;
                }
                this.setCurrentRecord({ commandName: command.name, payload, result });
                if (!this.inRedo) {
                    this.history.splice(this.stackPosition);
                }

                if (!this.canRedo()) {
                    this.state.canRedo = false;
                }
                this.state.canUndo = true;
            };
            this.mutations[`${command.name}${random}`] = (state, payload) => {
                this.data = state.list;
                command.undo.call(this, state.list, payload.payload, payload.result);
                this.stackPosition--;

                if (!this.canUndo()) {
                    this.state.canUndo = false;
                }
                this.state.canRedo = true;
            };
        });

        this.random = random;
        this.commands = {};
        this.history = [];
        this.stackPosition = 0;
        this.inRedo = false;
    }
    static commands = [{
        name: "remove",
        exec(list, lineNum) {
            const line = list.splice(lineNum, 1);
            return line;
        },
        undo(list, lineNum, line) {
            list.splice(lineNum, 0, ...line);
        }
    }, {
        name: "set",
        exec(list, { index, data }) {
            return list.splice(index, 1, data);
        },
        undo(list, { index }, last) {
            list.splice(index, 1, ...last);
        }
    }, {
        name: "add",
        exec(list, data) {
            list.push(data);
        },
        undo(list) {
            list.pop();
        }
    }];
    setCurrentRecord(record) {
        this.history[this.stackPosition - 1] = record;
    }
    getCurrentRecord() {
        return this.history[this.stackPosition - 1];
    }
    undo(context) {
        if (!this.canUndo()) {
            return;
        }
        const record = this.getCurrentRecord();
        context.commit(record.commandName + this.random, { payload: record.payload, result: record.result });
    }
    canUndo() {
        return this.stackPosition > 0;
    }
    redo(context) {
        if (!this.canRedo()) {
            return;
        }
        this.inRedo = true;
        this.stackPosition++;
        const record = this.getCurrentRecord();

        context.commit(record.commandName, record.payload);
        this.inRedo = false;
    }
    canRedo() {
        return this.stackPosition < this.history.length;
    }
}
