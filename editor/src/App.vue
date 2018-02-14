<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<script>
// require("./assets/css/fontawesome.min.css");

export default {
    name: "App"
};
</script>

<style>
body {
    margin: 0;
}
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
}
</style>
