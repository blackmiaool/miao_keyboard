<template>
    <div class="root">
        <section v-for="(section,i) in expression" :key="i">
            <span v-if="section.mode==='print'" :title="`print ${section.value}`" class="print-section">
                {{section.value}}
            </span>
            <span v-if="section.mode==='press-key'" :title="'Press '+upperCaseFirst(section.value)+ ' Key'" class="press-key-section el-button--primary">
                <i class="fa fa-gavel" aria-hidden="true"></i>
                {{upperCaseFirst(section.value)}}
            </span>
            <span v-if="section.mode==='press-toggle'" class="press-toggle-section" :title="`${section.value.text} ${section.value.action}`">
                <i v-if="section.value.action==='down'" class="fa fa-hand-o-down" aria-hidden="true"></i>
                <i v-if="section.value.action==='up'" aria-hidden="true" class="fa fa-hand-o-up"></i>
                {{getModifierText(section.value.modifiers)}}
            </span>
        </section>
    </div>
</template>

<script>
import { shortModifierMap } from "@/common";

export default {
    name: "Expression",
    props: ["expression"],
    methods: {
        upperCaseFirst(text) {
            return text.replace(/^[\s\S]/, ch => ch.toUpperCase());
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
    }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped lang="less">
.root {
    text-align: left;
    padding: 2px 0px;
}
section {
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
