<?php
    $file_name = "infos.txt";
    $send = 0;
    $temperature = -1;
    $input = -1;

    if (file_exists($file_name))
    {
        $lines = file($file_name);
        var_dump($lines);

        $send = explode(':', $lines[0])[1];
        $temperature = explode(':', $lines[1])[1];
        $input = explode(':', $lines[2])[1];
    }
?>
