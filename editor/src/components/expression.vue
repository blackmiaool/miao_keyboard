<template>
    <draggable v-model="expression.data" class="root" :options="{animation:250,disabled:!editable}">
        <section v-for="(section,i) in expression.data" :key="i" style="">
            <span v-if="section.mode==='print'" :title="`print ${section.value}`" class="print-section">
                {{section.value}}
            </span>
            <span v-if="section.mode==='press-key'" :title="'Press '+upperCaseFirst(section.value)+ ' Key'" class="press-key-section el-button--primary">
                <i v-if="!isConsumer(section.value)" class="fa fa-gavel" aria-hidden="true"></i>
                <i v-if="isConsumer(section.value)" class="fa fa-headphones" aria-hidden="true"></i>
                {{upperCaseFirst(section.value)}}
            </span>
            <span v-if="section.mode==='press-toggle'" class="press-toggle-section" :title="`${getModifierText(section.value.modifiers)} ${section.value.action}`">
                <i v-if="section.value.action==='down'" class="fa fa-hand-point-down" aria-hidden="true"></i>
                <i v-if="section.value.action==='up'" aria-hidden="true" class="fa fa-hand-point-up"></i>
                {{getModifierText(section.value.modifiers)}}
            </span>
            <i v-if="editable" class="fa fa-minus-circle delete-btn clickable" @click="deleteSection(i)"></i>
        </section>
    </draggable>
</template>

<script>
import { shortModifierMap, consumer2usb } from "@/common";
import draggable from "vuedraggable";

export default {
    name: "Expression",
    props: ["expression", "editable"],
    mounted() {},
    methods: {
        deleteSection(i) {
            this.expression.data.splice(i, 1);
        },
        upperCaseFirst(text) {
            return text.replace(/^[\s\S]/, ch => ch.toUpperCase());
        },
        isConsumer(key) {
            return consumer2usb[key];
        },
        getModifierText(modifiers) {
            const text = modifiers
                .map(arr => {
                    let modifier = "";
                    if (arr[0] === "<") {
                        modifier += "L";
                    } else if (arr[0] === ">") {
                        modifier += "R";
                    } else {
                        modifier += shortModifierMap[arr[0]];
                    }
                    if (arr[1]) {
                        modifier += shortModifierMap[arr[1]];
                    }
                    return modifier;
                })
                .join("+");
            return text;
        }
    },
    data() {
        return {};
    },
    components: {
        draggable
    }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped lang="less">
@import "../less/mixin.less";
// .list-enter-active,
// .list-leave-active {
//     transition: all 1s;
// }
// .list-enter,
// .list-leave-to {
//     opacity: 0;
//     transform: translateY(30px);
// }
.root {
    text-align: left;
    padding: 2px 0px;
}
.delete-btn {
    position: absolute;
    top: 3px;
    left: 1px;
    transform: translate(-50%, -50%);
    font-size: 12px;
    color: #aaa;
    &:hover {
        color: gray;
    }
    &:after {
        content: "";
        .crop(0);
        background-color: white;
        z-index: -1;
        border-radius: 1000px;
    }
}
section {
    position: relative;
    line-height: 28px;
    transition: 0.3s all;
    display: inline-block;
    margin: 1px 3px;

    > span {
        padding: 3px 7px;
        border-radius: 3px;
        cursor: default;
        vertical-align: middle;
        line-height: 19px;
        &.print-section {
            border: 1px solid #555;
            color: #333;
        }
        &.press-toggle-section {
            border: 2px solid #e6a23c;
            color: #e6a23c;
            font-weight: bold;
            border-radius: 6px;
        }
        &.press-key-section {
            display: inline-block;
            color: #fff;
            // background-color: #538fbe;
            // border: 1px solid #2d6898;
            // background-image: linear-gradient(
            //     bottom,
            //     rgb(73, 132, 180) 0%,
            //     rgb(97, 155, 203) 100%
            // );
            // background-image: -o-linear-gradient(
            //     bottom,
            //     rgb(73, 132, 180) 0%,
            //     rgb(97, 155, 203) 100%
            // );
            // background-image: -moz-linear-gradient(
            //     bottom,
            //     rgb(73, 132, 180) 0%,
            //     rgb(97, 155, 203) 100%
            // );
            // background-image: -webkit-linear-gradient(
            //     bottom,
            //     rgb(73, 132, 180) 0%,
            //     rgb(97, 155, 203) 100%
            // );
            // background-image: -ms-linear-gradient(
            //     bottom,
            //     rgb(73, 132, 180) 0%,
            //     rgb(97, 155, 203) 100%
            // );

            // background-image: -webkit-gradient(
            //     linear,
            //     left bottom,
            //     left top,
            //     color-stop(0, rgb(73, 132, 180)),
            //     color-stop(1, rgb(97, 155, 203))
            // );
            box-shadow: 0px 2px 0px darken(#409eff, 15%),
                0px 3px 0px rgba(0, 0, 0, 0.4);
            //   background-color: steelblue;
            //   color: white;
        }
    }
}
</style>
