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
        <td class="expression">{{li.expression}}</td>
      </tr>
    </tbody>
  </table>
</div>
</template>

<script>
const config = `!<+a::blackmiaool
^!+a::qk333333
<+d::sll{space}l
>^w::{esc}
>^n::{down}
>^p::{up}
>^f::{right}
>^b::{left}
>^a::{home}
>^e::{end}
>^d::{deleteforward}
>^v::{pagedown}
<!v::{pageup}
^!p::{printscreen}
<+p::git{space}pull{space}--rebase
<+f::function
<+l::local{space} 
<+r::return{space}
<+q::qk333333
>+q::~
>+w::\` 
<+s::sunqimin
<+i::@xiaomi.com
<+b::blackmiaool
<+c::console.log(){left}
<+v::console.warn(){left}
<+t::this.state.
<+w::ilwt.science
<+o::const{space}
<+u::sudo{space}apt-get{space}install
<^<!r::{restart}
<+n::(()=>{leftbracket}{rightbracket});{left}{left}{left}{enter}
140::{<^-down}{left}
141::{<^-down}{right}
# <: left modifier key
# >: right modifier key
# ^: ctrl
# !: alt
# +: shift
# #: win`;
const modifierMap = {
  "^": "Ctrl",
  "!": "Alt",
  "+": "Shift",
  "#": "Win"
};
function modifier2PlainText(modifier) {
  let ret = "";
  if (modifier[0] === "<") {
    ret += "L";
  } else if (modifier[0] === ">") {
    ret += "R";
  }
  ret += modifierMap[modifier[modifier.length - 1]];
  return ret;
}
const list = config
  .split(/\r?\n/)
  .map(line => {
    const match = line.match(/([\^+!#<>]*)([\da-zA-Z]+)::([\s\S]+)/);
    if (!match) {
      return null;
    }
    const ret = {
      modifiers: (match[1].match(/[<>]?[\^+!#]/g) || []).map(
        modifier2PlainText
      ),
      key: match[2],
      expression: match[3]
    };

    return ret;
  })
  .filter(line => line);
console.log(list);

export default {
  name: "HelloWorld",
  data() {
    return {
      msg: "Welcome to Your Vue.js App",
      list
    };
  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.table-wrap {
  text-align: center;
}
table {
  margin: auto;
}
.modifier {
  margin: 2px;
  border: 1px solid #2c3e50;
  background-color: rgba(44, 62, 80, 0.2);
  border-radius: 3px;
  padding: 3px 5px;
  display: inline-block;
}
</style>
