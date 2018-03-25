<template>
    <div id="app">
        <tabs/>
        <router-view/>
    </div>
</template>

<script>
// require("./assets/css/fontawesome.min.css");
import tabs from "@/components/tabs";
import { mapState } from "vuex";

export default {
    name: "App",
    data() {
        return { value: "" };
    },
    components: { tabs },
    computed: {
        ...mapState(["script"])
    },
    methods: {
        exportConfig() {}
    }
};
</script>

<style>
#app {
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;

    background-size: cover;
    background-attachment: fixed;
    background-position: center center;
    padding: 30px 0;
    min-height: 100%;
    box-sizing: border-box;
}
</style>
