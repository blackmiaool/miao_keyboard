import { shortModifierMap, ascii2usb, key2usb, code2usb } from "@/common";
import Expression from "@/expression";

function modifier2PlainText(modifier) {
    let ret = "";
    if (modifier[0] === "<") {
        ret = "Left";
    } else if (modifier[0] === ">") {
        ret = "Right";
    }
    ret = shortModifierMap[modifier[modifier.length - 1]] + ret;
    return ret;
}
export default class Rule {
    constructor(
        { modifiers, key, expression } = {
            modifiers: [],
            expression: new Expression("")
        }
    ) {
        Object.assign(this, { modifiers, key, expression });
    }
    static format0Reg = /([\^+!#<>]*)([\da-zA-Z]+)::([\s\S]+)/;
    static fromFormat0(input) {
        const match = input.match(Rule.format0Reg);
        const modifiers = (match[1].match(/[<>]?[\^+!#]/g) || []).map(
            modifier2PlainText
        );
        const key = match[2].toUpperCase();

        const expression = new Expression(match[3]);
        return new Rule({ modifiers, key, expression });
    }
    getModifersUSB() {
        const modifiers = this.modifiers.reduce((p, modifier) => {
            // eslint-disable-next-line no-bitwise
            return p | code2usb[modifier];
        }, 0);
        return modifiers;
    }
    getKeyUSB() {
        return Rule.key2usb(this.key);
    }
    static key2usb(key) {
        let usb;

        if (key.length === 1) {
            const ascii = key.charCodeAt(0);
            usb = ascii2usb[ascii];
        } else if (key.match(/^\d+$/)) {
            usb = key * 1;
        } else {
            usb = key2usb[key];
        }
        return usb;
    }
}
