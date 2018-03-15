import { key2usb, consumer2usb } from "@/common";
import Rule from "@/rule";

// prettier-ignore
export const kbLayout = [
    [{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2 }],
    [{ w: 1.5 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 1.5 }],
    [{ w: 1.75 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.25, l: 2 }],
    [{ w: 2.25, l: 2 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.75, l: 2 }],
    [{ w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.75, l: 2 }, { w: 1.75, l: 2 }, { style: { "margin-left": 0.25 }, w: 1.25, l: 2 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }]
];
export default class KBMode {
    constructor({ index, isBasic, macro, map, trigger, triggerKey }) {
        this.macro = macro;
        this.map = map;
        this.isBasic = isBasic;
        this.index = index;
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
    mapToString(getSpecialCode) {
        let ret = '';
        kbLayout.forEach((lineLayout, lineIndex) => {
            lineLayout.forEach((key, keyIndex) => {
                let len = 1;
                if (key.l) {
                    len = key.l;
                }
                for (let i = 0; i < len; i++) {
                    const keyName = this.map[lineIndex][keyIndex];

                    const usbCode = Rule.key2usb(keyName);
                    // if (key2usb[keyName]) {
                    //     usbCode = key2usb[keyName];
                    // } else if (consumer2usb[keyName]) {
                    //     usbCode = getSpecialCode(keyName);
                    // }
                    ret += `${usbCode},`;
                }
            });
            ret += '\n';
        });
        return ret;
    }
    static modesGetLua() {

    }
    static basicMode = null
}
