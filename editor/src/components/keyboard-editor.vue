<template>
    <div class="wrap" @keydown.prevent="onKeyDown">
        <el-form ref="form" label-width="80px">
            <el-form-item label="Key name">
                <el-input :value="value" :placeholder="'inherit: '+baseValue" clearable @clear="onClear"></el-input>
            </el-form-item>
            <el-select v-model="selectedKey" @change="keyChange" style="width:120px;" placeholder="Key">
                <el-option v-for="(usb,key) in key2usb" :key="usb" :label="key" :value="key">
                </el-option>
            </el-select>
            <el-select v-model="selectedConsumer" @change="keyChange" style="width:120px;" placeholder="Media">
                <el-option v-for="(usb,consumer) in consumer2usb" :key="usb" :value="consumer">
                </el-option>
            </el-select>
            <el-select v-model="selectedModifier" @change="keyChange" style="width:120px;" placeholder="Modifier">
                <el-option v-for="(usb,modifier) in modifier2usb" :key="usb" :value="modifier">
                </el-option>
            </el-select>
            <el-select v-model="selectedChar" @change="keyChange" style="width:120px;" placeholder="Char">
                <el-option v-for="(usb,char) in baseAscii2usb" :key="char" :value="String.fromCharCode(char).toUpperCase()">
                </el-option>
            </el-select>
            <el-select v-model="selectedModeTrigger" @change="keyChange" style="width:140px;" placeholder="Mode trigger">
                <el-option v-for="(mode,i) in modes" :key="i" :value="'mode'+i" v-if="!mode.isBasic">
                </el-option>
            </el-select>
            <el-select v-model="selectedCustomized" @change="keyChange" style="width:140px;" placeholder="Customized">
                <el-option v-for="(customized,i) in customized2usb" :key="i" :value="i">
                </el-option>
            </el-select>
        </el-form>
    </div>
</template>

<script>
import {
    code2modifier,
    consumer2usb,
    shiftTable,
    key2usb,
    modifier2usb,
    baseAscii2usb,
    customized2usb
} from "@/common";
import { mapState } from "vuex";

export default {
    methods: {
        onClear() {
            this.$emit("input", null);
        },
        keyChange() {
            const value =
                this.selectedKey ||
                this.selectedConsumer ||
                this.selectedModifier ||
                this.selectedModeTrigger ||
                this.selectedChar ||
                this.selectedCustomized;
            console.log("change", value);
            this.$emit("input", value);
            this.selectedKey = null;
            this.selectedConsumer = null;
            this.selectedModifier = null;
            this.selectedChar = null;
            this.selectedModeTrigger = null;
            this.selectedCustomized = null;
        },
        onKeyDown(e) {
            console.log("e", e);
            console.log(e.key);

            const key = e.key;

            if (key.match(/^[a-zA-Z0-9]$/) || key.match(/^[\S]$/)) {
                if (shiftTable[e.key.charCodeAt(0)]) {
                    this.$emit("input", null);
                } else {
                    this.$emit("input", key.toUpperCase());
                }
            } else if (key === " ") {
                this.$emit("input", "Space");
            } else if (code2modifier[e.code]) {
                this.$emit("input", e.code);
            } else {
                this.$emit("input", e.key);
            }
            // this.$nextTick(() => {
            //     this.expressionInput = "";
            // });
        }
    },
    watch: {},
    mounted() {},
    props: ["keyText", "value", "baseValue"],
    computed: {
        ...mapState(["modes"])
    },
    data() {
        return {
            key2usb,
            selectedKey: null,
            selectedConsumer: null,
            selectedModifier: null,
            modifier2usb,
            consumer2usb,
            baseAscii2usb,
            selectedChar: null,
            selectedModeTrigger: null,
            selectedCustomized: null,
            customized2usb
        };
    },
    components: {}
};
</script>

<style scoped lang="less">
.wrap {
    text-align: left;
    background-color: rgba(255, 255, 255, 0.7);
    padding: 10px;
    border-radius: 5px;
    margin-top: 15px;
}
</style>
