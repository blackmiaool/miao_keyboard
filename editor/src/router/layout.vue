<template>
    <div class="comp-layout window">
        <Keyboard :map="kbMap" :layout="kbLayout" />

    </div>
</template>

<script>
import Keyboard from "@/components/keyboard";

// prettier-ignore
const kbMap = [[
['Escape', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'Backspace'],
['Tab', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\'],
['CapsLock', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 'Enter'],
['ShiftLeft', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 'ShiftRight'],
['ControlLeft', 'MetaLeft', 'ArrowUp', 'AltLeft', 'Space', 'Space', 'ControlRight', 140, 141, 135, 136]],

[0, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 0,
0, 124, 125, 126, 0, 0, 0, 0, 0, 18, 127, 129, 128, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],

[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 70,
0, 0, 0, 0, 0, 0, 0, 80, 81, 79, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]];

// prettier-ignore
const kbLayout = [
    [{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2 }],
    [{ w: 1.5 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 1.5 }],
    [{ w: 1.75 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.25, l: 2 }],
    [{ w: 2.25, l: 2 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.75, l: 2 }],
    [{ w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.75, l: 2 }, { w: 1.75, l: 2 }, { style: { "margin-left": 0.25 }, w: 1.25, l: 2 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }]
];

export default {
    methods: {},
    data() {
        return { kbMap: kbMap[0], kbLayout };
    },
    components: { Keyboard }
};
</script>

<style scoped lang="less">
.window {
    background-color: rgba(255, 255, 255, 0.8);
    display: inline-block;
    padding: 20px;
    border-radius: 5px;
}
</style>
