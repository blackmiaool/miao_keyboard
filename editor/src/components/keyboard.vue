<template>
    <div>
        <div class="kb-line" v-for="(keyLine,i) in layout" :key="i">
            <Key :class="{selected:key===selectedKey}" v-for="(key,j) in keyLine" :text="map[i][j]" :basicText="basicMap[i][j]" :key="j" :width="key.w" :line="key.l" :style0="key.style" @click="selectKey(i,j)" />
        </div>
    </div>
</template>

<script>
import Key from "@/components/key";
import { mapState } from "vuex";
import { kbLayout } from "@/kbmode";

export default {
    methods: {
        selectKey(x, y) {
            const clickKey = this.layout[x][y];

            if (this.selectedKey === clickKey) {
                this.selectedKey = undefined;
                this.$emit("unselectKey");
            } else {
                this.selectedKey = this.layout[x][y];
                this.$emit("selectKey", x, y);
            }
        }
    },
    mounted() {
        console.log(this.layout);
    },
    computed: {
        ...mapState({ basicMap: state => state.modes[0].map })
    },
    props: ["map"],
    data() {
        return { selectedKey: undefined, layout: kbLayout };
    },
    components: {
        Key
    }
};
</script>

<style scoped lang="less">

</style>
