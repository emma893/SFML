# SFML se descarga aqui 
https://www.sfml-dev.org/download/sfml/2.5.1 o se lo instala para que los modulos se puedan usar.

# se crea un archivo de texto (con el nombre main, por ejemplo) y se le da la extension .cpp para programar en c++ y demas!
nano main.cpp

# Se compila el .cpp, y se genera un archivo con el mismo nombre con extension .o que es el codigo maquina.
g++ -c main.cpp

# Ahora se genera un exe con el .o y los modulos de SFML que se usaron en el programa.
g++ main.o -o sfml-circle -lsfml-graphics -lsfml-window -lsfml-system
