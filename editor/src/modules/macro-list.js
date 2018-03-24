
import Rule from "@/rule";
import VuexList from "./vuex-list";

let config = `
<+6::694421865
!<+a::blackmiaool
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
>+q::~
>+w::\` 
<+i::@xiaomi.com
<+b::blackmiaool
<+c::console.log(){left}
<+v::console.warn(){left}
<+t::this.state.
<+w::ilwt.science
<+o::const{space}
<+u::sudo{space}apt-get{space}install
<+n::(()=>{leftbracket}{rightbracket});{left}{left}{left}{enter}
customized1::{<^-down}{left}
customized2::{<^-down}{right}
# <: left modifier key
# >: right modifier key
# ^: ctrl
# !: alt
# +: shift
# #: win`;
config = config
    .replace(/{space}/g, "{Space}")
    .replace(/{esc}/g, "{Escape}")
    .replace(/{down}/g, "{ArrowDown}")
    .replace(/{up}/g, "{ArrowUp}")
    .replace(/{right}/g, "{ArrowRight}")
    .replace(/{left}/g, "{ArrowLeft}")
    .replace(/{home}/g, "{Home}")
    .replace(/{end}/g, "{End}")
    .replace(/{deleteforward}/g, "{Delete}")
    .replace(/{pagedown}/g, "{PageDown}")
    .replace(/{pageup}/g, "{PageUp}")
    .replace(/{printscreen}/g, "{PrintScreen}")
    .replace(/{enter}/g, "{Enter}");

const list = config
    .split(/\r?\n/)
    .map(line => {
        if (!Rule.format0Reg.test(line)) {
            return null;
        }

        return Rule.fromFormat0(line);
    })
    .filter(line => line);


export default new VuexList({
    state: {
        list
    },
    mutations: {},
    actions: {},
    getters: {}
});

