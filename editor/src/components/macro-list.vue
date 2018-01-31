<template>
    <div class="comp-macro-list table-wrap">
        <el-table :data="list" stripe class="table">
            <el-table-column label="Modifiers" align="left" width="180">
                <template slot-scope="scope">
                    <span v-for="modifier in scope.row.modifiers" v-bind:key="modifier" class="modifier">
                        {{modifier}}
                    </span>
                </template>
            </el-table-column>
            <el-table-column prop="key" label="Key" width="60" align="center">
            </el-table-column>
            <el-table-column label="Expression">
                <template slot-scope="scope">
                    <Expression :expression="scope.row.expression" />
                </template>
            </el-table-column>
            <el-table-column prop="address" label="Action" align="center" width="220">
                <template slot-scope="scope">
                    <span class="hover">
                        <el-button type="success" size="mini" @click="$emit('edit')">
                            <i class="fa fa-edit"></i> Edit</el-button>

                        <el-button type="danger" size="mini" @click="$emit('delete')">
                            <i class="fa fa-trash"></i> Delete</el-button>
                    </span>
                </template>
            </el-table-column>
        </el-table>
    </div>
</template>

<script src="./macro-list.js">
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
</style>
<style lang="less">
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
