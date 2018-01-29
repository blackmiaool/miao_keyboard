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
      <tr  v-for="li in list" v-bind:key="li[0]" class="macro-line">
        <td>
          <span v-for="modifier in li.modifiers" v-bind:key="modifier" class="modifier">{{modifier}}</span>
        </td>
        <td class="normal-key">{{li.key}}</td>
        <td class="expression"><Expression :expression="li.expression"/></td>
        <td >
          <span class="hover">
          <button class="btn btn-info btn-sm">
            <i class="fa fa-edit"></i>
            Edit</button>
          <button class="btn btn-danger btn-sm">
            <i class="fa fa-trash"></i>
            Delete</button>
          </span>
        </td>
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
.table {
  max-width: 1000px;
  margin: auto;
  td {
    &.normal-key {
      vertical-align: middle;
    }
    vertical-align: middle;
    padding: 3px;
  }
  .hover {
    visibility: hidden;
  }
  tr:hover {
    .hover {
      visibility: visible;
    }
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
