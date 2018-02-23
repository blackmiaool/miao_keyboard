<template>
    <div class="wrap">
        <el-form ref="form" :model="mode" label-width="120px">
            <el-form-item label="Mode name">
                <el-input v-model="mode.name" :disabled="isBasicMode"></el-input>
            </el-form-item>
            <el-form-item label="Enable macro">
                <el-switch v-model="mode.macro"></el-switch>
            </el-form-item>
            <el-container v-if="!isBasicMode">
                <el-col :span="12">
                    <el-form-item label="Trigger">
                        <el-select v-model="mode.trigger" placeholder="please select your zone">
                            <el-option label="Keep pressing to tigger" value="pressing"></el-option>
                            <el-option label="Press to trigger" value="toggle"></el-option>
                        </el-select>
                    </el-form-item>
                </el-col>
                <el-col :span="11">
                    <el-form-item label="Trigger key">
                        <el-input type="number" v-model="mode.triggerKey"></el-input>
                    </el-form-item>
                </el-col>
            </el-container>

        </el-form>
    </div>
</template>

<script>
export default {
    methods: {},
    mounted() {},
    props: ["mode"],
    computed: {
        isBasicMode() {
            return this.mode.name === "basic";
        }
    },
    data() {
        return {};
    },
    components: {}
};
</script>

<style scoped lang="less">
.wrap {
    text-align: left;
}
</style>
