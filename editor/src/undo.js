class Undo {
    constructor({ data }) {
        this.commands = {};
        this.history = [];
        this.stackPosition = 0;
        this.data = data;
    }
    setCurrentRecord(record) {
        this.history[this.stackPosition - 1] = record;
    }
    getCurrentRecord() {
        return this.history[this.stackPosition - 1];
    }
    exec(commandName, arg) {
        const command = this.commands[commandName];
        const result = command.exec.call(this, arg);
        this.stackPosition++;
        this.setCurrentRecord({ commandName, arg, result });
        this.history.splice(this.stackPosition);
    }
    undo() {
        if (!this.canUndo()) {
            return;
        }
        const record = this.getCurrentRecord();
        const command = this.commands[record.commandName];
        command.undo.call(this, record.arg, record.result);
        this.stackPosition--;
    }
    canUndo() {
        return this.stackPosition > 0;
    }
    redo() {
        if (!this.canRedo()) {
            return;
        }
        this.stackPosition++;
        const record = this.getCurrentRecord();
        const command = this.commands[record.commandName];
        command.exec.call(this, record.arg, record.result);
    }
    canRedo() {
        return this.stackPosition < this.history.length;
    }
    register(command) {
        this.commands[command.name] = command;
    }
}
// const undo = new Undo({
//     data: [1, 2, 3]
// });
// undo.register({
//     name: "delete line",
//     exec(lineNum) {
//         const line = this.data.splice(lineNum, 1);
//         return line;
//     },
//     undo(lineNum, line) {
//         this.data.splice(lineNum, 0, ...line);
//     }
// });
// undo.exec("delete line", 2);
// console.log(undo);
export default Undo;
