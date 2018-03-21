export const shortModifierMap = {
    "^": "Control",
    "!": "Alt",
    "+": "Shift",
    "#": "Meta"
};
// prettier-ignore
export const shiftTable = [
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0];
// prettier-ignore
export const ascii2usbRaw = [
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    44, 30, 52, 32, 33, 34, 36, 52, 38, 39, 37, 46, 54, 86, 55, 56,
    39, 30, 31, 32, 33, 34, 35, 36, 37, 38, 51, 51, 54, 46, 55, 56,
    31, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 47, 49, 48, 0, 0,
    53, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 47, 49, 48, 53, 0];

export const ascii2usb = ascii2usbRaw.reduce((pre, v, i) => {
    if (v) {
        pre[i] = v;
    }
    return pre;
}, {});

export const baseAscii2usb = ascii2usbRaw.reduce((pre, v, i) => {
    if (v && !shiftTable[i] && /\S/.test(String.fromCharCode(i))) {
        pre[i] = v;
    }
    return pre;
}, {});

export const usb2ascii = {};
ascii2usbRaw.forEach((num, i) => {
    if (!shiftTable[i]) {
        usb2ascii[num] = i;
    }
});
export const code2short = {
    ControlLeft: "<^",
    ControlRight: ">^",
    Control: "^",
    AltLeft: "<!",
    AltRight: ">!",
    Alt: "!",
    ShiftLeft: "<+",
    ShiftRight: ">+",
    Shift: "+",
    MetaLeft: "<#",
    MetaRight: ">#",
    Meta: "#"
};
export const shortMap = {
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
export const consumer2usb = {
    VolumeDown: 0xea,
    VolumeUp: 0xe9,
    Mute: 0xe2,
    // macbook only
    BrightDown: 0x70,
    // macbook only
    BrightUp: 0x6f
};
export const modifier2ahk = {
    ControlLeft: 1,
    ControlRight: 16,
    Control: 1 + 16,
    AltLeft: 4,
    AltRight: 64,
    Alt: 4 + 64,
    ShiftLeft: 2,
    ShiftRight: 32,
    Shift: 2 + 32,
    MetaLeft: 8,
    MetaRight: 128,
    Meta: 8 + 128
};
export const modifier2usb = {
    ControlLeft: 224,
    ControlRight: 228,
    AltLeft: 226,
    AltRight: 230,
    ShiftLeft: 225,
    ShiftRight: 229,
    MetaLeft: 227,
    MetaRight: 231,
};
export const printableKeyMap = {
    leftbracket: "{",
    rightbracket: "}"
};
export function match2arr(str, reg) {
    const ret = [];
    let result;
    // eslint-disable-next-line no-cond-assign
    while ((result = reg.exec(str))) {
        ret.push(result.slice(1));
    }
    return ret;
}
export const code2modifier = {
    ControlLeft: "LControl",
    ControlRight: "RControl",
    AltLeft: "LAlt",
    AltRight: "RAlt",
    ShiftLeft: "LShift",
    ShiftRight: "RShift",
    MetaLeft: "LWin",
    MetaRight: "RWin"
};
export const key2usb = {
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
    NumLock: 83
};
export const key2icon = {
    VolumeUp: 'fa fa-volume-up',
    VolumeDown: 'fa fa-volume-down',
    Mute: 'fa fa-volume-off',
    BrightDown: 'fa fa-thermometer-quarter',
    BrightUp: 'fa fa-thermometer-full'
};
export const key2short = {
    Escape: "Esc",
    ";": ":\n;",
    "'": `"\n'`,
    ",": `<\n,`,
    ".": `>\n.`,
    "/": `?\n/`,
    "]": "}\n]",
    "[": "{\n[",
    "\\": "|\n\\",
    1: "!\n1",
    2: "@\n2",
    3: "#\n3",
    4: "$\n4",
    5: "%\n5",
    6: "^\n6",
    7: "&\n7",
    8: "*\n8",
    9: "(\n9",
    0: ")\n0",
    "-": "_\n-",
    "=": "+\n=",
    "`": '~\n`',
    ShiftLeft: "LShift",
    ShiftRight: "RShift",
    MetaLeft: "LMeta",
    MetaRight: "RMeta",
    ControlLeft: "LCtrl",
    ControlRight: "RCtrl",
    AltLeft: "LAlt",
    AltRight: "RAlt",
    ArrowUp: "↑",
    ArrowDown: "↓",
    ArrowRight: "→",
    ArrowLeft: "←",
    PrintScreen: "PrtScn"
};
export function isModeTrigger(key) {
    return /^mode\d+$/.test(key);
}
export function getModeFromModeTrigger(key) {
    try {
        return key.match(/(\d+)$/)[1] * 1;
    } catch (e) {
        return null;
    }
}
export function leftPadding(num, width) {
    const str = num.toString();
    if (str.length > width) {
        return num;
    }
    return '0'.repeat(width - str.length) + str;
}
export function luaStringify(obj) {
    return JSON.stringify(obj, undefined, 4).replace(
        /^(\s*)"(\d+)":/gm,
        (all, indent, num) => `${indent}[${num}]=`
    );
}
