import Vuex from "vuex";
import Vue from "vue";
import KBMode from './kbmode';

Vue.use(Vuex);

const modes = [
    new KBMode({
        isBasic: true,
        macro: true,
        index: 0,
        // prettier-ignore
        map: [
            ['Escape', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 'Backspace'],
            ['Tab', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\\'],
            ['CapsLock', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', 'Enter'],
            ['ShiftLeft', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 'ShiftRight'],
            ['ControlLeft', 'MetaLeft', 'ArrowUp', 'AltLeft', 'Space', 'Space', 'ControlRight', 140, 141, 'mode1', 'mode2']]
    }),
    new KBMode({
        trigger: "pressing",
        triggerKey: 135,
        macro: true,
        index: 1,
        // prettier-ignore
        map: [
            [null, 'F1', 'F2', 'F3', 'F4', 'F5', 'F6', 'F7', 'F8', 'F9', 'F10', 'F11', 'F12', null],
            [null, 'BrightDown', 'BrightUp', null, null, null, null, null, null, null, null, 'VolumeDown', 'VolumeUp', 'Mute'],
            [null, null, null, null, null, null, null, null, null, null, null, null, null],
            [null, null, null, null, null, null, null, null, null, null, null, null],
            [null, null, null, null, null, null, null, null, null, null, null]]
    }),
    new KBMode({
        trigger: "toggle",
        triggerKey: 136,
        macro: false,
        index: 2,
        // prettier-ignore
        map: [
            [null, null, null, null, null, null, null, null, null, null, null, null, null, 'Delete'],
            [null, null, null, null, null, null, null, null, 'ArrowUp', null, null, null, null, 'PrintScreen'],
            [null, null, null, null, null, null, null, 'ArrowLeft', 'ArrowDown', 'ArrowRight', null, null, null],
            [null, null, null, null, null, null, null, null, null, null, null, null],
            [null, null, null, null, null, null, null, null, null, null, null]]
    })
];
const luaScript = require("@/../../udisk/main.lua");

const store = new Vuex.Store({
    state: {
        ahk: {},
        modes
    },
    mutations: {
        setAHK(state, ahk) {
            state.ahk = ahk;
        }
    },
    getters: {
        script(state) {
            console.log("getter");
            let ret = "";
            const ahkLuaTable = JSON.stringify(state.ahk, undefined, 4).replace(
                /^(\s*)"(\d+)":/gm,
                (all, indent, num) => `${indent}[${num}]=`
            );
            ret = "";
            ret += "local ahk_data=";
            ret += ahkLuaTable;
            ret += "\n";
            ret += luaScript;
            return ret;
        }
    }
});

export default store;
