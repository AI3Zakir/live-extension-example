<?php
/**
 * Created by PhpStorm.
 * User: lamiev
 * Date: 10/18/18
 * Time: 1:26 AM
 */
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>PHP-CPP EXAMPLE</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css" rel="stylesheet"
          integrity="sha384-MCw98/SFnGE8fJT3GXwEOngsV7Zt27NXFoaoApmYm81iuXoPkFOJwJ8ERdknLPMO" crossorigin="anonymous">
</head>
<body>
<div class="container">
    <ul>
        <li>
            <a href="/?todos=all">Fetch all todos</a>
        </li>
        <li>
            <a href="/?todos=userId1">Fetch data for UserID 1</a>
        </li>
        <li>
            <a href="/?todos=userId2">Fetch data for UserID 2</a>
        </li>
    </ul>
    <?php
    if ($_GET['todos'] == 'all') {
        echo make_request_with_cpp('http://jsonplaceholder.typicode.com/todos');
    } elseif ($_GET['todos'] == 'userId1') {
        echo make_request_with_cpp('http://jsonplaceholder.typicode.com/todos?userId=1');
    } elseif ($_GET['todos'] == 'userId2') {
        echo make_request_with_cpp('http://jsonplaceholder.typicode.com/todos?userId=2');
    }
    ?>
</div>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js"
        integrity="sha384-ChfqqxuZUCnJSK3+MXmPNIyE6ZbWh2IMqE241rYiqJxyMiZ6OW/JmZQ5stwEULTy"
        crossorigin="anonymous"></script>
</body>
</html>
