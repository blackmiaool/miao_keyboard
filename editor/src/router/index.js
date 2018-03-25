import Vue from "vue";
import Router from "vue-router";
import MacroList from "@/router/macro-list.vue";
import Layout from "@/router/layout";
import Import from "@/router/import";
import Export from "@/router/export";

Vue.use(Router);

export default new Router({
    routes: [
        {
            path: "/",
            name: "home",
            component: MacroList
        },
        {
            path: "/macro-list",
            name: "macro-list",
            component: MacroList
        },
        {
            path: "/layout",
            name: "layout",
            component: Layout
        },
        {
            path: "/import",
            name: "import",
            component: Import
        },
        {
            path: '/export',
            name: 'export',
            component: Export
        }
    ]
});
