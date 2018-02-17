import Vuex from "vuex";
import Vue from "vue";

Vue.use(Vuex);

const luaScript = require("@/../../udisk/main.lua");

const store = new Vuex.Store({
    state: {
        ahk: {}
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
