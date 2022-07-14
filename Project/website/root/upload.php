<?php
$uploaddir = './';
$uploadfile = $uploaddir . basename($_FILES['UserFile']['name']);

if (move_uploaded_file($_FILES['UserFile']['tmp_name'], $uploadfile)) {
    echo "Le fichier est valide, et a été téléchargé
           avec succès. Voici plus d'informations :\n";
} else {
    echo "Attaque potentielle par téléchargement de fichiers.
          Voici plus d'informations :\n";
}

?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>Plage</title>
		<style>
			body
			{
				background-image: url("beach.jpg");
				background-repeat: no-repeat;
				background-size: 160%;
			}

			h1
			{
				text-align: center;
				font-family: playa;
				font-size: 1000%;
				color: palevioletred;
			}

			@font-face {
			font-family: "playa";
			src: url("playa.ttf");
		}

		@font-face {
			font-family: "love";
			src: url("Love.ttf");
		}

		.pam
		{
			float: right;
			float: inline-end;
		}

		.txt
		{
				font-family: love;
				position: relative;
				right: -20%;
				background-color: aliceblue;
				font-size: 150%;
			}
		</style>
	</head>

	<body>
		<form>
			<input type="button" onclick="window.location.href = '/';" value="Retour a l'acceuil" />
		</form>
	</body>
</html>