import Vuex from "vuex";
import Vue from "vue";
import { getLuaDeclaration } from "@/common";
import KBMode from './kbmode';
import Rule from "./rule";
import MacroList from "./modules/macro-list";


Vue.use(Vuex);

const modes = [

];

const luaScript = require("@/../../udisk/main.lua");

function getConfigJson(state) {
    const obj = {};
    obj.modes = state.modes.map((mode) => {
        return mode.toPlainObject();
    });
    obj.macro = state.MacroList.list;
    console.log(state.MacroList.list);
    return `--[[
        ${JSON.stringify(obj, false, 4)}
    ]]--
    `;
}
const store = new Vuex.Store({
    state: {
        ahk: {},
        modes,
        script: '',
    },
    mutations: {
        importConfig(state, config) {
            config.modes.forEach((mode, i) => {
                mode.index = i;
            });
            console.log(config, state);
            state.modes = config.modes.map(mode => new KBMode(mode));
            state.MacroList.list = config.macro.map(li => new Rule(li));
        },
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
            KBMode.modesGetLua(this.state.modes, declarations);
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

    },
    modules: {
        MacroList
    }
});

export default store;
