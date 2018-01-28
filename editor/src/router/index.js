import Vue from 'vue';
import Router from 'vue-router';
import HelloWorld from '@/components/macro-list';

Vue.use(Router);

export default new Router({
  routes: [
    {
      path: '/',
      name: 'macro-list',
      component: HelloWorld,
    },
  ],
});
