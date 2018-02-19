<template>
    <div class="kb-key" :style="finalStyle" @click="onClick">
        <div class="key-front flex-center">
            <div class="text">{{shortText}}</div>
        </div>
    </div>

</template>
<script>
import { key2short } from "@/common";

export default {
    methods: {
        onClick() {
            this.$emit("click");
        }
    },
    computed: {
        shortText() {
            return key2short[this.text] || this.text;
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
        text: {}
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
    &.selected {
        border: 2px solid #0077f3;
        .key-front {
            margin: -1px;
        }
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
        }
    }
}
</style>

