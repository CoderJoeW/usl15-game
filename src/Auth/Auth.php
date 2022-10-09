<?php

namespace Usl\Auth;

class Auth{
    public function authRequired(){
        if(!$this->isLoggedIn()){
            header('Location: /home/login/');    
        }
    }
    
    public function isLoggedIn(){
        if(empty($_SESSION)){
            return false;   
        }else{
            return $_SESSION['LoggedIn'];
        }
    }
}
?>