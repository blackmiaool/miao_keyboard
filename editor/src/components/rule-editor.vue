<template>
    <div @keydown="onKeyDown">
        <el-form ref="form" label-width="100px">
            <el-form-item label="Modifiers">
                <el-select v-model="data.modifiers" multiple placeholder="Select" @visible-change="visibleChange" style="width:400px;">
                    <el-option v-for="(value,modifier) in modifierMap" :key="value" :label="modifier" :value="modifier">
                    </el-option>
                </el-select>
            </el-form-item>
            <el-form-item label="Key">
                <el-input v-model="data.key" style="width:220px;"></el-input>
            </el-form-item>
            <el-form-item label="Expression">
                <div @keydown.prevent="onInputExpressionDown" @keyup="onInputExpressionUp">
                    <el-input style="width:150px;color:white;" v-model="expressionInput" placeholder="Focus to input"></el-input>
                    <el-checkbox v-model="blockModifier">BlockModifier</el-checkbox>
                    <el-select v-model="selectedConsumer" @change="consumerChange" style="width:120px;" placeholder="Media">
                        <el-option v-for="(usb,consumer) in consumer2usb" :key="usb" :label="consumer" :value="consumer">
                        </el-option>
                    </el-select>
                    <el-select v-model="selectedKey" @change="keyChange" style="width:120px;" placeholder="Key">
                        <el-option v-for="(usb,key) in key2usb" :key="usb" :label="key" :value="key">
                        </el-option>
                    </el-select>
                </div>
                <ExpressionComp :expression="data.expression" :editable="true" />
                <div>{{data.expression.toPlainText()}}</div>
            </el-form-item>
            <el-form-item label="">
                <el-button type="primary" @click="onSubmit">Save</el-button>
                <el-button type="danger" @click="onClear">Clear</el-button>
                <el-button type="danger" @click="onClearExpression">Clear Expression</el-button>
            </el-form-item>
        </el-form>
    </div>

</template>

<script>
import ExpressionComp from "@/components/expression";
import { modifierMap, code2modifier, consumer2usb, key2usb } from "@/common";
import Expression from "@/expression";

export default {
    created() {
        this.cachedData = JSON.parse(JSON.stringify(this.value));
        console.log("this.cachedData", this.cachedData);
        Object.assign(this.data, this.cachedData);

        this.data.expression = new Expression(this.data.expression);
        console.log("mounted", this.data);
    },
    methods: {
        onClearExpression() {
            this.data.expression.empty();
        },
        onClear() {
            this.data.expression = new Expression([]);
            this.data.modifiers = [];
            this.data.key = undefined;
        },
        keyChange(key) {
            this.data.expression.addPress(key);
            this.selectedKey = undefined;
        },
        consumerChange(consumer) {
            this.data.expression.addPress(consumer);
            this.selectedConsumer = undefined;
        },
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

                if (set.has(modifier)) {
                    set.delete(modifier);
                } else {
                    set.add(modifier);
                }
                this.data.modifiers = [...set];
            }
            e.preventDefault();
        },
        onInputExpressionUp(e) {
            if (code2modifier[e.code]) {
                if (!this.blockModifier) {
                    this.data.expression.addModifier(e.code, "up");
                }
            }
        },
        onInputExpressionDown(e) {
            console.log("e", e);
            console.log(e.key);
            const key = e.key;
            if (key.match(/^[a-zA-Z0-9]$/) || key.match(/^[\S]$/)) {
                this.data.expression.addPrint(key);
            } else if (code2modifier[e.code]) {
                if (!this.blockModifier) {
                    this.data.expression.addModifier(e.code, "down");
                }
            } else {
                this.data.expression.addPress(key);
            }
            this.$nextTick(() => {
                this.expressionInput = "";
            });
        },
        onSubmit() {
            if (this.data.key && this.data.key.length === 1) {
                this.data.key = this.data.key.toUpperCase();
            }
            console.log(this.data);
            this.$emit("input", this.data);
        }
    },
    watch: {
        data: {
            handler: newValue => {},
            deep: true
        }
    },
    props: {
        value: {
            type: Object
        }
    },
    data() {
        return {
            selectedKey: undefined,
            key2usb,
            selectedConsumer: undefined,
            consumer2usb,
            cachedData: undefined,
            modifierMap,
            acceptingModifer: false,
            expressionInput: "",
            expression: [],
            data: {
                modifiers: undefined,
                key: undefined,
                expression: undefined
            },
            blockModifier: true
        };
    },
    components: {
        ExpressionComp
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
