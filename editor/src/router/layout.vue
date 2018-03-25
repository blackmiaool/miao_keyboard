<template>
    <div class="comp-layout window">
        <Keyboard v-if="selectedMode" :map="selectedMode.map" :layout="kbLayout" @unselectKey="unselectKey" @selectKey="selectKey" />
        <KeyboardEditor v-if="selectedKey" v-model="selectedMode.map[selectedKey.x][selectedKey.y]" :baseValue="baseMap[selectedKey.x][selectedKey.y]" />
        <div class="mode-wrap">
            <div class="mode-select">
                <div class="mode clickable" v-for="(mode,i) in modes" :key="i" @click="selectedMode=mode" :class="{selected:selectedMode==mode}">{{mode.name||'mode'+i}}</div>
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
import { mapState } from "vuex";
import { kbLayout } from "@/kbmode";

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

export default {
    methods: {
        selectKey(x, y) {
            this.selectedKey = { x, y };
        },
        unselectKey() {
            this.selectedKey = null;
        }
    },
    created() {},
    computed: {
        ...mapState({
            modes: state => state.modes,
            baseMap: state => state.modes[0].map
        })
    },
    mounted() {
        if (this.modes) {
            this.selectedMode = this.modes[0];
        }
    },
    data() {
        return {
            kbLayout,
            selectedMode: undefined,
            selectedKey: null
        };
    },
    components: { Keyboard, ModeEditor, KeyboardEditor }
};
</script>

<style scoped lang="less">
.window {
    background-color: rgba(255, 255, 255, 0.8);

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
