<template>
    <div>
        <router-link to="macro-list">
            <i class="fa fa-fire"></i>
            Macro</router-link>
    </div>

</template>
<script>
export default {};
</script>
<style lang="less" scoped>
a {
    text-decoration: none;
    color: white;
    display: inline-block;
    background-color: rgba(255, 255, 255, 0.7);
    padding: 10px 25px;
    border-radius: 5px;
    border-bottom-left-radius: 0;
    border-bottom-right-radius: 0;
    &.router-link-active {
        background-color: #0077f3;
    }
}
</style>

