<?php
session_start();

require_once __DIR__.'/config.php';

if(isset($argv)){
    $options = getopt('',array('uri::'));
    
    $url = explode('/', ltrim($options['uri'],'/'));
}else{
    if($engine == 'nginx'){
        $url = isset($_SERVER['REQUEST_URI']) ? explode('/', ltrim($_SERVER['REQUEST_URI'],'/')) : '/';
    }else if($engine == 'apache'){
        $url = isset($_SERVER['PATH_INFO']) ? explode('/', ltrim($_SERVER['PATH_INFO'],'/')) : '/';
    }
}

if ($url == '/' || empty($url[0])){
    require_once("{$rootPath}Home/Controller/Home.php");

    $controller = new Home();
    
    $view = $controller->index();
    
    $controller->init();
    
    if($controller->data != null){
        extract($controller->data, EXTR_PREFIX_SAME, 'wddx');       
    }

    $cssFiles = $controller->cssFiles;
    $jsFiles = $controller->jsFiles;
    
    $viewPath = "{$rootPath}Home/View/{$view}.php";

    if(file_exists($viewPath)){
        require_once("{$rootPath}Template/View/template.php");
    }
}else{
    //The first element should be a controller
    $requestedController = ucfirst($url[0]); 

    // If a second part is added in the URI, 
    // it should be a method
    $requestedAction = isset($url[1]) && $url[1] != '' ? ucfirst($url[1]) : 'Index';

    // The remain parts are considered as 
    // arguments of the method
    $requestedParams = array_slice($url, 2); 

    // Check if controller exists.
    $ctrlPath = "{$rootPath}{$requestedController}/Controller/{$requestedController}.php";

    if (file_exists($ctrlPath)){
        $view = null;
        
        require_once($ctrlPath);

        $controllerObj  = new $requestedController();

        //Check that action exist or return 404
        if(!method_exists($controllerObj,$requestedAction)){
            header('HTTP/1.1 404 Not Found');
        
            require_once("{$rootPath}Template/View/404.php");
        }
        
        if($requestedAction != ''){
            $view = $controllerObj->$requestedAction($requestedParams);
        }else{
            // If no action is specified we default to loading the index
            $view = $controllerObj->index($requestedParams);
        }
        
        $controllerObj->init();
        
        if($controllerObj->data != null){
            extract($controllerObj->data, EXTR_PREFIX_SAME, 'wddx');   
        }
        
        $cssFiles = $controllerObj->cssFiles;
        $jsFiles = $controllerObj->jsFiles;
        
        if($view != null){
            // Store view path
            $viewPath = "{$rootPath}{$requestedController}/View/{$view}.php";

            // Check if view uses template
            if($controllerObj->useTemplate){

                if(file_exists($viewPath)){
                    require_once("{$rootPath}Template/View/{$controllerObj->template}.php");
                }
            }else{
                // If the view does not use the template then we just load the view
                if(file_exists($viewPath)){
                    require_once($viewPath);
                }
            }
        }
    }else{
        header('HTTP/1.1 404 Not Found');
        
        require_once("{$rootPath}Template/View/404.php");
    }
}
