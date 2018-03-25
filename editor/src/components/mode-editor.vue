<template>
    <div class="wrap">
        <el-form v-if="mode" ref="form" :model="mode" label-width="120px">
            <el-form-item label="Enable macro">
                <el-switch v-model="mode.macro"></el-switch>
            </el-form-item>
            <el-container v-if="!mode.isBasic">
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
                        <i class="fa fa-code-branch"></i>
                        <span>{{mode.index}}</span>
                    </el-form-item>
                </el-col>
            </el-container>

        </el-form>
        <div v-if="!mode" style="flex:1;">
            Select a mode first
        </div>
    </div>
</template>

<script>
import { getModeFromModeTrigger } from "@/common";

export default {
    methods: {
        getModeFromModeTrigger
    },
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
