<template>
    <div>
        <div class="kb-line" v-for="(keyLine,i) in layoutarr" :key="i">
            <Key :class="{selected:key===selectedKey}" v-for="(key,j) in keyLine" :text="map[i][j]" :basicText="basicMap[i][j]" :key="j" :width="key.w" :line="key.l" :style0="key.style" @click="selectKey(i,j)" />
        </div>
    </div>
</template>

<script>
import { mapState } from "vuex";
import Key from "@/components/key";
import { kbLayouts } from "../kbmode";


export default {
    methods: {
        selectKey(x, y) {
            const clickKey = this.layoutarr[x][y];

            if (this.selectedKey === clickKey) {
                this.selectedKey = undefined;
                this.$emit("unselectKey");
            } else {
                this.selectedKey = this.layoutarr[x][y];
                this.$emit("selectKey", x, y);
            }
        }
    },
    mounted() {
        console.log(this.layoutarr);
    },
    computed: {
        ...mapState({ basicMap: state => state.modes[0].map }),
        layoutarr() {
            return kbLayouts[this.layout].value.layout;
        }
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
