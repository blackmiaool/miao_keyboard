export const shortModifierMap = {
    "^": "Control",
    "!": "Alt",
    "+": "Shift",
    "#": "Meta"
};
export const ascii2usb = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    44, 30, 52, 32, 33, 34, 36, 52, 38, 39, 37, 46, 54, 86, 55, 56,
    39, 30, 31, 32, 33, 34, 35, 36, 37, 38, 51, 51, 54, 46, 55, 56,
    31, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 0, 0, 0, 0, 0,
    53, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 0, 50, 0, 53, 0];
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
export const code2usb = {
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
export const modifierMap = {
    Control: 1 + 16,
    Shift: 2 + 32,
    Alt: 4 + 64,
    Win: 8 + 128,
    LControl: 1,
    LShift: 2,
    LAlt: 4,
    LMeta: 8,
    RControl: 16,
    RShift: 32,
    RAlt: 64,
    RMeta: 128
};
export const printableKeyMap = {
    leftbracket: '{',
    rightbracket: '}',
};
export function match2arr(str, reg) {
    const ret = [];
    let result;
    // eslint-disable-next-line no-cond-assign
    while (result = reg.exec(str)) {
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
export const shiftTable = [
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0];
