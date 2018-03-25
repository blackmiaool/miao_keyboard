
import VuexList from "./vuex-list";


export default new VuexList({
    state: {
        list: []
    },
    mutations: {
        setList(state, list) {
            state.list = list;
        }
    },
    actions: {},
    getters: {}
});

