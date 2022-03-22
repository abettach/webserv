<?php
setcookie("qwerty", "219ffwef9w0f", time() + (86400 * 30), '/');
?>

<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
	<title>Document</title>
	<style>
		html {
			font-family: sans-serif;
			height: 100%;
		}

		body {
			height: 100vh;
			color: #FFB399;
		}

		.size {
			font-size: 66px;
			color: #FFB399;
		}

		.-mt {
			margin-top: -30px;
		}
	</style>
</head>

<body class="bg-Light overflow-hidden">
	<div class="mx-auto h-75 d-flex w-100 justify-content-center -mt">
		<div class="d-flex flex-column my-auto">
			<h1 class="size text-center">Logged in</h1>
			<a href="home.php" class="btn btn-danger text-center mt-5 p-3 rounded-pill fs-3 text-decoration-none">Go Home</a>
		</div>
	</div>
</body>

</html>