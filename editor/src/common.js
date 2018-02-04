export const shortModifierMap = {
    "^": "Control",
    "!": "Alt",
    "+": "Shift",
    "#": "Meta"
};

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
