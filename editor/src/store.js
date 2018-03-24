import Vuex from "vuex";
import Vue from "vue";
import { getLuaDeclaration } from "@/common";
import KBMode from './kbmode';
import MacroList from "./modules/macro-list";


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
            ['ControlLeft', 'MetaLeft', 'ArrowUp', 'AltLeft', 'Space', 'Space', 'ControlRight', 'customized1', 'customized2', 'mode1', 'mode2']]
    }),
    new KBMode({
        trigger: "pressing",
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

function getConfigJson() {
    return '';
}
const store = new Vuex.Store({
    state: {
        ahk: {},
        modes,
        script: '',
    },
    mutations: {
        setAHK(state, ahk) {
            state.ahk = ahk;
        },
        setScript(state, script) {
            state.script = script;
        },
    },
    actions: {
        exportConfig({ commit, state }) {
            let ret = "";
            const map = {};
            state.MacroList.list.forEach(rule => {
                const modifiers = rule.getModifersUSB();
                const keyCode = rule.getKeyUSB();

                if (!map[keyCode]) {
                    map[keyCode] = {};
                }
                map[keyCode][modifiers] = rule.expression.toPlainText();
            });
            window.modes = modes;

            const declarations = [];
            KBMode.modesGetLua(modes, declarations);
            declarations.push(['ahk_data', map]);
            const exportJson = getConfigJson(state);
            ret = exportJson;
            ret += declarations.map(([name, value]) => {
                return getLuaDeclaration(name, value);
            }).join('\n');
            console.log('ret:', ret);
            ret += luaScript;

            commit('setScript', ret);
            // return ret;
        }
    },
    getters: {
        // script(state) {
        //     console.log("getter");
        //     let ret = "";
        //     const ahkLuaTable = JSON.stringify(state.ahk, undefined, 4).replace(
        //         /^(\s*)"(\d+)":/gm,
        //         (all, indent, num) => `${indent}[${num}]=`
        //     );
        //     ret = "";
        //     ret += "local ahk_data=";
        //     ret += ahkLuaTable;
        //     ret += "\n";
        //     ret += luaScript;
        //     return ret;
        // }
    },
    modules: {
        MacroList
    }
});

export default store;
