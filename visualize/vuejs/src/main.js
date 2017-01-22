import Vue from 'vue'
import App from './App'
import 'jquery'

require('bootstrap-loader')

/* eslint-disable no-new */
new Vue({
  el: '#app',
  template: '<App/>',
  components: { App }
})
