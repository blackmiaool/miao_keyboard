<template>
    <div class="comp-layout window" :class="{dragging}" @drop="drop" @dragenter="dragEnter" @dragleave="dragLeave" @dragexit="dragExit" @dragover="dragOver" @mouseover="dragClear">
        <el-form>
            <el-form-item label="Config File">
                <el-input type="textarea" v-model="config"></el-input>
            </el-form-item>
            <el-button @click="submit" type="primary" style="width:100%;">Import</el-button>
            <el-button @click="importDefault" type="warning" style="width:100%;margin:0;margin-top:20px;">Import Default Config</el-button>
        </el-form>
    </div>
</template>

<script>
import { getJSONFromLua } from "@/common";
import defaultConfig from "../defaultConfig.json";
import Drag from "../drag-mixin";

export default {
    created() {},
    mixins: [Drag],
    computed: {
        canImport() {
            try {
                getJSONFromLua(this.config);
            } catch (e) {
                return false;
            }
            return true;
        }
    },
    methods: {
        name: "Import",
        onDrop(fileContent) {
            this.config = fileContent;
            this.submit();
        },
        submit() {
            let config;
            try {
                config = getJSONFromLua(this.config);
            } catch (e) {
                this.$message({
                    message: e.message,
                    type: "error"
                });
                return;
            }
            this.$store.commit("importConfig", config);
            this.$message({
                message: "Succeed to import config!",
                type: "success",
                duration: 1300
            });
        },
        importDefault() {
            this.$store.commit("importConfig", defaultConfig);
            this.$message({
                message: "Succeed to import default config!",
                type: "success",
                duration: 500
            });
        }
    },
    mounted() {
        setTimeout(() => {
            this.importDefault();
        });
    },
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
    &.dragging {
        opacity: 0.7;
    }
}
</style>
