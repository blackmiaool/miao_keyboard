<template>
    <div class="kb-key clickable" :style="finalStyle" @click="onClick" :class="{inherit:!text}" :title="text">
        <!-- <el-tooltip class="item" effect="dark" :disabled="!title" :enterable="false" :content="title" placement="top" transition="el-zoom-in-top" :popper-options="{removeOnDestroy:true}"> -->
        <div class="key-front flex-center">
            <div v-if="key2icon[shortText]">
                <i :class="key2icon[shortText]"></i>
            </div>
            <div v-else-if="isModeTrigger(shortText)">
                <i class="fa fa-code-branch"></i>
                <span>{{getModeFromModeTrigger(shortText)}}</span>
            </div>
            <div v-else-if="isCustomized(shortText)">
                <i class="fa fa-wrench"></i>
                <span>{{getIndexFromCustomized(shortText)}}</span>
            </div>
            <div v-else class="text">{{shortText}}</div>

        </div>
        <!-- </el-tooltip> -->
    </div>

</template>
<script>
import {
    key2short,
    key2icon,
    isModeTrigger,
    getModeFromModeTrigger,
    isCustomized,
    getIndexFromCustomized
} from "@/common";

export default {
    methods: {
        onClick() {
            this.$emit("click");
        },
        getShort(text) {
            return key2short[text] || text;
        },
        isModeTrigger,
        getModeFromModeTrigger,
        isCustomized,
        getIndexFromCustomized
    },
    data() {
        return { key2icon };
    },
    computed: {
        title() {
            if (!this.text) {
                return "inherit from basic mode";
            }
            return null;
        },
        shortText() {
            if (this.text) {
                return this.getShort(this.text);
            }
            return this.getShort(this.basicText);
        },
        finalStyle() {
            const ret = { width: `${this.width * 54}px` };

            if (this.style0) {
                if (this.style0["margin-left"]) {
                    ret["margin-left"] = `${this.style0["margin-left"] * 54}px`;
                }
            }
            return ret;
        }
    },
    props: {
        style0: {
            type: Object
        },
        line: {
            type: Number,
            default: 1
        },
        width: {
            type: Number,
            default: 1
        },
        text: {},
        basicText: {}
    }
};
</script>
<style lang="less" scoped>
.kb-key {
    vertical-align: bottom;
    box-sizing: border-box;
    border: 1px solid black;
    height: 54px;
    display: inline-block;
    border-radius: 5px;
    background-color: #ccc;
    position: relative;
    overflow: hidden;
    &.selected {
        border: 2px solid #0077f3;
        .key-front {
            margin: -1px;
        }
    }
    &.inherit {
        opacity: 0.4;
    }
    .key-front {
        position: absolute;
        background-color: white;
        left: 6px;
        top: 3px;
        right: 6px;
        bottom: 9px;
        border-radius: 2px;
        white-space: pre;
        // font-weight: bold;
        > .text {
            line-height: 18px;
        }
    }
}
</style>

