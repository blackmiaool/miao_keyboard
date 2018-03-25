<template>
    <div id="app">
        <tabs/>
        <router-view/>
        <div style="max-width:860px;margin:auto;">
            <el-button type="success" @click="exportConfig" class="clickable export-btn" style="    border-bottom-left-radius: 0;border-bottom-right-radius: 0;width: 100%;">
                <i class="fa fa-paper-plane"></i> Export</el-button>
            <el-input type="textarea" :value="script" readonly style="border-top-left-radius: 0;border-top-right-radius: 0;" :rows="3"></el-input>
        </div>
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
        exportConfig() {
            this.$store.dispatch("exportConfig");
        }
    }
};
</script>

<style>
#app {
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    background-image: url(./assets/bg.jpg);
    background-size: cover;
    background-attachment: fixed;
    background-position: center center;
    padding: 30px 0;
    min-height: 100%;
    box-sizing: border-box;
}
</style>
