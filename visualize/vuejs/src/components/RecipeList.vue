<template xmlns:v-on="http://www.w3.org/1999/xhtml" xmlns:v-bind="http://www.w3.org/1999/xhtml">
  <div class="container-fluid recipe-list">
    <nav aria-label="..." v-show="totalPage != 0">
      <ul class="pager">
        <li class="previous" v-show="hasPrev">
          <a href="#" v-on:click="navigate(-1)"><span aria-hidden="true">&larr;</span> Prev</a></li>
        <li class="next" v-show="hasNext">
          <a href="#" v-on:click="navigate(+1)">Next <span aria-hidden="true">&rarr;</span></a></li>
      </ul>
    </nav>

    <div class="row nutrient-lists">
      <div class="high-nutrient" v-show="highNutrient != ''">
        <span class="glyphicon glyphicon-chevron-up" aria-hidden="true"></span> {{ highNutrient }}
      </div>
      <div class="low-nutrient" v-show="lowNutrient != ''">
        <span class="glyphicon glyphicon-chevron-down" aria-hidden="true"></span> {{ lowNutrient }}
      </div>
    </div>

    <div class="row recipe" v-for="(recipe, index) in recipes">
      <div class="col-md-3"><img class="recipe" :src="recipe.image"/></div>
      <div class="col-md-9">
        <div class="title"><a :href="recipe.url">{{ recipe.label }}</a></div>
        <div class="source">from {{ recipe.source }}</div>
        <ingredient-chart v-bind:recipe="recipe"></ingredient-chart>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios'
import IngredientChart from './IngredientChart'

export default{
  name: 'recipe-list',
  props: [
    'neuron'
  ],
  components: {
    IngredientChart
  },
  data () {
    return {
      recipes: [],
      pageSize: 10,
      totalPage: 0,
      page: 0,
      hasPrev: false,
      hasNext: true
    }
  },
  watch: {
    neuron (newNeuron) {
      if (newNeuron !== undefined) {
        this.page = 0
        this.fetch(newNeuron, 0)
      }
    }
  },
  computed: {
    highNutrient () {
      return (this.recipes.length === 0)
          ? []
          : this.recipes[0].nutrient.slice(-6).reverse().map((n) => n.label).join(', ')
    },
    lowNutrient () {
      return (this.recipes.length === 0)
          ? []
          : this.recipes[0].nutrient.slice(0, 6).map((n) => n.label).join(', ')
    }
  },
  methods: {
    ingredients (recipe) {
      return [...recipe.ingredients].sort((a, b) => b.weight - a.weight).map((r) => r.food) // .join(', ')
    },
    fetch (neuron, from) {
      let self = this
      axios.get('/neuron/' + neuron.pos + '/' + from)
        .then(function (response) {
          self.recipes = response.data.recipes
          self.totalPage = Math.ceil(response.data.total / self.pageSize)
        }).catch(function (error) {
          console.log('Failed to contact server', error)
        })
    },
    navigate (offset) {
      if ((offset < 0 && this.hasPrevPage()) || (offset > 0 && this.hasNextPage())) {
        let from = ((this.page + offset) * this.pageSize)
        this.fetch(this.neuron, from)
        this.page += offset
        this.hasNext = this.hasNextPage()
        this.hasPrev = this.hasPrevPage()
      }
    },
    hasPrevPage () { return (this.page - 1 >= 0) },
    hasNextPage () { return (this.page + 1 < this.totalPage) }
  }
}
</script>

<style lang="scss" scoped>
.recipe-list {
  > nav {
    ul.pager {
      li > a {
        background-color: #073642;
        color: #FDF6E3;
        border: 1px solid #002B36;
        &:hover {
          background-color: #002B36;
          color: #93A1A1;
        }
      }
    }
  }
  > .row {
    padding: 5px 0 5px 0;
    text-align: left;

    &.nutrient-lists {
      .high-nutrient {
        color: #859900;
      }
      .low-nutrient {
        color: #DC322F;
      }
      ul.nutrient-list {
        padding-left: 0px;
        li.nutrient {
          display: block;
        }
      }
    }

    &.recipe {
      img.recipe {
        width: 100%;
        height: 100%;

        border: 1px solid #EEE8D5;
        border-radius: 10px;
      }

      .source {
        font-style: italic;
      }
    }
  }
}
</style>
