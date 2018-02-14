<template>
    <div class="comp-macro-list table-wrap">
        <header>
            <i class="redo-btn fa fa-undo" :class="{active:listUndo.canUndo()}" @click="listUndo.undo()"></i>
            <i class="undo-btn fa fa-redo" :class="{active:listUndo.canRedo()}" @click="listUndo.redo()"></i>
        </header>
        <el-table :data="list" stripe class="table" ref="table">
            <el-table-column type="expand">
                <template slot-scope="scope" align="left">
                    <RuleEditor :value="scope.row" @input="onSave(scope.row,$event)" />
                </template>
            </el-table-column>
            <el-table-column label="Modifiers" align="left" width="180">
                <template slot-scope="scope">
                    <span v-for="modifier in scope.row.modifiers" v-bind:key="modifier" class="modifier">
                        {{modifier}}
                    </span>
                </template>
            </el-table-column>
            <el-table-column prop="key" label="Key" width="80" align="center" sortable>
            </el-table-column>
            <el-table-column label="Expression">
                <template slot-scope="scope">
                    <ExpressionComp :expression="scope.row.expression" :editable="false" />
                </template>
            </el-table-column>
            <el-table-column label="Action" align="center" width="220">
                <template slot-scope="scope">
                    <span class="miao-hover">
                        <el-button type="success" size="mini" @click="edit(scope.row)">
                            <i class="fa fa-edit"></i> Edit</el-button>

                        <el-button type="danger" size="mini" @click="deleteLine(scope.row)">
                            <i class="fa fa-trash"></i> Delete</el-button>
                    </span>
                </template>
            </el-table-column>
        </el-table>
        <el-button type="success" @click="exportConfig" class="clickable export-btn">
            <i class="fa fa-paper-plane"></i> Export</el-button>
    </div>
</template>

<script src="./macro-list.js">
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped lang="less">
.redo-btn,
.undo-btn {
    opacity: 0.5;
    &.active {
        opacity: 1;
    }
}

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
<style lang="less">
.export-btn {
    width: 100%;
    margin-top: 10px;
}
// .comp-macro-list {
//     .el-table {
//         background-color: transparent !important;
//     }
//     table {
//         background-color: transparent !important;
//     }
//     tr {
//         background-color: transparent !important;
//         td {
//             background-color: transparent !important;
//         }
//     }
//     tr.el-table__row--striped {
//         background-color: rgba(251, 251, 251, 0.2) !important;
//     }
// }
</style>
