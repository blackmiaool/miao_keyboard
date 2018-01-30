<template>
<div class="table-wrap">
  <table class="table table-striped">
    <thead>
      <tr >
        <th>Modifiers</th>
        <th>Key</th>
        <th>Expression</th>
        <th>Action</th>
      </tr>
    </thead>
    <tbody>
      <MacroLine v-for="(li,index) in list" v-bind:key="li[0]" :li="li" class="macro-line" @delete="deleteLine(index)" @edit="editLine(index)"/>
    </tbody>
  </table>
</div>
</template>

<script src="./macro-list.js">
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped lang="less">
.table-wrap {
    text-align: center;
}
.table {
    max-width: 1000px;
    margin: auto;
}
</style>
