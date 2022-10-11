<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <meta name="language" content="English">
        <meta name="revisit-after" content="1 days">

        <script>document.getElementsByTagName("html")[0].className += " js";</script>
        <script src="/public/js/codyhouse/scripts.js" defer></script>
        <script src="/public/js/codyhouse/dark-mode.js" defer></script>
        <!-- favicon -->
        <link rel="icon" type="image/svg+xml" href="">

        <link rel="stylesheet" href="/public/css/codyhouse/style.css">

        <title>USL</title>

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
    </head>
    <body>
        <div class="app-ui js-app-ui">
            <?php
            if($useHeader){
                require_once('header.php');
            }
            ?>

            <!-- main content -->
            <?php require_once($viewPath); ?>
        </div>
    </body>
</html>
