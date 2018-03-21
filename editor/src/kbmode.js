// import { key2usb, consumer2usb, getModeFromModeTrigger, isModeTrigger } from "@/common";
import Rule from "@/rule";
import { leftPadding, luaStringify, isModeTrigger, getModeFromModeTrigger, consumer2usb } from "./common";

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
    getUSBMap() {
        const ret = [];
        kbLayout.forEach((lineLayout, lineIndex) => {
            const line = [];
            ret.push(line);
            lineLayout.forEach((key, keyIndex) => {
                let len = 1;
                if (key.l) {
                    len = key.l;
                }
                for (let i = 0; i < len; i++) {
                    const keyName = this.map[lineIndex][keyIndex];
                    // if (!keyName) {
                    //     keyName = KBMode.basicMode.map[lineIndex][keyIndex];
                    // }

                    const usbCode = Rule.key2usb(keyName);
                    if (usbCode) {
                        line.push(usbCode);
                    } else {
                        line.push(keyName);
                    }

                    // if (!usbCode && keyName) {
                    //     if (inheritance) {

                    //     }
                    //     usbCode = getSpecialCode(keyName);
                    // }
                    // if (key2usb[keyName]) {
                    //     usbCode = key2usb[keyName];
                    // } else if (consumer2usb[keyName]) {
                    //     usbCode = getSpecialCode(keyName);
                    // }
                }
            });
        });
        return ret;
    }
    static modesGetLua(modes) {
        let specialCode = 128;
        const consumerCodeMap = {};
        const modeTriggerMap = {};

        function getSpecialCode(name) {
            const ret = specialCode++;
            if (isModeTrigger(name)) {
                modeTriggerMap[ret] = getModeFromModeTrigger(name);
            } else if (consumer2usb[name]) {
                consumerCodeMap[ret] = consumer2usb[name];
            } else {
                console.warn('unknown key', name);
            }
            return ret;
        }
        const maps = modes.map(mode => mode.getUSBMap());
        const basicMap = maps[0];

        let ret = maps.reduce((pMap, map) => {
            const modeStr = map.reduce((pLine, line, row) => {
                const lineStr = line.reduce((pp, key, column) => {
                    let code = key;
                    if (typeof key === 'string') { // special key
                        code = getSpecialCode(key);
                        map[row][column] = code;
                    } else if (key === null) {
                        if (basicMap[row][column]) {
                            code = basicMap[row][column];
                        }
                    }
                    return `${pp + leftPadding(code, 3)}, `;
                }, '');
                return `${pLine + lineStr}\n`;
            }, '');
            return `${pMap + modeStr}\n`;
        }, '');
        console.log(consumerCodeMap);
        ret = ret.trim();
        ret = `
local kb_index=[[${ret}]];
local consumer_map=${luaStringify(consumerCodeMap)};
local mode_trigger_map=${luaStringify(modeTriggerMap)};
`;
        console.log(ret);
        return ret;
    }
    static basicMode = null
}
