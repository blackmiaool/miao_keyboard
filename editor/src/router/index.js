import Vue from 'vue';
import Router from 'vue-router';
import MacroList from '@/components/macro-list.vue';

Vue.use(Router);

console.log('MacroList', MacroList);
export default new Router({
  routes: [
    {
      path: '/',
      name: 'macro-list',
      component: MacroList,
    },
  ],
});
