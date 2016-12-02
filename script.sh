barra(){ 
echo "_____________" 
} 
buscar(){ 
echo "Buscar contacto por correo" 
read busqueda 
cat ~/.agenda | grep ${busqueda} 
echo "Presione enter para continuar" 
read continua 
barra 
menu 
} 

mostrartodo(){ 
echo "Todos los contactos registrados" 
cat ~/.agenda
echo "Presione enter para continuar" 
read continua 
barra 
menu 
} 

anadir(){ 
echo "Inserte nombre" && read nombre 
barra 
echo "Inserte telefono(s)" && read telefono 
barra 
echo "Inserte correo electronico" && read correo 
echo "Nombre: ${nombre} - Telefono(s): ${telefono} - E-mail : ${correo}" >> ~/.agenda 
barra 
menu 
} 
borrar(){ 
echo "Inserte nombre a borrar" 
read nombre_borrar 
cat ~/.agenda | grep -v "${nombre_borrar}" > ~/.agenda2 && mv ~/.agenda2 ~/.agenda 
barra 
menu 
} 
menu(){ 
echo "Que desea hacer?" 
echo "1.- Añadir contacto" 
echo "2.- Buscar Contacto" 
echo "3.- Borrar contacto" 
echo "4.- Mostrar todo"
echo "5.- Salir" 
read seleccion 
case ${seleccion} in 
1) 
barra 
anadir 
;; 
2) 
barra 
buscar 
;; 
3) 
barra 
borrar 
;; 
4) 
barra
mostrartodo
;; 
5) 
exit 
;; 
*) 
echo "Error" 
echo "Se repetira el menu" 
menu 
;; 
esac 
} 

echo "Agenda techProgramming" 
menu

 
