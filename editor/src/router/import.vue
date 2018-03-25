<template>
    <div class="comp-layout window">
        <el-form>
            <el-form-item label="Config File">
                <el-input type="textarea" v-model="config"></el-input>
            </el-form-item>
            <el-button @click="submit" type="primary" style="width:100%;" :disabled="!canImport">Import</el-button>
        </el-form>
    </div>
</template>

<script>
export default {
    created() {},
    computed: {
        canImport() {
            return false;
        }
    },
    methods: {
        submit() {
            this.$store.commit("importConfig", this.config);
        }
    },
    mounted() {},
    data() {
        return {
            config: ""
        };
    },
    components: {}
};
</script>

<style scoped lang="less">
.window {
    padding: 20px;
}
</style>
