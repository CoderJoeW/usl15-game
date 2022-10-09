<?php

namespace Usl\Database\PDO;

use Usl\Database\PDO\DatabaseConstants;
use Usl\Database\PDO\Database;

class Model{ 
    public static function getInstance($type){
        $credentials = DatabaseConstants::DB_CREDENTIALS[$type];
        
        return new Database($credentials['username'], $credentials['password'], $credentials['host'], $credentials['db']);
    }
}
?>