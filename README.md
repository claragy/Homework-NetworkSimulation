README
Les fichiers d'inclusion de GTest et TCLAP ne sont pas inclus.
Si GTest et TCLAP ne sont pas installés sur la machine :
-Pour GTest, télécharger le code source, le compiler, créer un dossier "lib" et un dossier "include" dans le dossier principal du projet, copier dans lib : "ligtest.a" et "libgest_main.a", et copier dans include le dossier "gtest" qui est dans googletest. 
-Pour TCLAP, télécharger le code source et copier le dossier "tclap" entier (qui est dans tclap-1.2.2/include/) dans le dossier include du dossier principal du projet.
