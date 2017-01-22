<template>
  <div class='ingredient-chart'>
    <div class="row">
      <div class="col-md-4 ingredients">
        <div class="ingredient" v-for="ingredient in sortedIngredients" :title="ingredient.food">{{ ingredient.food }}</div>
      </div>
      <div class="col-md-8 chart">
        <canvas :ref='recipe._id' :height="canvasHeight"></canvas>
      </div>
    </div>
  </div>
</template>

<script>
import Chart from 'chart.js/src/chart.js'

export default {
  name: 'ingredient-chart',
  props: [
    'recipe'
  ],
  data () {
    return {
      chart: undefined
    }
  },
  mounted () {
    this.show()
  },
  updated () {
    if (this.chart !== undefined) {
      this.chart.destroy()
    }
    this.show()
  },
  computed: {
    canvasHeight () {
      return 18 * this.recipe.ingredients.length
    },
    sortedIngredients () {
      return [...this.recipe.ingredients].sort((a, b) => b.weight - a.weight)
    }
  },
  methods: {
    show () {
      let ctx = this.$refs[this.recipe._id]
      ctx.height = this.canvasHeight
      this.chart = new Chart(ctx, {
        type: 'horizontalBar',
        data: {
          labels: this.sortedIngredients.map((i) => i.food),
          datasets: [{
            backgroundColor: 'rgba(0, 43, 54, 1)',
            borderColor: '#EEE8D5',
            hoverBackgroundColor: 'rgba(0, 43, 54, 1)',
            hoverBorderColor: '#EEE8D5',
            borderWidth: 1,
            data: this.sortedIngredients.map((i) => i.weight)
          }]
        },
        options: {
          animation: false,
          legend: { display: false },
          scales: {
            xAxes: [{
              display: false
            }],
            yAxes: [{
              display: false,
              barThickness: 17,
              barPercentage: 1,
              categoryPercentage: 1,
              gridLines: { display: false }
            }]
          }
        }
      })
    }
  }
}
</script>

<style lang='scss' scoped>
.ingredient-chart {
  .ingredients {
    padding-right: 0px;
    .ingredient {
      overflow: hidden;
      white-space: nowrap;
      text-overflow: ellipsis;
      font-size: 14px;
      height: 18px;
    }
  }
  .chart {
    padding: 0px;
  }
}
</style>
