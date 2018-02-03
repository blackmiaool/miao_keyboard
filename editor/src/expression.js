import { printableKeyMap, match2arr, code2short } from "@/common";

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
