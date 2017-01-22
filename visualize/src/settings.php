<?php

$production = false;

return [
    'settings' => [
        'production' => $production,
        'displayErrorDetails' => ($production) ? false : true,
        'addContentLengthHeader' => false,

        // Renderer settings
        'renderer' => [
            'template_path' => __DIR__ . '/../templates/',
            'cache_path' => ($production) ? __DIR__ . '/../cache/' : false,
            'asset_path' => ($production) ? '/static' : 'http://localhost:8080'
        ],

        // Monolog settings
        'logger' => [
            'name' => 'slim-app',
            'path' => __DIR__ . '/../log/app.log',
            'level' => ($production) ? \Monolog\Logger::INFO : \Monolog\Logger::DEBUG,
        ],

        // Elasticsearch Settings
        'elasticsearch' => [
            'host' => ($production) ? '127.0.0.1' : 'elasticsearch-5',
            'port' => 9200
        ],

        'som' => [
            'size' => 48
        ]
    ]
];