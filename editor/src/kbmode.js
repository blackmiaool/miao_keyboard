export default class KBMode {
    constructor({ isBasic, macro, map, trigger, triggerKey }) {
        this.macro = macro;
        this.map = map;

        if (isBasic) {
            KBMode.basicMode = this;
        } else {
            this.trigger = trigger;
            this.triggerKey = triggerKey;
        }
        // name: "basic",
        // macro: true,
        // // prettier-ignore
        // map: [
        //     ['Escape', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'Backspace'],
        //     ['Tab', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\'],
        //     ['CapsLock', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 'Enter'],
        //     ['ShiftLeft', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 'ShiftRight'],
        //     ['ControlLeft', 'MetaLeft', 'ArrowUp', 'AltLeft', 'Space', 'Space', 'ControlRight', 140, 141, 135, 136]]
    }
    static basicMode = null
}
