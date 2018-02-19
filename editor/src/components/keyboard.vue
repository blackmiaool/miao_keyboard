<template>
    <div>
        <div class="kb-line" v-for="(keyLine,i) in layout" :key="i">
            <Key :class="{selected:key===selectedKey}" v-for="(key,j) in keyLine" :text="map[i][j]" :key="j" :width="key.w" :line="key.l" :style0="key.style" @click="selectKey(i,j)" />
        </div>
    </div>
</template>

<script>
import Key from "@/components/key";

export default {
    methods: {
        selectKey(x, y) {
            const clickKey = this.layout[x][y];
            console.log(this.selectedKey, clickKey);
            if (this.selectedKey === clickKey) {
                this.selectedKey = undefined;
            } else {
                this.selectedKey = this.layout[x][y];
            }
        }
    },
    mounted() {
        console.log(this.layout);
    },
    props: ["map", "layout"],
    data() {
        return { selectedKey: undefined };
    },
    components: {
        Key
    }
};
</script>

<style scoped lang="less">

</style>
