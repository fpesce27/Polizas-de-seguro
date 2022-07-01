    #include<iostream>
#include<string.h>
#include<stdlib.h>
#include<conio.h>

using namespace std;

struct incidente
{
    int CodigoIncidente;
    char FechaHora[14]; // AAAAMMDDHH:MM
    int DNIAsegurado;
    int DNIOtroConductor;
    int NroPoliza;
    char Calle[50];
    int Altura;
};

struct poliza
{
    int NroPoliza;
    int DNI;
    char Nombre[30];
    char Apellido[30];
    bool CuotaAlDia;
    char Patente[10];
    bool Activa;
    int CdadIncidentes;
};

struct nodo
{
    poliza info;
    nodo *sgte;
};

void sobreescribir(char ruta[],nodo *inicio,bool &correcto)
{
    FILE *f;
    poliza x;

    if (f=fopen(ruta,"wb"))
    {
        while(inicio)
        {
            x = inicio->info;
            fwrite(&x,sizeof(poliza),1,f);
            inicio=inicio->sgte;
        }
        fclose(f);
        correcto=1;
    }
    else
        cout << "Error al abrir el archivo" <<endl;

}

void escribirReporteCSV(nodo *inicio)
{
    FILE *f;
    f = fopen("ReportePoliza.csv","wt");
    fprintf(f,"Nro Poliza;DNI;Nombre;Apellido;Patente;Activa;Cantidad de Incidentes\n");
    while (inicio)
    {
        if (inicio->info.CuotaAlDia!=1)
        {
            fprintf(f,"%d;%d;%s;%s;%s;%d;%d\n",
            inicio->info.NroPoliza,inicio->info.DNI,inicio->info.Nombre,
            inicio->info.Apellido,inicio->info.Patente,inicio->info.Activa,
            inicio->info.CdadIncidentes);
        }
        inicio=inicio->sgte;
    }
    fclose(f);
    return;
}

void escribirReporteHTML(nodo * inicio)
{
    FILE *f;
    f = fopen("ReportePoliza.html","wt");
    fprintf(f,"<html><body>\n");
    fprintf(f,"<h1>Reporte de Polizas que no tienen la cuota al dia<body>\n");
    fprintf(f,"<table border=1>\n");
    fprintf(f,"<th>Nro Poliza</th><th>DNI</th><th>Nombre</th><th>Apellido</th><th>Patente"
              "</th><th>Activa</th><th>Cantidad de Incidentes</th>\n");

    while (inicio)
    {
        if (inicio->info.CuotaAlDia != 1)
        {
            fprintf(f,"<tr>\n");
            fprintf(f,"<td>%d</td><td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>%d</td><td>%d</td>",
            inicio->info.NroPoliza, inicio->info.DNI,inicio->info.Nombre,inicio->info.Apellido,inicio->info.Patente,inicio->info.Activa,
            inicio->info.CdadIncidentes);
            fprintf(f,"</tr>\n");  
        }
        inicio = inicio->sgte;
    }
    fprintf(f,"</table>");
    fprintf(f,"</body>");
    fprintf(f,"</html>");
    fclose(f);
    return;
}

nodo *ObtenerUltimo(nodo * p)
{
    if (p)
    {
        while (p->sgte)
        {
            p = p->sgte;
        }
    }
    return p;

}

void InsertarSiguiente (nodo  *&n, poliza x)
{
    nodo *nuevo= new nodo();
    nuevo->info=x;
    if (n)
    {
        nuevo->sgte= n->sgte;
        n->sgte = nuevo;
    }
    else
    {
        nuevo->sgte= n;
        n= nuevo;
    }
    return;
}

void InsertarAlFinal(nodo *&lista, poliza x)
{
    nodo *aux;
    aux=ObtenerUltimo(lista);
    if (aux)
        InsertarSiguiente(aux,x);
    else
        InsertarSiguiente(lista,x);
    return;
}

void desactivarPoliza(int num,nodo *&inicio)
{
    poliza x;
    nodo *aux=inicio;

    while(aux && aux->info.NroPoliza!=num)
        aux=aux->sgte;
    if (aux)
    {
        aux->info.Activa=0;
    }
    else
        cout << "No se encontro el numero" <<endl;
}

void buscarPolizaPorNro(nodo *inicio,int numero)
{
    while(inicio && inicio->info.NroPoliza!=numero)
        inicio=inicio->sgte;
    if (inicio)
    {
        cout << "Numero de poliza: " << inicio->info.NroPoliza <<endl;
        cout << "DNI: " << inicio->info.DNI <<endl;
        cout << "Nombre: " << inicio->info.Nombre <<endl;
        cout << "Apellido: " << inicio->info.Apellido <<endl;
        cout << "Esta la cuota al dia? ";
        if (inicio->info.CuotaAlDia == 1)
            cout << "Si" <<endl;
        else
            cout << "No" <<endl;
        cout << "Patente: " << inicio->info.Patente <<endl;
        cout << "Esta activa la poliza? ";
        if (inicio->info.Activa == 1)
            cout << "Si" <<endl;
        else
            cout << "No" <<endl;
        cout << "Cantidad de incidentes: " << inicio->info.CdadIncidentes <<endl;
    }
    else
        cout << "No se encontro el numero de poliza ingresado" <<endl;
}

void buscarPolizaPorDNI(nodo *inicio,int dni)
{
    int i = 0;
    bool encontrado=false;
    while(inicio)
    {
        if(inicio->info.DNI==dni)
        {
            cout << "----------Poliza del DNI buscado " << i+1 << " ----------" << endl;
            cout << "Numero de poliza: " << inicio->info.NroPoliza <<endl;
            cout << "DNI: " << inicio->info.DNI <<endl;
            cout << "Nombre: " << inicio->info.Nombre <<endl;
            cout << "Apellido: " << inicio->info.Apellido <<endl;
            cout << "¿Esta la cuota al dia? " << inicio->info.CuotaAlDia <<endl;
            cout << "Patente: " << inicio->info.Patente <<endl;
            cout << "¿Esta activa la poliza? " << inicio->info.Activa <<endl;
            cout << "Cantidad de incidentes: " << inicio->info.CdadIncidentes <<endl;
            inicio=inicio->sgte;
            encontrado=true;
            i++;
        }
        else
            inicio=inicio->sgte;
    }
    if (!encontrado)
        cout << "El DNI ingresado no corresponde con ninguna poliza almacenada" << endl;
}

void buscarPoliza(nodo *inicio)
{
    int numero_poliza,dni,eleccion;
    cout << "Buscar poliza por Nro. de Poliza o por DNI?" << endl;
    cout << "1 -> Nro. de Poliza\n2 -> DNI" << endl;
    cin >> eleccion;

    system("cls");
    if (eleccion==1)
    {
        cout << "Ingrese el Nro de Poliza: ";
        cin >> numero_poliza;

        system("cls");
        buscarPolizaPorNro(inicio,numero_poliza);
    }
    else if (eleccion==2)
    {
        cout << "Ingrese el Nro de DNI: ";
        cin >> dni;

        system("cls");
        buscarPolizaPorDNI(inicio,dni);
    }
    else
    {
        cout << "Opcion no disponible";
    }
}

void procesarIncidentes(nodo *&inicio,char RUTAP[],char RUTAINCIDENTE1[] ,char RUTAINCIDENTE2[])
{
    FILE *f,*y;
    incidente x;
    nodo *aux=inicio;
    int opcion;
    y=fopen(RUTAP,"ab");

    cout << "Elija el lote de incidente de prueba a utilizar: \n-Lote de Incidente 1\n-Lote de Incidente 2 " << endl;
    cin >> opcion;

    if(opcion == 1 )
    {
        if(f=fopen(RUTAINCIDENTE1,"rb"))
        {
            while(fread(&x,sizeof(incidente),1,f))
            {
                aux = inicio;
                while(aux && (aux->info.NroPoliza!=x.NroPoliza))
                    aux=aux->sgte;
                if (aux) 
                {
                    aux->info.CdadIncidentes++;
                    fwrite(&x, sizeof(incidente),1,y);
                }
                /*else
                    cout << "El incidente no coincide con ninguna poliza registrada" <<endl; ESTO ES PARA SABER SI EL LOTE DE INCIDENTE PROCESA BIEN*/ 
            }

        fclose(f);
        system("cls");
        cout << "--------Lote 1 Procesado Correctamente--------" << endl;
        }
        else
            cout << "No se pudo acceder al archivo" << endl;
    }
    
    if(opcion == 2 )
    {
        if(f=fopen(RUTAINCIDENTE2,"rb"))
        {
            while(fread(&x,sizeof(incidente),1,f))
            {
                aux = inicio;
                while(aux && (aux->info.NroPoliza!=x.NroPoliza))
                    aux=aux->sgte;
                if (aux)
                {
                    aux->info.CdadIncidentes++;
                    fwrite(&x, sizeof(incidente),1,y);
                }
                /*else
                    cout << "El incidente no coincide con ninguna poliza registrada" <<endl; ESTO ES PARA SABER SI EL LOTE DE INCIDENTE PROCESA BIEN*/ 
            }
        fclose(f);
        system("cls");
        cout << "--------Lote 2 Procesado Correctamente--------" << endl;
        }
        else
            cout << "No se pudo acceder al archivo" << endl;
    }
    fclose(y);
    if( opcion !=1 and opcion !=2 )
        cout << "Opcion Incorrecta" << endl;

}

nodo* BuscarNodoAnterior(nodo * raiz, nodo * anterior_a)
{
    nodo *p =NULL;
    if (raiz != anterior_a)
    {
        p = raiz;
        while (p && (p->sgte!= anterior_a) )
        {
            p = p->sgte;
        }
        return p;
    }
    else
        return NULL;
}

void insertarPrimero(nodo *&lista,poliza num)
{
    nodo *nuevo=new nodo();
    nodo *aux=lista;
    nuevo->info=num;
    lista=nuevo;
    nuevo->sgte=aux;
}

void insertarOrdenado(nodo *&lista,poliza x)
{
    nodo *aux=lista;
    nodo *anterior=NULL;

    while (aux && aux->info.CdadIncidentes > x.CdadIncidentes)
    {
        aux=aux->sgte;
    }
    if (aux && aux!=lista)
    {
        anterior=BuscarNodoAnterior(lista,aux);
        InsertarSiguiente(anterior,x);
    }
    else
    {
        if (aux==NULL)
        {
            InsertarAlFinal(lista,x);
        }
        else if (aux==lista)
        {
            insertarPrimero(lista,x);
        }
    }
}

nodo *ObtenerSiguiente(nodo  *n)
{
    if (n)
        return n->sgte;
    else
        return NULL;
}


void MostrarLista(nodo *milista)
{
    int i=1;
    while (milista)
    {
        if (milista->info.Activa!=0)
        {
            cout << "--------------------------Miembro " << i << "-----------------------------------" <<endl;
            cout << "Cantidad de incidentes: " <<milista->info.CdadIncidentes << "   " <<endl;
            cout << "DNI: " << milista->info.DNI << "   " <<endl;
            cout << "Nombre: " <<milista->info.Nombre << "   " <<endl;
            cout << "Apellido: " <<milista->info.Apellido << "   " <<endl;
            cout << "Esta la cuota al dia?: ";
            if (milista->info.CuotaAlDia == 1)
                cout << "Si" <<endl;
            else
                cout << "No" <<endl;
            cout << "Patente: " <<milista->info.Patente << "   " <<endl;
            cout << "Esta activa la poliza?: ";
            if (milista->info.Activa == 1)
                cout << "Si" <<endl;
            else
                cout << "No" <<endl;
            cout << "Numero de poliza: " <<milista->info.NroPoliza << "   " <<endl;
            i++;
        }
        milista = ObtenerSiguiente(milista);
    }
}

void cargarPoliza(nodo *&inicio)
{
    poliza x;

    cout << "Ingrese el numero de la poliza: ";
    cin >> x.NroPoliza;
    system("cls");

    cout << "Ingrese el DNI: ";
    cin >> x.DNI;
    system("cls");

    cout << "Ingrese el Nombre: ";
    fflush(stdin);
    cin.getline(x.Nombre,30);
    system("cls");

    cout << "Ingrese el Apellido: ";
    fflush(stdin);
    cin.getline(x.Apellido,30);
    system("cls");

    cout << "La cuota esta al dia? (0 no, 1 si) :";
    cin >> x.CuotaAlDia;
    system("cls");

    cout << "Ingrese la patente: ";
    fflush(stdin);
    cin.getline(x.Patente,10);
    system("cls");

    cout << "Esta activa la poliza? (0 no, 1 si) :";
    cin >> x.Activa;
    system("cls");

    cout << "Ingrese la cantidad de incidentes: ";
    cin >> x.CdadIncidentes;
    system("cls");

    insertarOrdenado(inicio, x);
}

void mostrarMenu()
{
    cout << "--------------------------Menu de opciones----------------------------" << endl;
    cout << "A - Cargar una nueva poliza " << endl;
    cout << "B - Desactivar una poliza existente" << endl;
    cout << "C - Buscar una poliza por Nro. de Poliza o por DNI" << endl;
    cout << "D - Listar todas las polizas activas ordenadas por saldo descendente" << endl;
    cout << "E - Procesar un lote de incidentes" << endl;
    cout << "F - Mostrar todas las polizas que no tengan la cuota al dia en HTML y CSV" << endl;
    cout << "G - Cargar datos de prueba (ATENCION, SOBREESCRIBIRA LOS DATOS PREEXISTENTES DE ASEGURADOS.BAK)" <<endl;
    cout << "ESC - Salir del programa" <<endl;
}

void cargarDatosPrueba(char ruta[],char RUTAINCIDENTE1[] ,char RUTAINCIDENTE2[])
{
    FILE *f;
    poliza x;
    incidente y;

    if (f=fopen(ruta,"wb")) // crea datos de prueba para polizas
    {
        x.NroPoliza=123;
        x.DNI=44670846;
        strcpy(x.Nombre,"Franco");
        strcpy(x.Apellido,"Pesce");
        x.CuotaAlDia=1;
        strcpy(x.Patente,"oxb414");
        x.Activa=1;
        x.CdadIncidentes=3;
        fwrite(&x,sizeof(poliza),1,f);

        x.NroPoliza=456;
        x.DNI=42721841;
        strcpy(x.Nombre,"Ignacio");
        strcpy(x.Apellido,"Giussani");
        x.CuotaAlDia=0;
        strcpy(x.Patente,"ab 123 po");
        x.Activa=1;
        x.CdadIncidentes=10;
        fwrite(&x,sizeof(poliza),1,f);

        x.NroPoliza=159;
        x.DNI=42721841;
        strcpy(x.Nombre,"Pedro");
        strcpy(x.Apellido,"Sanchez");
        x.CuotaAlDia=0;
        strcpy(x.Patente,"xw 766 pq");
        x.Activa=1;
        x.CdadIncidentes=1999;
        fwrite(&x,sizeof(poliza),1,f);

        x.NroPoliza=789;
        x.DNI=42999432;
        strcpy(x.Nombre,"Joan");
        strcpy(x.Apellido,"Cannossa");
        x.CuotaAlDia=1;
        strcpy(x.Patente,"abc123");
        x.Activa=0;
        x.CdadIncidentes=0;
        fwrite(&x,sizeof(poliza),1,f);

        x.NroPoliza=741;
        x.DNI=76542374;
        strcpy(x.Nombre,"Juan");
        strcpy(x.Apellido,"Perez");
        x.CuotaAlDia=0;
        strcpy(x.Patente,"kj 612 nm");
        x.Activa=0;
        x.CdadIncidentes=324;
        fwrite(&x,sizeof(poliza),1,f);

        fclose(f);
    }
    else
        cout << "Error al abrir el archivo" <<endl;
    
    if (f=fopen(RUTAINCIDENTE1,"wb")) // crea lote de prueba para incidentes
    {
        y.CodigoIncidente = 666;
        strcpy(y.FechaHora,"2021102518:30");
        y.DNIAsegurado = 76542374;
        y.DNIOtroConductor = 44891550;
        y.NroPoliza = 741;
        strcpy(y.Calle, "Carlos Morganti");
        y.Altura = 9830;
        fwrite(&y,sizeof(incidente),1,f);

        y.CodigoIncidente = 9384;
        strcpy(y.FechaHora,"2021102512:30");
        y.DNIAsegurado = 42999432;
        y.DNIOtroConductor = 48393092;
        y.NroPoliza = 789;
        strcpy(y.Calle, "Pelegrini Sanchez");
        y.Altura = 1230;
        fwrite(&y,sizeof(incidente),1,f);

        y.CodigoIncidente = 669;
        strcpy(y.FechaHora,"2021102518:30");
        y.DNIAsegurado = 34223514;
        y.DNIOtroConductor = 34555212;
        y.NroPoliza = 345;
        strcpy(y.Calle,"Gualeguaychu");
        y.Altura = 623;
        fwrite(&y,sizeof(incidente),1,f);

        y.CodigoIncidente = 723;
        strcpy(y.FechaHora,"2021162411:30");
        y.DNIAsegurado = 44246514;
        y.DNIOtroConductor = 44556212;
        y.NroPoliza = 932;
        strcpy(y.Calle,"Segurola");
        y.Altura = 3900;
        fwrite(&y,sizeof(incidente),1,f);

        fclose(f);
    }
    else
        cout << "Error al abrir el archivo" <<endl;

    if (f=fopen(RUTAINCIDENTE2,"wb")) // crea lote de prueba para incidentes
    {
        y.CodigoIncidente = 333;
        strcpy(y.FechaHora,"2000010100:00");
        y.DNIAsegurado = 44670846;
        y.DNIOtroConductor = 22099523;
        y.NroPoliza = 123;
        strcpy(y.Calle,"Avenida maipu");
        y.Altura = 2300;
        fwrite(&y,sizeof(incidente),1,f);

        y.CodigoIncidente = 752;
        strcpy(y.FechaHora,"1492052516:42");
        y.DNIAsegurado = 42999432;
        y.DNIOtroConductor = 22992374;
        y.NroPoliza = 789 ;
        strcpy(y.Calle,"Medrano");
        y.Altura = 554;
        fwrite(&y,sizeof(incidente),1,f);

        y.CodigoIncidente = 569;
        strcpy(y.FechaHora,"1993031215:23");
        y.DNIAsegurado = 48735641;
        y.DNIOtroConductor = 23697214;
        y.NroPoliza = 964;
        strcpy(y.Calle,"Av Rivadavia");
        y.Altura = 1910;
        fwrite(&y,sizeof(incidente),1,f);

        y.CodigoIncidente = 968;
        strcpy(y.FechaHora,"1964021218:20");
        y.DNIAsegurado = 16987541;
        y.DNIOtroConductor = 16853417;
        y.NroPoliza = 689 ;
        strcpy(y.Calle,"Av Alberdi");
        y.Altura = 1152;
        fwrite(&y,sizeof(incidente),1,f);

        fclose(f);
    }
    else
        cout << "Error al abrir el archivo" <<endl;
}

void levantarPolizas(nodo *&inicio,char ruta[],bool &correcto)
{
    FILE *f;
    poliza x;

    if (f=fopen(ruta,"rb"))
    {
        while(fread(&x,sizeof(poliza),1,f))
        {
            insertarOrdenado(inicio,x);
        }
    }
    fclose(f);
    correcto=1;
}

int main()
{
    system("cls");
    char RUTA[]="Asegurados.BAK";
    char RUTAP[]="procesados.BAK";
    char RUTAINCIDENTE1[]="LoteIncidente1.BAK";
    char RUTAINCIDENTE2[]="loteIncidente2.BAK";
    char eleccion;
    int opcion;
    bool correcto=0;
    nodo *inicio=NULL;

    cout << "Levantado polizas......." <<endl;
    levantarPolizas(inicio,RUTA,correcto);
    if (correcto==1)
        cout << "POLIZAS LEVANTADAS CORRECTAMENTE" <<endl;
    else
        cout << "ERROR AL LEVANTAR LAS POLIZAS" <<endl;
    do
    {
        mostrarMenu();
        do
        {
            eleccion=getch();
        } 
        while (eleccion!=27 && !(eleccion>='A' && eleccion<='G') && !(eleccion>='a' && eleccion<='g') );

        switch (eleccion)
        {
            case 'a':
            case 'A':
                    system("cls");
                    cargarPoliza(inicio);
                    cout << "-----DATOS CARGADOS CORRECTAMENTE-----" <<endl;
                break;
            case 'b':
            case 'B':
                    system("cls");
                    int desactivarNroPoliza;
                    cout << "Ingrese el numero de poliza a desactivar: ";
                    cin >> desactivarNroPoliza;

                    desactivarPoliza(desactivarNroPoliza,inicio);
                break;
            case 'c':
            case 'C':
                    system("cls");
                    buscarPoliza(inicio);
                break;
            case 'd':
            case 'D':
                    system("cls");
                    MostrarLista(inicio);
                break;
            case 'e':
            case 'E':
                    system("cls");
                    procesarIncidentes(inicio,RUTAP,RUTAINCIDENTE1,RUTAINCIDENTE2);
                break;
            case 'f':
            case 'F':
                    system("cls");
                    escribirReporteHTML(inicio);
                    escribirReporteCSV(inicio);
                    cout << "-----REPORTES ESCRITOS CORRECTAMENTE-----" <<endl;
                break;
            case 'g':
            case 'G':
                    cout << "Estas seguro que quieres cargar los datos?" << endl;
                    cout << "0 -> No\n1 -> Si" << endl;
                    cin >> opcion;
                    if (opcion==1)
                    {
                        system("cls");
                        cargarDatosPrueba(RUTA,RUTAINCIDENTE1,RUTAINCIDENTE2);
                        cout << "-----DATOS CARGADOS CORRECTAMENTE-----" <<endl;
                    }
                break;
        }

        cout << "Presione una tecla para continuar" << endl;
        getch();
        system("cls");
    } 
    while ((int)eleccion != 27);
    cout << "Finalizando jornada..........." <<endl;
    correcto=0;
    sobreescribir(RUTA,inicio,correcto);
    if (correcto==1)
        cout << "DATOS GUARDADOS CORRECTAMENTE" <<endl;
    else
        cout << "ERROR AL GUARDAR LOS DATOS" <<endl;
}