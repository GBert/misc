<?php 
	$page = $_GET['page'];
?>

<!DOCTYPE html>
<html id="root" style="font-size: 10px; background-color: #ccc;">
	<head>
		<title>Integrator</title>
		<link rel="stylesheet" type="text/css" href="styles/main.css?<?php echo time(); ?>">
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<noscript>Javascript ist erforderlich!</noscript>

	</head>
	<body>
		<?php include 'php/'.$page.'.php'; ?>
	</body>
</html>
