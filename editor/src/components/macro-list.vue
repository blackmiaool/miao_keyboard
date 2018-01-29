<template>
<div class="table-wrap">
  <table>
    <thead>
      <tr >
        <th>Modifiers</th>
        <th>Key</th>
        <th>Expression</th>
      </tr>
    </thead>
    <tbody>
      <tr  v-for="li in list" v-bind:key="li[0]" class="macro-line">
        <td>
          <span v-for="modifier in li.modifiers" v-bind:key="modifier" class="modifier">{{modifier}}</span>
        </td>
        <td class="normal-key">{{li.key}}</td>
        <td class="expression"><Expression :expression="li.expression"/></td>
      </tr>
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
table {
  margin: auto;
  tr:hover {
    background-color: #eee;
  }
}
.modifier {
  margin: 2px;
  border: 1px solid #2c3e50;
  background-color: rgba(44, 62, 80, 0.1);
  border-radius: 3px;
  padding: 3px 5px;
  display: inline-block;
}
</style>
