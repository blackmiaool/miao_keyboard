<template>
<tr class="macro-line">
<td>
    <span v-for="modifier in li.modifiers" v-bind:key="modifier" class="modifier">
    {{modifier}}
    </span>
</td>
<td class="normal-key">{{li.key}}</td>
<td class="expression"><Expression :expression="li.expression"/></td>
<td >
    <span class="hover">
    <el-button type="success" size="mini" @click="$emit('edit')"><i class="fa fa-edit"></i> Edit</el-button>

    <el-button type="danger" size="mini" @click="$emit('delete')"><i class="fa fa-trash"></i> Delete</el-button>
    </span>
</td>
</tr>
</template>

<script>
import Expression from "@/components/expression";

export default {
    name: "MacroLine",
    props: ["li"],
    methods: {},
    data() {
        return {};
    },
    components: {
        Expression
    }
};
</script>

<style scoped lang="less">
td {
    &.normal-key {
        vertical-align: middle;
    }
    vertical-align: middle;
    padding: 3px;
}
.hover {
    visibility: hidden;
    opacity: 0;
}
tr:hover {
    .hover {
        visibility: visible;
        opacity: 1;
    }
}
.modifier {
    margin: 2px;
    border: 2px solid #563d7c;
    color: #563d7c;
    font-weight: bold;
    border-radius: 6px;
    padding: 3px 5px;
    display: inline-block;
}
</style>
