<?php

use Usl\Welcome;

class Home extends Welcome{

    public function index(){
        $this->data['test'] = 'Welcome to the homepage';
        
        return 'index';
    }

}
?>