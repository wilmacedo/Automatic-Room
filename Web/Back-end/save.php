<?php

    include 'data.php';

    if ($temperature == -1 || $input == -1)
    {
        echo "response:error 01";
        return;
    }

    $new_send = $_GET['send'];
    $new_temperature = $_GET['temp'];
    $new_input = $_GET['input'];

    $content = 
        "Send:" . $new_send .
        "Temperature:" . $new_temperature .
        "Input:" . $new_input;

    if (file_put_contents($file_name, $content))
    {
        echo "response:ok";
    } else
    {
        echo "response:error 02";
    }
?>
