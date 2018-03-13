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

        const random = Date.now() + Math.random();
        VuexList.commands.forEach((command) => {
            this.mutations[command.name] = (state, payload) => {
                this.state.canUndo = true;
                const result = command.exec.call(this, state.list, payload);
                this.stackPosition++;
                this.setCurrentRecord({ commandName: command.name, payload, result });
                this.history.splice(this.stackPosition);
                if (!this.canRedo()) {
                    this.state.canRedo = false;
                }
            };
            this.mutations[`${command.name}${random}`] = (state, payload) => {
                this.data = state.list;
                command.undo.call(this, state.list, payload.payload, payload.result);
                this.stackPosition--;
            };
        });
        this.mutations.setUndoState = function setUndoState(state, value) {
            console.log('value', value);
            state.canUndo = value;
        };
        this.mutations.setRedoState = function setRedoState(state, value) {
            state.canRedo = value;
        };
        this.random = random;
        this.commands = {};
        this.history = [];
        this.stackPosition = 0;
        console.log(this);
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
    getCurrentRecord(bias = 0) {
        return this.history[this.stackPosition - 1 + bias];
    }
    undo(context) {
        if (!this.canUndo()) {
            return;
        }
        const record = this.getCurrentRecord();
        context.commit(record.commandName + this.random, { payload: record.payload, result: record.result });

        if (!this.canUndo()) {
            context.commit('setUndoState', false);
        }
        context.commit('setRedoState', true);
    }
    canUndo() {
        return this.stackPosition > 0;
    }
    redo(context) {
        if (!this.canRedo()) {
            return;
        }
        const record = this.getCurrentRecord(1);
        context.commit(record.commandName, record.payload);

        if (!this.canRedo()) {
            context.commit('setRedoState', false);
        }
        context.commit('setUndoState', true);
    }
    canRedo() {
        return this.stackPosition < this.history.length;
    }
}
