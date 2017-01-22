<template>
  <div id="app">
    <div class="header">
      <div class="title">Vegetarian Recipes</div>
      <div class="subtitle">A way to explore Vegetarian Recipes</div>
    </div>
    <div class="main">
      <div class="container-fluid">
        <div class="row">
          <div class="col-md-8 som-column">
            <som
                    :neurons="neurons"
                    v-on:selected="selected"></som>
          </div>
          <div class="col-md-4 recipe-list-column">
            <recipe-list
                    :neuron="selectedNeuron"></recipe-list>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios'
import Som from './components/Som'
import RecipeList from './components/RecipeList'
import Chart from 'chart.js/src/chart.js'

Chart.defaults.global.maintainAspectRatio = false

export default {
  name: 'app',
  data () {
    return {
      neurons: [],
      selectedNeuron: undefined
    }
  },
  methods: {
    selected (neuron) {
      this.selectedNeuron = neuron
    }
  },
  beforeCreate () {
    var self = this
    axios.get('/neuron')
      .then(function (response) {
        self.neurons = response.data
      }).catch(function (error) {
        console.log('Failed to contact server', error)
      })
  },
  components: {
    Som,
    RecipeList
  }
}
</script>

<style lang="scss">
html, body {
  background-color: #fcfcfc !important;
  color: #586E75 !important;

  #app {
    font-family: 'Avenir', Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;

    .header {

      background-color: #073642;
      color: #fcfcfc;

      text-align: left;

      .title {
        padding: 10px 0 0 40px;
        font-size: 32px;
        font-weight: bold;
      }
      .subtitle {
        padding: 2px 0 2px 75px;
        background-color: #002B36;
        font-size: 16px;
      }
    }

    .main {

      > .container-fluid {
        > .row {
          > .som-column {
            padding-top : 10px;
          }

          > .recipe-list-column {
            padding-top : 10px;
            height: calc(100vh - 85px);
            min-height: calc(100vh - 85px);
            max-height: calc(100vh - 85px);
            overflow: auto;
          }
        }
      }
    }
  }
}
</style>
