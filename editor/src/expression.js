import { printableKeyMap, match2arr, code2short, shiftTable, ascii2usb } from "@/common";


const key2usb = {
    Enter: 40,
    Escape: 41,
    Backspace: 42,
    Tab: 43,
    Space: 44,
    CapsLock: 57,
    F1: 58,
    F2: 59,
    F3: 60,
    F4: 61,
    F5: 62,
    F6: 63,
    F7: 64,
    F8: 65,
    F9: 66,
    F10: 67,
    F11: 68,
    F12: 69,
    PrintScreen: 70,
    ScrollLock: 71,
    Pause: 72,
    Insert: 73,
    Home: 74,
    PageUp: 75,
    Delete: 76,
    End: 77,
    PageDown: 78,
    ArrowRight: 79,
    ArrowLeft: 80,
    ArrowDown: 81,
    ArrowUp: 82,
    NumLock: 83,

};
const media2usb = {
    VolumeDown: 0xea,
    VolumeUp: 0xe9,
    Mute: 0xe2,
    // macbook only
    BrightDown: 0x70,
    // macbook only
    BrightUp: 0x6f
};

export default class Expression {
    constructor(rawText) {
        if (typeof rawText === 'string') {
            this.data = Expression.matchSubPattern(rawText, Expression.expressionReg);
        } else if (Array.isArray(rawText)) {
            this.data = rawText;
        }
    }
    addPrint(text) {
        const last =
            this.data &&
            this.data[this.data.length - 1];
        if (last && last.mode === "print") {
            last.value += text;
        } else {
            this.data.push({
                mode: "print",
                value: text
            });
        }
    }
    addModifier(key, action) {
        console.log(key, action);

        const modifier = {
            mode: 'press-toggle',
            value: {
                modifiers: [code2short[key].split('')],
                action
            }
        };
        this.data.push(modifier);
    }
    addPress(key) {
        if (key === ' ') {
            key = 'Space';
        }
        this.data.push({
            mode: 'press-key',
            value: key
        });
    }
    static expressionReg = [
        { reg: /^[^{}]+/, mode: "print" },
        { reg: /^{([A-Za-z]+)}/, mode: 'press-key' },
        { reg: /^{([^-]+)-([\da-zA-Z]+)}/, mode: 'press-toggle' }];
    static matchSubPattern(expression, patterns) {
        const output = [];
        let pos = 0;
        expression = expression.trim();
        while (pos !== expression.length) {
            const strThis = expression.substr(pos);
            const found = patterns.some(({ reg, mode }) => {
                const match = strThis.match(reg);
                if (match) {
                    let foundPrint = false;
                    const last = output[output.length - 1];

                    if (mode === 'print') {
                        if (last && last.mode === 'print') {
                            last.value += match[0];
                            foundPrint = true;
                        }
                    } else if (mode === 'press-key') {
                        const toPrint = printableKeyMap[match[1]];

                        if (toPrint) {
                            foundPrint = true;
                            if (last && last.mode === 'print') {
                                last.value += toPrint;
                            } else {
                                output.push({ value: match[0], mode: 'print' });
                            }
                        }
                    }
                    if (!foundPrint) {
                        if (mode === 'print') {
                            output.push({ value: match[0], mode });
                        } else if (mode === 'press-key') {
                            output.push({ value: match[1], mode });
                        } else if (mode === 'press-toggle') {
                            const modifiers = match2arr(match[1], /([<>]?)([\^+!#])/g);
                            const action = match[2];

                            output.push({
                                value: {
                                    modifiers,
                                    action
                                },
                                mode
                            });
                        }
                    }

                    pos += match[0].length;
                    return true;
                }
                return false;
            });
            if (!found) {
                console.warn('invalid expression', expression, pos);
                break;
            }
        }
        return output;
    }
    toJSON() {
        return this.toString();
    }
    toPlainText() {
        const shortMap = {
            "^": 1 + 16,
            "+": 2 + 32,
            "!": 4 + 64,
            "#": 8 + 128,
            "<^": 1,
            "<+": 2,
            "<!": 4,
            "<#": 8,
            ">^": 16,
            ">+": 32,
            ">!": 64,
            ">#": 128
        };
        let pressingModifierCode = 0;
        return this.data.map((item) => {
            switch (item.mode) {
                case "press-key": {
                    let usbCode;
                    if (key2usb[item.value]) {
                        usbCode = key2usb[item.value];
                    } else if (media2usb[item.value]) {
                        usbCode = `M${media2usb[item.value]}`;
                    }
                    if (!usbCode) {
                        console.warn('unknown key');
                    }
                    if (pressingModifierCode === 0) {
                        return `${usbCode}`;
                    }
                    return `${pressingModifierCode},${usbCode}`;
                }
                case "print":
                    return item.value.split("").map((char) => {
                        const isShift = shiftTable[char.charCodeAt(0)];
                        const usbCode = ascii2usb[char.charCodeAt(0)];
                        if (isShift) {
                            return `${shortMap["<+"]},${usbCode}`;
                        }
                        if (pressingModifierCode === 0) {
                            return `${usbCode}`;
                        }
                        return `${pressingModifierCode},${usbCode}`;
                    }).join(' ');
                case "press-toggle":
                    {
                        const modifierValue = item.value.modifiers.reduce((p, modifier) => {
                            modifier = modifier.join('');
                            // eslint-disable-next-line no-bitwise
                            return p | shortMap[modifier];
                        }, 0);
                        if (item.value.action === 'down') {
                            // eslint-disable-next-line no-bitwise
                            pressingModifierCode |= modifierValue;
                        } else {
                            // eslint-disable-next-line no-bitwise
                            pressingModifierCode &= ~modifierValue;
                        }
                        return `${pressingModifierCode},0`;
                    }
                default:
                    console.warn('unknown mode', item.mode);
                    return '';
            }
        }).join(' ');
    }
    toString() {
        return this.data.map((item) => {
            switch (item.mode) {
                case "press-key":
                    return `{${item.value}}`;
                case "print":
                    {
                        const print = item.value.replace(/[{}]/g, (match) => {
                            if (match === '{') {
                                return '{leftbracket}';
                            } else if (match === '}') {
                                return '{rightbracket}';
                            }
                            return '';
                        });
                        return print;
                    }
                case "press-toggle":
                    return `{${item.value.modifiers.map(modifiers => modifiers.join('')).join('')}-${item.value.action}}`;
                default:
                    console.warn('unknown mode', item.mode);
                    return '';
            }
        }).join('');
    }
}