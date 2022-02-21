/*
	Registrar 
	Logout
	Introduzir dinheiro
	Retirar
	Adicionar Contactos
	Ver Historial de transferencias
	Fazer transferências
*/
/*-------------------------------------------------------------------------------------------------------------------------*/
//LIVRARIAS
#include<stdio.h>
#include<locale.h>
#include<stdbool.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
/*-------------------------------------------------------------------------------------------------------------------------*/
//ESTRUTURAS
typedef struct{
	int iNumConta;
}regContacto;

typedef struct{
	int iContaRemetente;
	int iContaDestino;
	float fMontante;
}regTransacao;

typedef struct{
	int iNumConta;
	char *cNome;
	char *cPassword;
	float fDinheiro;
	int iNumAmigos;
	int iNumTransferencias;
	regContacto *vtrAmigos;
	regTransacao *vtrRegistro;
}regUtilizador;

/*-------------------------------------------------------------------------------------------------------------------------*/
//DECLARAÇÃO DE FUNÇÕES E VARIÁVEIS GLOBAIS
void vMenu();
void vRegistro();
void vRegistrar(char *, char *, int);
void vVerUtilizadores();
void vMenuUtilizador();

void vFazerTransferencia();
void vTransferir(int, int, float);
void vVerTransferencias();
void vDepositarDinheiro();
void vEntrar(float);
void vRetirarDinheiro();
void vRetirar(float);

char *cObterTexto(char *, int);
int iObterNumero(char *);
float fObterNumero(char *);
int iGerarNumeroConta();
void vAdicionarContacto();
bool bContactoExiste(int, regUtilizador *);
void vVerContactos();

bool bLogin();

regUtilizador *vtrBaseUtilizadores;
int iNumeroUtilizadores=0;
int iSessaoNumUtilizador; 
/*-------------------------------------------------------------------------------------------------------------------------*/
int main()
{
	setlocale(LC_ALL, "");
	
	vtrBaseUtilizadores=(regUtilizador *)malloc(sizeof(regUtilizador));
	vRegistrar("Teixa", "Password", 1);
	
	vMenu();
	
	return 0;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vMenu()
{
	bool bSair=false;
	char cOption[2];
	int iOption;
	do
	{
		system("CLS");
		printf("Escolha uma opção:\n");
		printf("1:Registar\n2:Iniciar Sessao\n3:Ver Utilizadores\n4:Sair\n");
		//VALIDA A ENTRADA DO UTILIZADOR
		scanf("%s", cOption);
		iOption=atoi(cOption);
		switch(iOption)
		{
			case 1:
					vRegistro();
				break;
			case 2:{
					if(bLogin()==true)
					vMenuUtilizador();
				}
				break;
			case 3:
					vVerUtilizadores();
				break;
			case 4:
					bSair=true;
				break;
			default: printf("Parametro Invalido\n");
		}
	}while(bSair==false);
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vMenuUtilizador()
{
	bool bSair=false;
	int iOption;
	char cOption[2];
	do
	{
		system("CLS");
		printf("Bem Vindo %s, Dinheiro disponivel: $%f\nEscolha uma opcao:\n", vtrBaseUtilizadores[iSessaoNumUtilizador-1].cNome, vtrBaseUtilizadores[iSessaoNumUtilizador-1].fDinheiro);
		printf("1:Hacer Transferecia\n2:Ver Contactos\n3:Adicionar Contacto\n4:Historial Transferencias\n5:Ingresar Dinheiro\n6:Retirar Dinheiro\n7:Logout\n");
		scanf("%s", cOption);
		iOption=atoi(cOption);
		
		switch(iOption)
		{
			case 1:
				vFazerTransferencia();
				break;
			case 2: 
				vVerContactos();
				break;
			case 3:
				vAdicionarContacto();
				break;
			case 4:
				vVerTransferencias();
				break;
			case 5:
				vDepositarDinheiro();
				break;
			case 6:
				vRetirarDinheiro();
				break;
			case 7:
				bSair=true;
				break;
			default: {
				printf("Parametros invalidos\n");
				system("PAUSE");
			}
		}
	}while(bSair==false);

}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vAdicionarContacto()
{
	int iNumConta=iObterNumero("Insira o número de conta");
	if(iNumConta<=iNumeroUtilizadores && iNumConta!=iSessaoNumUtilizador)
	{
		regUtilizador *vtrUtilizador;
		vtrUtilizador=&vtrBaseUtilizadores[iSessaoNumUtilizador-1];
		if(vtrUtilizador->iNumAmigos==0)
		{
			vtrUtilizador->vtrAmigos=(regContacto *)malloc(sizeof(regContacto));
			vtrUtilizador->vtrAmigos[0].iNumConta=iNumConta;
			vtrUtilizador->iNumAmigos=1;
			printf("Se ha añadido a %s a tu lista de contactos\n", vtrBaseUtilizadores[iNumConta-1].cNome);
		}
		else if(bContactoExiste(iNumConta, vtrUtilizador)==false)
		{
			int iUltimoIndice=vtrUtilizador->iNumAmigos;
			vtrUtilizador->vtrAmigos=(regContacto *)realloc(vtrUtilizador, sizeof(regContacto)*(iUltimoIndice+1));
			vtrUtilizador->vtrAmigos[iUltimoIndice].iNumConta=iNumConta;
			vtrUtilizador->iNumAmigos++;
			printf("Se ha añadido a %s a tu lista de contactos\n", vtrBaseUtilizadores[iNumConta-1].cNome);
		}
		else
			printf("El usuario %s ya es tu contacto\n", vtrBaseUtilizadores[iNumConta-1].cNome);
	}
	else
		printf("Conta Invalida\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vVerContactos()
{
	int i;
	int iNumConta;
	regUtilizador *vtrUtilizador;
	vtrUtilizador=&vtrBaseUtilizadores[iSessaoNumUtilizador-1];
	if(vtrUtilizador->iNumAmigos>0)
	{
		for(i=0; i<vtrUtilizador->iNumAmigos; i++)
		{
			iNumConta=vtrUtilizador->vtrAmigos[i].iNumConta;
			printf("Nome: %s \t Número de conta: %d\n", vtrBaseUtilizadores[iNumConta-1].cNome, iNumConta);
		}
	}
	else
		printf("Aun no tienes ningun contacto :(\n");
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
bool bContactoExiste(int iNumConta, regUtilizador *vtrUtilizador)
{
	int i;
	for(i=0; i<vtrUtilizador->iNumAmigos; i++)
	{
		if(vtrUtilizador->vtrAmigos[i].iNumConta==iNumConta)
		{
			return true;
		}
	}
	return false;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vDepositarDinheiro()
{
	float fQuantidade=fObterNumero("Ingrese la cantidad que desea añadir a su conta");
	vEntrar(fQuantidade);
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vEntrar(float fQuantidade)
{
	vTransferir(iSessaoNumUtilizador, iSessaoNumUtilizador, fQuantidade);
	vtrBaseUtilizadores[iSessaoNumUtilizador-1].fDinheiro+=fQuantidade;
	printf("Se ha ingresado $%f a su conta\n", fQuantidade);
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRetirarDinheiro()
{
	float fQuantidade=fObterNumero("Ingrese la cantidad que desea retirar de su cuenta");
	if(fQuantidade<=vtrBaseUtilizadores[iSessaoNumUtilizador-1].fDinheiro)
	{
		vRetirar(fQuantidade);
	}
	else
		printf("No cuenta con los fondos para hacer ese retiro\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRetirar(float fQuantidade)
{
	vTransferir(iSessaoNumUtilizador, iSessaoNumUtilizador, fQuantidade*-1);
	vtrBaseUtilizadores[iSessaoNumUtilizador-1].fDinheiro-=fQuantidade;
	printf("Se ha retiraddo $%f de su cuenta\n", fQuantidade);
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vFazerTransferencia()
{
	regTransacao vtrNuevaTransaccion;
	int iContaDestino=iObterNumero("Insira o número de conta de destino");
	int iContaRemetente=iSessaoNumUtilizador;
	float fMontante=fObterNumero("Insira o valor a ser transferido");
	if(iContaDestino<=iNumeroUtilizadores)
	{
		if(fMontante<=vtrBaseUtilizadores[iSessaoNumUtilizador-1].fDinheiro)
		{
			vTransferir(iContaDestino, iContaRemetente, fMontante);
		}
		else
			printf("Não há fundos suficientes para realizar esta transação\n");
	}
	else
		printf("Conta invalida\n");
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vTransferir(int iDestino, int iRemetente, float fQuantidade)
{	
	bool bDest;
	regUtilizador *vtrUtilizador, *vtrDestino;
	vtrUtilizador=&vtrBaseUtilizadores[iSessaoNumUtilizador-1];
	bDest=(iDestino<=iNumeroUtilizadores && iDestino!=iRemetente)?true:false;
	if(bDest)
	{
		regTransacao vtrTransacao;
		vtrTransacao.iContaDestino=iDestino;
		vtrTransacao.iContaRemetente=iRemetente;
		vtrTransacao.fMontante=fQuantidade;
		
		vtrDestino=&vtrBaseUtilizadores[iDestino-1];
		
		vtrUtilizador->fDinheiro-=fQuantidade;
		vtrDestino->fDinheiro+=fQuantidade;
		
		vtrUtilizador->vtrRegistro=(vtrUtilizador->iNumTransferencias==0)?(regTransacao *)malloc(sizeof(regTransacao)):vtrUtilizador->vtrRegistro;
		vtrDestino->vtrRegistro=(vtrDestino->iNumTransferencias==0)?(regTransacao *)malloc(sizeof(regTransacao)):vtrDestino->vtrRegistro;
		
		vtrUtilizador->iNumTransferencias++;
		vtrDestino->iNumTransferencias++;
		
		vtrUtilizador->vtrRegistro=(regTransacao *)realloc(vtrUtilizador->vtrRegistro, sizeof(regTransacao)*(vtrUtilizador->iNumTransferencias));
		vtrUtilizador->vtrRegistro[vtrUtilizador->iNumTransferencias-1]=vtrTransacao;
		
		vtrDestino->vtrRegistro=(regTransacao *)realloc(vtrDestino->vtrRegistro, sizeof(regTransacao)*(vtrDestino->iNumTransferencias));
		vtrDestino->vtrRegistro[vtrUtilizador->iNumTransferencias-1]=vtrTransacao;
		
	}
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vVerTransferencias()
{
	int i, iIndiceDestino, iIndiceRemitente;
	regUtilizador *vtrUtilizador;
	vtrUtilizador=&vtrBaseUtilizadores[iSessaoNumUtilizador-1];
	if(vtrUtilizador->iNumTransferencias>0)
	{
		float fQuantidade;
		for(i=0; i<vtrUtilizador->iNumTransferencias; i++)
		{
			iIndiceDestino=vtrUtilizador->vtrRegistro[i].iContaDestino-1;
			iIndiceRemitente=vtrUtilizador->vtrRegistro[i].iContaRemetente-1;
			fQuantidade=vtrUtilizador->vtrRegistro[i].fMontante;
			printf("De %s para %s: $%f\n", vtrBaseUtilizadores[iIndiceRemitente].cNome, vtrBaseUtilizadores[iIndiceDestino].cNome, fQuantidade);
		}
	}
	else
		printf("Você ainda não fez nenhuma transferência\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
bool bLogin()
{
	bool logged=false;
	int iNumConta=iObterNumero("Introduza o número da sua conta");
	if(iNumConta<=iNumeroUtilizadores)
	{
		char *cPassword=cObterTexto("Introduza a password", 0);
		logged=(strcmp(cPassword, vtrBaseUtilizadores[iNumConta-1].cPassword)==0)?true:false;
		if(logged==true)
		{
			printf("A password esta incorreta\n");
			iSessaoNumUtilizador=iNumConta;
		}
		else
			printf("A password esta incorreta\n");
	}
	else
		printf("Numero da conta inexistente\n");
		
	system("PAUSE");
	return logged;
	
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRegistro(){
		char *cNome=cObterTexto("Introduza o nome", 0);
		char *cPassword=cObterTexto("Introduza a password (Minimo 8 Caracteres)", 8);
		int iNumeroConta=iGerarNumeroConta();
		
		vRegistrar(cNome, cPassword, iNumeroConta);
		
		printf("O seu numero de conta é: %d\n", iNumeroConta);
		system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vRegistrar(char *cNome, char *cPassword, int iNumConta)
{
	
	vtrBaseUtilizadores[iNumConta-1].cNome=(char *)malloc(sizeof(char));
	vtrBaseUtilizadores[iNumConta-1].cPassword=(char *)malloc(sizeof(char));
	
	regUtilizador *vtrUtilizador;
	vtrUtilizador=&vtrBaseUtilizadores[iNumConta-1];
	
	strcpy(vtrUtilizador->cNome, cNome);
	strcpy(vtrUtilizador->cPassword, cPassword);
	
	vtrUtilizador->iNumConta=iNumConta;
	vtrUtilizador->fDinheiro=0;
	vtrUtilizador->iNumAmigos=0;
	vtrUtilizador->iNumTransferencias=0;
	iNumeroUtilizadores++;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
void vVerUtilizadores()
{
	int i;
	
	if(iNumeroUtilizadores>0)
	{
		for(i=0; i<iNumeroUtilizadores; i++)
		{
			printf("Nome: %s \t Número de Conta: %d\n", vtrBaseUtilizadores[i].cNome, vtrBaseUtilizadores[i].iNumConta);
		}	
	}
	else
		printf("Ainda não há utilizadores registrados\n");
		
	system("PAUSE");
}
/*-------------------------------------------------------------------------------------------------------------------------*/
char *cObterTexto(char *cPeticion, int iLongitud)
{
	char *cTexto;
	bool bValido=false;
	cTexto=(char *)malloc(sizeof(char));
	do
	{
		fflush(stdin);
		system("CLS");
		printf("%s: ", cPeticion);
		scanf("%[^\n]", cTexto);
		
		if(strcmp(cTexto, ""))
		{
			bValido=(iLongitud<=0)?true:false;
			if(bValido==false)
			{
				bValido=(strlen(cTexto)>=iLongitud)?true:false;
			}
		}
	}while(!bValido);
	return cTexto;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
int iObterNumero(char *peticao)
{
	char *cNumero;
	int iNumero;
	cNumero=(char *)malloc(sizeof(char));
	do
	{
		fflush(stdin);
		printf("%s: ", peticao);
		scanf("%s", cNumero);
		iNumero=atoi(cNumero);
	}while(iNumero<=0);
	return iNumero;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
float fObterNumero(char *peticao)
{
	char *cNumero;
	float fNumero;
	cNumero=(char *)malloc(sizeof(char));	
	do
	{
		fflush(stdin);
		printf("%s: ", peticao);	
		scanf("%s", cNumero);
		fNumero=atof(cNumero);
	}while(fNumero<=0);
	return fNumero;
}
/*-------------------------------------------------------------------------------------------------------------------------*/
int iGerarNumeroConta()
{
	if(iNumeroUtilizadores==0)
	{
		vtrBaseUtilizadores=(regUtilizador *)malloc(sizeof(regUtilizador));
		return iNumeroUtilizadores+1;
	}
	else{
		vtrBaseUtilizadores=(regUtilizador *)realloc(vtrBaseUtilizadores, sizeof(regUtilizador)*(iNumeroUtilizadores+1));
		return iNumeroUtilizadores+1;
	}
}
