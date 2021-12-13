# Contiki-anemometro
Codigo anemometro en contiki-ng cc2650 launchpad-XL

# Pasos para compilar y cubir un archivo al MCU cc2650
1. Crear una carpeta con el archivo .c (OJO deben tener el mismo nombre)

2. Crear el archivo makefile el nombre del proyectodebe ser el mimso que 
la carpeta (Ver ejemplo en el libro. Basicamente son 3 cosas: nombre, 
directorio de contiki-ng o contiki, y el makefile.include)

3. Compilar los archivos para pasar al launchpad. (En todo caso, revisar 
la carpeta de platform y ver los targets disponibles)
	En contiki:
	sudo make TARGET=srf06-cc26xx BOARD=launchpad/cc2650
	En contiki-ng
	sudo make TARGET=cc26x0-cc13x0 BOARD=launchpad/cc2650	

4. Usar el SmartRF Flash Programm de texas para subir el archivo .hex o el .bin 
(en contiki-ng el archivo se encuentra en la carpeta build)

5. Conectar el puerto serial con el CC2650 con el sgte comando
	En contiki
	sudo make TARGET=srf06-cc26xx BOARD=launchpad/cc2650 login PORT=/dev/ttyACM0
	En contiki-ng
	sudo make TARGET=cc26x0-cc13x0 BOARD=launchpad/cc2650 login PORT=/dev/ttyACM0


Nota. usar make TARGET=... savetarget para evitar escribir toda la linea y solo escribir "make"
