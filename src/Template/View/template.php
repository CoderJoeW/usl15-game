<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <meta name="title" content="Cheap Stream | Watch the newest movies for $2/m">
        <meta name="description" content="CheapStream searches the web to provide you the lastest movies for super cheap with no annoying ads">
        <meta name="keywords" content="cheapstream, cheap, stream, free, movies, noads, no, ads, adfree, watch, movies, online ">
        <meta name="robots" content="index, follow">
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <meta name="language" content="English">
        <meta name="revisit-after" content="1 days">

        <script>document.getElementsByTagName("html")[0].className += " js";</script>
        <script src="/public/js/codyhouse/scripts.js" defer></script>
        <script src="/public/js/codyhouse/dark-mode.js" defer></script>
        <!-- favicon -->
        <link rel="icon" type="image/svg+xml" href="">

        <title>CheapStream</title>
    </head>
    <body>
        <!-- font -->
        <link href="/public/css/global/google-fonts.css" rel="stylesheet">

        <link rel="stylesheet" href="/public/css/codyhouse/style.css">
        
        <?php
            if($cssFiles != null){
                $cssFilesCount = count($cssFiles);
                
                for($i = 0; $i < $cssFilesCount; $i++){
                    echo '<link rel="stylesheet" href="'.$cssFiles[$i].'">';
                }
            }
        ?>

        <?php
            if($jsFiles != null){
                $jsFilesCount = count($jsFiles);

                for($i = 0; $i < $jsFilesCount; $i++){
                    echo '<script src="'.$jsFiles[$i].'"></script>';
                }
            }
        ?>

        <div class="app-ui js-app-ui">
            <!-- header -->
            <?php require_once(__DIR__.'/template_header.php'); ?>
                
            <!-- navigation -->
            <?php require_once(__DIR__.'/template_navigation.php'); ?>
            
            <!-- main content -->
            <main class="app-ui__body padding-md js-app-ui__body">
                <div class="grid gap-sm" id="main-grid">
                    <?php require_once($viewPath); ?>
                </div>
            </main>
        </div>
    </body>
</html>
