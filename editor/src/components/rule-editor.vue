<template>
    <div @keydown="onKeyDown">
        <el-form ref="form" label-width="120px">
            <el-form-item label="Modifiers">
                <el-select v-model="data.modifiers" multiple placeholder="Select" @visible-change="visibleChange">
                    <el-option v-for="(value,modifier) in modifierMap" :key="value" :label="modifier" :value="modifier">
                    </el-option>
                </el-select>
            </el-form-item>
            <el-form-item label="Key">
                <el-input v-model="data.key" style="width:220px;"></el-input>
            </el-form-item>
            <el-form-item label="Expression">
                <el-input v-model="data.key" style="width:220px;"></el-input>
            </el-form-item>
        </el-form>
    </div>

</template>

<script>
import { modifierMap } from "@/common";

const code2modifier = {
    ControlLeft: "LCtrl",
    ControlRight: "RCtrl",
    AltLeft: "LAlt",
    AltRight: "RAlt",
    ShiftLeft: "LShift",
    ShiftRight: "RShift",
    WinLeft: "LWin",
    WinRight: "RWin"
};
export default {
    methods: {
        visibleChange(state) {
            this.acceptingModifer = state;
        },
        onKeyDown(e) {
            if (!this.acceptingModifer) {
                return;
            }
            console.log(e, this.data.modifiers);
            const modifier = code2modifier[e.code];
            console.log("modifier", modifier);
            if (modifier) {
                const set = new Set(this.data.modifiers);
                set.add(modifier);
                this.data.modifiers = [...set];
                console.log(1, this.data.modifiers);
            }
        }
    },
    watch: {
        data: {
            handler: newValue => {
                console.log(newValue);
            },
            deep: true
        }
    },
    props: ["data"],
    data() {
        return {
            modifierMap,
            acceptingModifer: false
        };
    }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped lang="less">
.table-wrap {
    text-align: center;
    padding: 30px;
    background-color: rgba(255, 255, 255, 0.8);
    max-width: 800px;
    margin: auto;
    border-radius: 5px;
}
.table {
    border-radius: 5px;
    text-align: left;
}
.modifier {
    margin: 2px;
    border: 2px solid #e6a23c;
    color: #e6a23c;
    font-weight: bold;
    border-radius: 6px;
    padding: 3px 5px;
    display: inline-block;
}
.miao-hover {
    visibility: hidden;
    opacity: 0;
}
tr:hover {
    .miao-hover {
        visibility: visible;
        opacity: 1;
    }
}
</style>
