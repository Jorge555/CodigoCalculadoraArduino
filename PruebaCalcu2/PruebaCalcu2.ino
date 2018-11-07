#include <LiquidCrystal.h> //Librería para la pantalla LCD 16*2.
#include <LiquidCrystal_I2C.h> //Librería para el control del LCD por la interfaz I2C.
#include <Keypad.h> //Librería para el control del keypad.
#include <Wire.h> //Librería para comunicación I2C.

//Con este código se utiliza la interfaz 12c

LiquidCrystal_I2C lcd(0x27,16,2); //Variable del LCD.
const byte Filas = 4; //Número de filas.
const byte Columnas = 4; //Número de columans.

char Teclado [Filas][Columnas]=
{ //Declaramos nuestro teclado.
 {'1', '4', '7', 'X'},
 {'2', '5', '8', '0'},
 {'3', '6', '9', '='},
 {'+', '-', '*', '/'}
};

byte FilaPines[Filas]={3,2,1,0}; //configuración de filas para los pines de arduino.
byte ColumnaPines[Columnas]={7,6,5,4}; // configuración de columnas para los pines de arduino.

Keypad miTeclado = Keypad (makeKeymap(Teclado),FilaPines, ColumnaPines, Filas, Columnas); //Creamos el objeto teclado.


boolean valorActual = false;   /*----Variables de Control---- */
boolean siguiente = false;
boolean final = false;
String num1,num2,num3;
int total;
int movimiento;
char op;
float r1,r2;
float decimal;
int contador=0;

/* ******************************Cada vez que se reinicie Arduino, se ejecuta la función setup.******************************************
  *************************************************************************************************************************************** */
void setup()
{
Wire.begin();
lcd.begin();
lcd.setCursor(2,0);           //Posicion columna-fila.
lcd.print("Proyecto Final");
lcd.setCursor(6,1);
lcd.print("UMG");
delay(2500);                  //Tiempo en pantalla.
lcd.clear();                  //Limpia pantalla.
lcd.setCursor(0,0);
lcd.print(">");
}

/* *************Estructura del bucle del programa************************ */

void loop()
{
char tecla = miTeclado.getKey(); //Obtenemos la tecla que se presiona y la guardamos en la variable tecla.
int longitudDelNumero;

//Verifico que se presionó una tecla

if (tecla != NO_KEY && (tecla=='1'||tecla=='2'||tecla=='3'||tecla=='4'||tecla=='5'||tecla=='6'||tecla=='7'||tecla=='8'||tecla=='9'||tecla=='0'))
{ //Si se presiona una tecla, se guardara en el Primer Numero.
  //Inicializamos las variables.
if (contador==1)
    {
      lcd.clear();
      valorActual = false;
      final = false;
      num1 = "";
      num2 = "";
      total = 0;
      op = ' ';
      lcd.setCursor(0,0);
      lcd.print(">");
      contador=0; 
    }
    
if (valorActual != true){
num1 = num1 + tecla; //Guarda el primer numero.
longitudDelNumero = num1.length();
movimiento = longitudDelNumero;
lcd.setCursor(1, 0);
lcd.print(num1);  //Imprime el primer numero.
}
else {
num2 = num2 + tecla; //Guarda el segundo numero.
longitudDelNumero = num2.length();
lcd.setCursor(movimiento+2, 0);
lcd.print(num2); //Imprime el segundo numero.
final = true;
}
}
else if ((tecla == '+' || tecla == '-' || tecla == '*' || tecla == '/') && valorActual == false && tecla != NO_KEY) //Si se pulsa una tecla de Operacion.
{
if (valorActual == false){
            valorActual = true;
            op = tecla; //La operacion depende de la tecla que se presione.
            lcd.setCursor(movimiento+1,0);
            lcd.print(op); //Imprime la operacion que seleccionamos.
}
}
 else if (final == true && tecla != NO_KEY && tecla == '=') //Si se presiona el boton de igual (=).
{
  //Dependiendo de la operación seleccionada, se procede al cálculo de las distintas operaciones.
 switch (op)
  {
    case '+': //Operacion SUMA.
    total = num1.toInt() + num2.toInt();//toInt Convierte el String en Numero Entero.
    break;

    case '-': //Operacion RESTA.
    total = num1.toInt() - num2.toInt();
    break;

    case '*': //Operacion MULTIPLICACION.
    total = num1.toInt() * num2.toInt();
    break;

    case '/': //Operacion DIVISION.
      if(num2.toInt()==0){ //Si el segundo numero para la division es 0,
 total = ' ';              //Va imprimir el error que se define más adelante.
 }else
  {
    total = (num1.toInt() / num2.toInt()); //Hace la division normal.
    r1=num1.toInt(); //Asignacion en r1 = Calculo de numeros decimales.
    r2=num2.toInt(); //Asignacion en r2 = Calculo de numeros decimales.
    decimal=r1/r2;
  }
   break;
  default:
   //si nada más coincide, elija esta opcion
  break;
}
num3=String(total);
lcd.clear();
lcd.setCursor(15,0);
lcd.autoscroll();
if(total==' '){            //Declaramos el error.
lcd.print("Sintax Error"); //Imprimimos el mensaje de error.
}else if(num3.length()>5){ //Si el resultado es muy grande para imprimir en pantalla.
lcd.print("No hay memoria");
}else{
  if (op == '/') {lcd.print(decimal,2);} //Operacion para los decimales.
  else{lcd.print(total);};
      contador=1;
     }
lcd.noAutoscroll();
}
else if (tecla == 'X' && tecla!= NO_KEY){  //Si presionamos la tecla "AC" se borran los valores.
lcd.clear();
valorActual = false;
final = false;
num1 = "";
num2 = "";
total = 0;
op = ' ';
lcd.setCursor(0,0);
lcd.print(">");
}
}

