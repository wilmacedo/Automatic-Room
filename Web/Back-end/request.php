<?php

    include 'data.php';

    $value = $_GET['value'];

    switch ($value)
    {
        case -1:
            echo $send;
            break;
        case 0:
            echo $temperature;
            break;
        case 1:
            echo $input;
            break;
        default:
            echo "response:error 01";
    }
?>
