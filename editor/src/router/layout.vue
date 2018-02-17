<template>
    <div class="comp-layout window">
        <div class="kb-line" v-for="(keyLine,i) in kbConfig" :key="i">
            <div class="kb-key" v-for="(key,j) in keyLine" :key="j" :style="{width:(key.w||1)*54+'px'}">
                <div class="key-front"></div>
            </div>
        </div>
    </div>
</template>

<script>
// prettier-ignore
const kbConfig = [
    [{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2 }],
    [{ w: 1.5 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 1.5 }],
    [{ w: 1.75 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.25, l: 2 }],
    [{ w: 2.25, l: 2 }, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, { w: 2.75, l: 2 }],
    [{ w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.75, l: 2 }, { w: 1.75, l: 2 }, {  w: 1.5 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }, { w: 1.25 }]
];
export default {
    data() {
        return { kbConfig };
    }
};
</script>

<style scoped lang="less">
.window {
    background-color: rgba(255, 255, 255, 0.8);
    display: inline-block;
    padding: 20px;
    border-radius: 5px;
}
.kb-line {
    // height: 54px;
}
.kb-key {
    vertical-align: top;
    box-sizing: border-box;
    border: 1px solid black;
    height: 54px;
    display: inline-block;
    border-radius: 5px;
    background-color: #ccc;
    position: relative;
    .key-front {
        position: absolute;
        background-color: white;
        left: 6px;
        top: 3px;
        right: 6px;
        bottom: 9px;
        border-radius: 2px;
    }
}
</style>
