<template xmlns:v-on="http://www.w3.org/1999/xhtml" xmlns:v-bind="http://www.w3.org/1999/xhtml">
  <div class="som container-fluid">
    <div class="row" v-for="(row, i) in neurons">
      <div class="col-md-1 neuron" v-for="(neuron, j) in row">
        <img :src="neuron.image" v-on:click="selectNeuron(neuron, i, j)" v-bind:class="{ selected: isSelected(neuron) }">
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'som',
  props: ['neurons'],
  data () {
    return {
      selectedPosition: -1
    }
  },
  methods: {
    selectNeuron (neuron, i, j) {
      this.selectedPosition = neuron.pos
      this.$emit('selected', neuron)
    },
    isSelected (neuron) {
      return this.selectedPosition === neuron.pos
    }
  }
}
</script>

<style lang="scss" scoped>
.som {
  .row {
    padding-bottom: 10px;
    .neuron {
      width: 12.5% !important;

      > img {
        height: 100%;
        width: 100%;

        border: 1px solid #EEE8D5;
        border-radius: 10px;

        &:hover {
          border: 3px solid #859900;
          cursor: pointer;
        }

        &.selected {
          border: 5px solid #859900;
        }
      }
    }
  }
}
</style>
