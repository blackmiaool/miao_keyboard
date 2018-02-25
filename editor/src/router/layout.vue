<template>
    <div class="comp-layout window">
        <Keyboard :map="selectedMode.map" :layout="kbLayout" :basicMap="modes[0].map" @unselectKey="unselectKey" @selectKey="selectKey" />
        <KeyboardEditor v-if="selectedKey" v-model="selectedMode.map[selectedKey.x][selectedKey.y]" />
        <div class="mode-wrap">
            <div class="mode-select">
                <div class="mode clickable" v-for="(mode,i) in modes" :key="i" @click="selectedMode=mode" :class="{selected:selectedMode==mode}">{{mode.name}}</div>
            </div>
            <div class="mode-editor">
                <ModeEditor :mode="selectedMode" />
            </div>
        </div>
    </div>
</template>

<script>
import Keyboard from "@/components/keyboard";
import KeyboardEditor from "@/components/keyboard-editor";
import ModeEditor from "@/components/mode-editor";

const modes = [
    {
        name: "basic",
        macro: true,
        // prettier-ignore
        map: [
['Escape', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'Backspace'],
['Tab', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\'],
['CapsLock', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 'Enter'],
['ShiftLeft', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 'ShiftRight'],
['ControlLeft', 'MetaLeft', 'ArrowUp', 'AltLeft', 'Space', 'Space', 'ControlRight', 140, 141, 135, 136]]
    },
    {
        name: "mode2",
        trigger: "pressing",
        triggerKey: 135,
        macro: true,
        // prettier-ignore
        map: [
[null, 'F1', 'F2', 'F3', 'F4', 'F5', 'F6', 'F7', 'F8', 'F9', 'F10', 'F11', 'F12', null],
[null, 'BrightDown', 'BrightUp', 126, null, null, null, null, null, null, 'Mute', 'VolumeDown', 'VolumeUp', null],
[null, null, null, null, null, null, null, null, null, null, null, null, null],
[null, null, null, null, null, null, null, null, null, null, null, null],
[null, null, null, null, null, null, null, null, null, null, null]]
    },
    {
        name: "game",
        trigger: "toggle",
        triggerKey: 136,
        macro: false,
        // prettier-ignore
        map: [
[null, null, null, null, null, null, null, null, null, null, null, null, null, 'Delete'],
[null, null, null, null, null, null, null, null, 'ArrowUp', null, null, null, null, 'PrintScreen'],
[null, null, null, null, null, null, null, 'ArrowLeft', 'ArrowDown', 'ArrowRight', null, null, null],
[null, null, null, null, null, null, null, null, null, null, null, null],
[null, null, null, null, null, null, null, null, null, null, null]]
    }
];
// // prettier-ignore
// const kbMap = [[
// ['Escape', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'Backspace'],
// ['Tab', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\'],
// ['CapsLock', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 'Enter'],
// ['ShiftLeft', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 'ShiftRight'],
// ['ControlLeft', 'MetaLeft', 'ArrowUp', 'AltLeft', 'Space', 'Space', 'ControlRight', 140, 141, 135, 136]],

// [0, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 0,
// 0, 124, 125, 126, 0, 0, 0, 0, 0, 18, 127, 129, 128, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],

// [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 70,
// 0, 0, 0, 0, 0, 0, 0, 80, 81, 79, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]];

// prettier-ignore
const kbLayout = [
    [{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2 }],
    [{ w: 1.5 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 1.5 }],
    [{ w: 1.75 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.25, l: 2 }],
    [{ w: 2.25, l: 2 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.75, l: 2 }],
    [{ w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.75, l: 2 }, { w: 1.75, l: 2 }, { style: { "margin-left": 0.25 }, w: 1.25, l: 2 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }]
];

export default {
    methods: {
        selectKey(x, y) {
            console.log(x, y);
            this.selectedKey = { x, y };
        },
        unselectKey() {
            this.selectKey = null;
        }
    },
    created() {
        this.selectedMode = modes[0];
    },
    mounted() {},
    data() {
        return {
            kbLayout,
            selectedMode: undefined,
            modes,
            selectedKey: null
        };
    },
    components: { Keyboard, ModeEditor, KeyboardEditor }
};
</script>

<style scoped lang="less">
.window {
    background-color: rgba(255, 255, 255, 0.8);
    display: inline-block;
    padding: 20px;
    border-radius: 5px;
}
.mode-wrap {
    margin-top: 20px;
    display: flex;
    flex-direction: row;
    .mode-select {
        display: inline-flex;
        flex-direction: column;
        margin-top: 10px;

        .mode {
            overflow: hidden;
            text-overflow: ellipsis;
            padding: 5px 10px;
            background-color: rgba(255, 255, 255, 0.7);
            display: inline-block;
            border-radius: 3px;
            border-top-right-radius: 0;
            border-bottom-right-radius: 0;
            border: 1px #aaa solid;
            margin-bottom: -1px;
            max-width: 100px;
            &.selected {
                border-right: none;
                background-color: #0077f3;
                color: white;
            }
        }
    }
    .mode-editor {
        flex: 1;
        background-color: rgba(255, 255, 255, 0.7);
        padding: 20px 10px;
        border-radius: 5px;
    }
}
</style>
