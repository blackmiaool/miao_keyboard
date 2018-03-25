<template>
    <div class="window">
        <!-- <el-button type="success" @click="exportConfig" class="clickable export-btn" style="    border-bottom-left-radius: 0;border-bottom-right-radius: 0;width: 100%;">
            <i class="fa fa-paper-plane"></i> Export</el-button>
         -->
        <el-input type="textarea" :value="script" readonly style="border-top-left-radius: 0;border-top-right-radius: 0;" :rows="15"></el-input>
        <div style="display:flex;flex-direction:row;">
            <el-button ref="copy" :data-clipboard-text="script" style="flex:1;margin:0;" type="warning">Copy</el-button>
            <el-button style="flex:1;margin:0;" type="primary" @click="download( 'main.lua',script)">Download</el-button>
        </div>
    </div>
</template>

<script>
import { mapState } from "vuex";
import ClipboardJS from "clipboard";

export default {
    name: "Export",
    mixins: [],
    mounted() {
        this.$store.dispatch("exportConfig");
        const clipboard = new ClipboardJS(this.$refs.copy.$el, {
            text: () => {
                return this.script;
            }
        });
        clipboard.on("success", () => {
            this.$message({
                message: "Successfully copied",
                type: "success"
            });
        });
    },
    computed: {
        ...mapState(["script"])
    },
    methods: {
        download(filename, text) {
            const element = document.createElement("a");
            element.setAttribute(
                "href",
                `data:text/plain;charset=utf-8,${encodeURIComponent(text)}`
            );
            element.setAttribute("download", filename);

            element.style.display = "none";
            document.body.appendChild(element);

            element.click();

            document.body.removeChild(element);
        }
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
