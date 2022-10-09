<?php

namespace Usl;

use Usl\Auth\Auth;

class Welcome extends Auth{
    public $data = null;
    public $db = null;
    public $template = 'template';
    public $useTemplate = true;
    public $authRequired = true;
    
    public $cssFiles = null;
    public $jsFiles = null;

    public $userId = null;

    public function __construct(){
        if(!empty($_SESSION)){
            $this->userId = $_SESSION['user_id'];
        }
    }
    
    public function init(){        
        
    }
}

?>