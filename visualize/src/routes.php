<?php

use Psr\Http\Message\ResponseInterface;
use Psr\Http\Message\ServerRequestInterface;

$app->get('/', function (ServerRequestInterface $request, ResponseInterface $response, $args) {
    return $this->view->render($response, 'index.phtml');
});


$app->get('/neuron', function($request, $response, $args) use ($container) {
    $mapSize = $container->get('settings')['som']['size'];
    $search = [
        'index' => 'recipes',
        'type' => 'recipe',
        'body' => [
            'size' => 0,
            'aggs' => [
                'neurons' => [
                    'terms' => ['field' => 'som.pos', 'size' => $mapSize ],
                    'aggs' => [
                        'bmu' => [
                            'top_hits' => [
                                'sort' => [['som.dist' => ['order' => 'asc']]],
                                'size' => 1,
                                '_source' => [ 'includes' => ['som', 'image']]
                            ]
                        ]
                    ]
                ]
            ]
        ]
    ];


    $result = $this->elasticsearch->search($search);
    $rows = [];
    foreach ($result['aggregations']['neurons']['buckets'] as $bucket) {
        $neuron = $bucket['bmu']['hits']['hits'][0]['_source']['som'];
        $neuron['_id'] = $bucket['bmu']['hits']['hits'][0]['_id'];
        $neuron['image'] = $bucket['bmu']['hits']['hits'][0]['_source']['image'];
        if (!isset($rows[$neuron['row']]))
            $rows[$neuron['row']] = [];

        $rows[$neuron['row']][$neuron['col']] = $neuron;
    }

    $response = $response->withJson($rows);
    return $response;
});


$app->get('/neuron/{pos}[/{start}]', function($request, $response, $args) {
    $search = [
        'index' => 'recipes',
        'type' => 'recipe',
        'body' => [
            'from' => (isset($args['start'])) ? $args['start'] : 0,
            'size' => 10,
            'query' => [
                'term' => [
                    'som.pos' => $args['pos']
                ]
            ],
            'sort' => [
                ['som.dist' => ['order' => 'asc' ] ]
            ]
        ]
    ];

    $result = $this->elasticsearch->search($search);
    $recipeList = [];
    foreach ($result['hits']['hits'] as $full_recipe) {
        $recipe = [
            '_id' => $full_recipe['_id'],
            'image' => $full_recipe['_source']['image'],
            'label' => $full_recipe['_source']['label'],
            'source' => $full_recipe['_source']['source'],
            'sourceIcon' => $full_recipe['_source']['sourceIcon'],
            'url' => $full_recipe['_source']['url'],
            'ingredients' => $full_recipe['_source']['ingredients'],
            'nutrient' => $full_recipe['_source']['som']['nutrient']
        ];
        $recipeList[] = $recipe;
    }

    $response = $response->withJson([
        'total' => $result['hits']['total'],
        'recipes' => $recipeList
    ]);

    return $response;
});