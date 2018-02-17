<template>
    <div id="app">
        <tabs/>
        <router-view/>
    </div>
</template>

<script>
// require("./assets/css/fontawesome.min.css");
import tabs from "@/components/tabs";

export default {
    name: "App",
    components: { tabs }
};
</script>

<style>
#app {
    font-family: "Avenir", Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    background-image: url(./assets/bg.jpg);
    background-size: cover;
    background-attachment: fixed;
    padding: 30px 0;
    min-height: 100%;
    box-sizing: border-box;
}
</style>
