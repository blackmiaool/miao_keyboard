<template>
    <div>
        <router-link to="macro-list">
            <i class="fa fa-fire"></i>
            Macro</router-link>
        <router-link to="layout">
            <i class="fa fa-keyboard"></i>
            Layout</router-link>
        <router-link to="import">
            <i class="fa fa-upload"></i>
            Import</router-link>
        <router-link to="export">
            <i class="fa fa-download"></i>
            Export</router-link>
    </div>

</template>
<script>
export default {};
</script>
<style lang="less" scoped>
a {
    text-decoration: none;
    color: #333;
    display: inline-block;
    background-color: rgba(255, 255, 255, 0.7);
    padding: 10px 25px;
    border-radius: 5px;
    border-bottom-left-radius: 0;
    border-bottom-right-radius: 0;
    &.router-link-active {
        color: white;
        background-color: #0077f3;
    }
}
</style>

