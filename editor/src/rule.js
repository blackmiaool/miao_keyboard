import { shortModifierMap } from "@/common";
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
    constructor() {}
    static format0Reg = /([\^+!#<>]*)([\da-zA-Z]+)::([\s\S]+)/;
    static fromFormat0(input) {
        const match = input.match(Rule.format0Reg);
        const modifiers = (match[1].match(/[<>]?[\^+!#]/g) || []).map(
            modifier2PlainText
        );
        const key = match[2].toUpperCase();
        const expression = new Expression(match[3]);
        return { modifiers, key, expression };
    }
}
