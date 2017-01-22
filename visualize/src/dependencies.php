<?php

use Elasticsearch\ClientBuilder;

// DIC configuration
$container = $app->getContainer();

// monolog
$container['logger'] = function ($c) {
    $settings = $c->get('settings')['logger'];
    $logger = new Monolog\Logger($settings['name']);
    $logger->pushProcessor(new Monolog\Processor\UidProcessor());
    $logger->pushHandler(new Monolog\Handler\StreamHandler($settings['path'], $settings['level']));
    return $logger;
};

// twig view renderer
$container['view'] = function ($c) {
    $settings = $c->get('settings')['renderer'];
    $production = $c->get('settings')['production'];
    $view = new \Slim\Views\Twig($settings['template_path'], [
        'cache' => false //$settings['cache_path']
    ]);

    // Instantiate and add Slim specific extension
    $basePath = rtrim(str_ireplace('index.php', '', $c['request']->getUri()->getBasePath()), '/');

    $view->addExtension(new Slim\Views\TwigExtension($c['router'], $basePath));

    $twig = $view->getEnvironment();
    $function = new Twig_SimpleFunction('asset', function ($filepath) use ($settings) {
        return $settings['asset_path'].$filepath;
    });
    $twig->addFunction($function);
    $twig->addGlobal('production', $production);

    return $view;
};

$container['elasticsearch'] = function ($c) {
    $esSettings = $c->get('settings')['elasticsearch'];
    $url = "http://{$esSettings['host']}:{$esSettings['port']}";
    $client = ClientBuilder::create()->setHosts([$url])->build();

    return $client;
};
