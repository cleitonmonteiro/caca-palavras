#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>
#define MAX 100

FILE *f_arquivo_palavras;
FILE *f_arquivo_cadastrados;
FILE *f_arquivo_cont_cadastrados;

char c_palavras[MAX][MAX];
char c_principal[22][33];
int i_Coordenadas[MAX][MAX];
int i_linhas_palavras_selecionadas[MAX];
int i_coordenadas_palavras_certas[2000];
int i_palavras_corretas[MAX];

struct player
{   
    char c_username[17]; 
    int i_vitorias_n1;
    int i_vitorias_n2;
    int i_vitorias_n3;
    int i_numero_jogos; 
};

struct player usuario;
struct player *s_rankeados_aux;
int i_cont_cadastrados;
int i_cont_coord_certas;


int contDiagonal;
int x=0, y=0, i=0, j=0, o=0, l=0;
int n=0, m=0;
int xp=0;
int i_treta=0;
int cont, opp, ops;
int s_i;
int i_numero_caractere;
int contCoordenadas;
int i_login_cadastro;

int i_condicao_reticao;
int i_condicao_linhas;
int i_condicao_acertou;
int i_condicao_continuar;
int i_condicao_continuar_s;
int i_condicao_sobrepor;
int i_condicao_colorir;
int i_condicao_sair;
int i_condicao_atualizar_dados;
int i_condicao_login_aceito;
int i_condicao_c ; 
int i_condicao_sair_tema;
int i_condicao_colorir_pal_lado;

int posicaoLinha;
int tamanhoPalavra;
int quantidadePalavras;
int numLinha;
int tentativas;
int posicaoPalavra;
int acertos;
int linha;
int coluna;

char letraPosicao;
char c_novo_jogo;
char c_nivel_escolhido;
char c_nivel_rank;
char c_resposta_jogar;
char c_opcao_menu;
char c_opcao_resolver[6];
char c_opcao_tema;
/********************************************************************************************************************************/
/**                                                         GERAIS                                                              */
/********************************************************************************************************************************/
void preencherMatriz(); //preenche matriz apenas com pontos '.'
void retirarPalavrasArquivo(); //retirar palavras do arquivo e salva em uma matriz(c_palavras)
void selecionarPalavrasJogo(); //dentre as palavras da matriz 'c_palavras' seleciona de forma 
void v_preencherMatrizPalavras();
void nome(); //titulo
void escrever(); //escrever matriz principal
void v_painelDeTentativasAcertos(); //quantidade de palavaras encontradas(acertos) e tentativas restantes
void v_mensagemDeErro();
void v_login(); // caso o usuario ja esteja cadastrado
void v_cadastrar(); // caso o usuario não esteja cadastrado
int i_consulta_nome(); // comparação de strings (username)
void v_jogar();
void v_novoJogo();
void sair ();  //Esta função fará com que o programa seja fechado
void v_coordenadas_certas();
void v_rank_nivel(); // ordena os usuarios de acordo com o nivel escolhido
void v_imprime_rank(); 
void v_selecao_temas(); 
void v_pause(); 
void v_escrever_menu_principal();
void v_escrever_menu_login();
void v_escrever_menu_cadastrar();
void v_escrever_menu_niveis();
void v_escrever_menu_temas();
void v_escrever_menu_raking();

;
/********************************************************************************************************************************/
/**                                                   MODO DE ESCRITA                                                           */
/********************************************************************************************************************************/
void horizontalDireitaEsquerda();
void horizontalEsquerdaDireita();
void verticalSuperiorInferior();
void verticalInferiorSuperior();
void diagonalPrincipalSuperiorInferior();
void diagonalPrincipalInferiorSuperior();
void diagonalSecundariaSuperiorInferior();
void diagonalSecundariaInferiorSuperior();
void coordenadasPalavras();
void coordenadasPalavrasInferiorSuperior();

/********************************************************************************************************************************/
/**                                                     MAIN                                                                    */
/********************************************************************************************************************************/
int main()
{
    srand(time(NULL));
    setlocale(LC_ALL,"");
    initscr();   //Esta função  inicializa a ncurses
    start_color(); //Esta função torna possível o uso das cores

    //Par de cores (Texto, Fundo)    
    init_pair(1,COLOR_WHITE,COLOR_BLUE); 
    init_pair(2,COLOR_BLUE,COLOR_WHITE); 
    init_pair(3,COLOR_RED,COLOR_WHITE);  
    init_pair(4,COLOR_WHITE,COLOR_BLACK); 
    init_pair(5,COLOR_GREEN,COLOR_BLACK); 
    init_pair(6,COLOR_RED,COLOR_BLACK);  
    init_pair(7,COLOR_YELLOW,COLOR_BLACK); 
    init_pair(8,COLOR_CYAN,COLOR_BLACK); 
    init_pair(10,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(11,COLOR_BLACK,COLOR_GREEN); 

    //init_pair(2,COLOR_BLUE,COLOR_WHITE);  
    
    

    /* COLOR_BLACK - COLOR_RED - COLOR_GREEN - COLOR_YELLOW - COLOR_BLUE - COLOR_MAGENTA - COLOR_CYAN - COLOR_WHITE */

    bkgd(COLOR_PAIR(5));/*Aqui nós definiremos a cor do fundo do nosso programa e a cor dos textos.*/

    acertos = 0; //Padrão
    contCoordenadas = 2; //Padrão
    i_cont_coord_certas = 0; //Padrão

    clear(); //por causa do terminal
    i_condicao_colorir_pal_lado = 0; 

    do
    {
        v_escrever_menu_principal();
        c_opcao_menu = getch();
        //fflush(stdin);
        __fpurge(stdin);

        i_condicao_sair = 0;        
        switch(c_opcao_menu)
        {
            case '1':
                // ler quantidade de usuarios cadastrados e salva em uma variavel   
                if((f_arquivo_cont_cadastrados = fopen("main_dados.txt","rb")) == NULL) {
        			printw("Erro na abertura do arquivo com a quanidade de usuarios cadastrados.\n");
			        sair();
			    }
			    
			    fread(&i_cont_cadastrados, sizeof(int),1,f_arquivo_cont_cadastrados);			   
			    fclose(f_arquivo_cont_cadastrados);


                // ler os dados dos usuarios cadastrados   
			    f_arquivo_cadastrados =  fopen("arquivo_cadastrados.txt","rb");
			    if(f_arquivo_cadastrados == NULL) {
			        printw("Erro na abertura do arquivo cadastrados!\n");
			        sair();
			    }


                //criando um vetor dinamico para armazenar os dados dos usuarios
                s_rankeados_aux = (struct player*) malloc( sizeof(struct player) * (i_cont_cadastrados));
    			if(s_rankeados_aux == NULL)
    			{
        			printw("Erro na memoria vetor dos dados dos cadastrados.");
        			sair();
    			}    
    			
    			// lendo do arquivo e salvando no vetor criado acima
    			fread(s_rankeados_aux, sizeof(struct player),10,f_arquivo_cadastrados);    			
    			fclose(f_arquivo_cadastrados);                
                v_login();
                v_jogar();
            break;
            case '2':
                // ler quantidade de usuarios cadastrados e salva em uma variavel   
                if((f_arquivo_cont_cadastrados = fopen("main_dados.txt","rb")) == NULL) {
        			printw("Erro na abertura do arquivo com a quanidade de usuarios cadastrados.\n");
			        sair();
			    }
			    
			    fread(&i_cont_cadastrados, sizeof(int),1,f_arquivo_cont_cadastrados);
			    fclose(f_arquivo_cont_cadastrados);

			    i_cont_cadastrados++;// acrescenta o numero de usuarios cadastrados

                // ler os dados dos usuarios cadastrados   
			    f_arquivo_cadastrados =  fopen("arquivo_cadastrados.txt","rb");
			    if(f_arquivo_cadastrados == NULL) {
			        printw("Erro na abertura do arquivo cadastrados!\n");
			        sair();
			    }

                s_rankeados_aux = (struct player*) malloc(sizeof(struct player) * i_cont_cadastrados);
    			if(s_rankeados_aux == NULL)
    			{
        			printw("Erro na memoria vetor dos dados dos cadastrados.");
        			sair();
    			}    

    			fread(s_rankeados_aux, sizeof(struct player),i_cont_cadastrados,f_arquivo_cadastrados);
    			fclose(f_arquivo_cadastrados);
                
                v_cadastrar();                
                //v_jogar();
                strcpy(s_rankeados_aux[i_cont_cadastrados - 1].c_username, usuario.c_username);                
    	        s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n1 = 0;	            
	            s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n2 = 0;	            
                s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n3 = 0;
                s_rankeados_aux[i_cont_cadastrados - 1].i_numero_jogos = 0;

                if((f_arquivo_cadastrados =  fopen("arquivo_cadastrados.txt","wb")) == NULL) {
			        printw("Erro na abertura do arquivo cadastrados!\n");
			        sair();
			    }
			    fwrite(s_rankeados_aux, sizeof(struct player), i_cont_cadastrados, f_arquivo_cadastrados);    
			    fclose(f_arquivo_cadastrados); 	

				if((f_arquivo_cont_cadastrados = fopen("main_dados.txt","wb")) == NULL) {
			        printw("Erro na abertura do arquivo com a quanidade de usuarios cadastrados.\n");
			        sair();
			    }
			    fwrite(&i_cont_cadastrados, sizeof(int),1,f_arquivo_cont_cadastrados);
			    fclose(f_arquivo_cont_cadastrados);

                do
			    {
			        mvprintw(6,5,"Usuário cadastrado com sucesso.");
			        mvprintw(7,5,"Dejesa retornar ao menu principal ? [S][N]: ");
			        c_resposta_jogar = getch();		
			        //__fpurge(stdin);
			        fflush(stdin);
			        if(c_resposta_jogar == 's' || c_resposta_jogar == 'S')
			        {
			            clear();
			            main();
			        }
			        else
			        {
			            if((c_resposta_jogar == 'n') ||(c_resposta_jogar =='N'))
			            {
			                clear();
			                sair();
			            }            
			        }
			    }while(1); 
            break;
            
            case '3':
            	if((f_arquivo_cont_cadastrados = fopen("main_dados.txt","rb")) == NULL) {
        			printw("Erro na abertura do arquivo com a quanidade de usuarios cadastrados.\n");
			        sair();
			    }
			    
			    fread(&i_cont_cadastrados, sizeof(int),1,f_arquivo_cont_cadastrados);
			    fclose(f_arquivo_cont_cadastrados);

            	// ler quantidade de usuarios cadastrados e salva em uma variavel   
			    f_arquivo_cadastrados =  fopen("arquivo_cadastrados.txt","rb");
			    if(f_arquivo_cadastrados == NULL) {
			        printw("Erro na abertura do arquivo cadastrados!\n");
			        sair();
			    }
			            	
            	s_rankeados_aux = (struct player*) malloc( sizeof(struct player) * (i_cont_cadastrados));
    			if(s_rankeados_aux == NULL)
    			{
        			printw("Erro na memoria vetor dos dados dos cadastrados.");
        			sair();
    			}    
    			// lendo do arquivo e salvando no vetor criado acima
    			fread(s_rankeados_aux, sizeof(struct player),i_cont_cadastrados,f_arquivo_cadastrados);
    			fclose(f_arquivo_cadastrados);
                
                do
                {
                    v_escrever_menu_raking();
                    c_nivel_rank = getch();
                    //fflush(stdin);
                    __fpurge(stdin);
                    switch (c_nivel_rank)
                    {
                        case '1':
                            i_condicao_c = 0;
                        break;

                        case '2':
                            i_condicao_c = 0;
                        break;
                        
                        case '3':
                            i_condicao_c = 0;
                        break;
                        default:
                            //printw("\nEntre com uma opção  válida.\n");
                            //v_pause();
                            clear();
                            v_escrever_menu_principal();
                			mvprintw(5,67,"%c",c_opcao_menu);
                            i_condicao_c = 1;
                        break;
                    } 

                }while(i_condicao_c);
				
                              
                v_rank_nivel();  //ordena de acordo com o nivel escolhido
                v_imprime_rank();
                do
			    {
			        mvprintw(1,2,"Dejesa retornar ao menu principal ? [S][N]: ");
			        c_resposta_jogar = getch();		
			        __fpurge(stdin);
			        //fflush(stdin);
			        if(c_resposta_jogar == 's' || c_resposta_jogar == 'S')
			        {
			            clear();
			            main();
			        }
			        else
			        {
			            if((c_resposta_jogar == 'n') ||(c_resposta_jogar =='N'))
			            {
			                clear();
			                sair();
			            }            
			        }
			    }while(1); 
            break;
            default:
                //printw("\nEntre com uma opção  válida.\n");
                //v_pause();
                clear();
                i_condicao_sair = 1; 
            break;
        }
    }while(i_condicao_sair);
    

    return 0;
}
void v_escrever_menu_principal()
{
	attron(COLOR_PAIR(11));
    mvprintw(1,66,"   => CAÇA PALAVRAS <=   \n");    
    attron(COLOR_PAIR(5)); 
    mvprintw(2,66,"│       Jogar (1)       │\n");
    mvprintw(3,66,"│    Cadastre-se (2)    │\n");
    mvprintw(4,66,"│      Ranking (3)      │\n");
    mvprintw(5,66,"│                       │\n");
    mvprintw(6,66,"└───────────────────────┘\n");
    	
    
    move(5,67);
}

void v_escrever_menu_raking()
{
    attron(COLOR_PAIR(11));
	mvprintw(4,90,"          Nível            ");
	attron(COLOR_PAIR(5)); 
	mvprintw(5,91,"        Fácil (1)        │");
	mvprintw(6,90,"┤        Médio (2)        │");
	mvprintw(7,90,"│       Difícil (3)       │");
	mvprintw(8,90,"│                         │");
    mvprintw(9,90,"└─────────────────────────┘");
	move(8,91);

}

void v_escrever_menu_login()
{
	attron(COLOR_PAIR(11));
	mvprintw(2,40,"          Login            ");
	attron(COLOR_PAIR(5)); 
	mvprintw(3,40,"│");
    mvprintw(4,40,"└─────────────────────────┤");
	move(3,41);
}

void v_escrever_menu_cadastrar()
{
	attron(COLOR_PAIR(11));
	mvprintw(2,40,"    Nome de usuário        ");
	attron(COLOR_PAIR(5)); 
	mvprintw(3,40,"│");
    mvprintw(4,40,"└─────────────────────────┤");
	move(3,41);
}

void v_escrever_menu_niveis()
{
	attron(COLOR_PAIR(11));
	mvprintw(4,40,"          Nível            ");
	attron(COLOR_PAIR(5)); 
	mvprintw(5,40,"│        Fácil (1)        │");
	mvprintw(6,40,"│        Médio (2)        ├");
	mvprintw(7,40,"│       Difícil (3)       │");
	mvprintw(8,40,"│                         │");
    mvprintw(9,40,"└─────────────────────────┘");
	move(8,41);

}

void v_escrever_menu_temas()
{
	attron(COLOR_PAIR(11));
	mvprintw(9,40,"           Tema            ");
	attron(COLOR_PAIR(5)); 
	mvprintw(10,40,"│        Praia (1)        │");
	mvprintw(11,40,"│        Cores (2)        │");
	mvprintw(12,40,"│       Cozinha (3)       │");
	mvprintw(13,40,"│      Adjetivos (4)      │");
	mvprintw(14,40,"│      Mamíferos (5)      │");
	mvprintw(15,40,"│      Profissões (6)     │");
	mvprintw(16,40,"│  Capitais no mundo (7)  │");
	mvprintw(17,40,"│ Sabores de sorvete (8)  │");
	mvprintw(18,40,"│                         │");
    mvprintw(19,40,"└─────────────────────────┘");
	move(18,41);
}

void v_pause()
{
	mvprintw(39,0,"Digite uma tecla para continuar...");
    getch();
    //fflush(stdin);
    __fpurge(stdin);
}

void preencherMatriz()
{
    for(i=0; i <= 21;i++)
    {
        for(j=0;j <= 32;j++)
        {
            c_principal[i][j] = '.';
        }
    }
}

void retirarPalavrasArquivo()
{
    while((i_numero_caractere = fgetc(f_arquivo_palavras)) != EOF)
    {
        if(i_numero_caractere == 10) //10 == numero do ENTER na tabela ASCII
        { 
            c_palavras[y][x] = '\0';
            y++;
            x=0;
            continue;
        }
        c_palavras[y][x] = i_numero_caractere;
        x++;
	}
    fclose(f_arquivo_palavras);
}

void v_preencherMatrizPalavras()
{

    for(cont=0;cont < quantidadePalavras;cont++)
    {
        numLinha = i_linhas_palavras_selecionadas[cont];
        tamanhoPalavra = strlen(c_palavras[numLinha]);

        //OPCÕES DE ESCRITA [H] [V] [D]

        opp = rand()%3;
        switch(opp){
            case 0:
                i_Coordenadas[cont][0] = 'H';
                i_Coordenadas[cont][1] = tamanhoPalavra;
                ops = rand()%2;
                switch(ops){
                    case 0:
                        horizontalDireitaEsquerda();
                    break;
                    case 1:
                        horizontalEsquerdaDireita();
                    break;
                    default:
                        printw("ERRO NA ESCOLHA DO OPÇÃO [H] [S].\n");
                        sair();
                    break;
                }
            break;
            case 1:
                i_Coordenadas[cont][0] = (int) 'V';
                i_Coordenadas[cont][1] = tamanhoPalavra;
                ops = rand()%2;
                switch (ops){
                    case 0:
                        verticalSuperiorInferior();
                    break;
                    case 1:
                        verticalInferiorSuperior();
                    break;
                    default:
                        printw("ERRO NA ESCOLHA DO OPÇÃO [V] [S].\n");
                        sair();
                    break;
                }
            break;
            case 2:
                i_Coordenadas[cont][0] = (int) 'D';
                i_Coordenadas[cont][1] = tamanhoPalavra;
                ops = rand()%4;
                switch (ops){
                    case 0:
                        diagonalPrincipalSuperiorInferior();
                    break;
                    case 1:
                        diagonalPrincipalInferiorSuperior();
                    break;
                    case 2:
                        diagonalSecundariaSuperiorInferior();
                    break;
                    case 3:
                        diagonalSecundariaInferiorSuperior();
                    break;
                    default:
                        printw("ERRO NA ESCOLHA DO OPÇÃO [D] [S].\n");
                        sair();
                    break;
                }
            break;
            default:
                printw("ERRO NA ESCOLHA DO OPÇÃO [H] [V] [D].\n");
            sair();
            break;
        }
    }
}

void selecionarPalavrasJogo()
{
    for(i=0;i < quantidadePalavras;i++)
    {
        do
        {
            posicaoLinha = rand()%y;
            i_condicao_linhas = 0;
            for(j= 0; j<i; j++)
            {
                if(posicaoLinha == i_linhas_palavras_selecionadas[j])
                    i_condicao_linhas = 1;
            }
        }while(i_condicao_linhas);
    
        i_linhas_palavras_selecionadas[i] = posicaoLinha;
    }
}

void nome()
{
    attron(COLOR_PAIR(11));
	mvprintw(1,5,"                                                           >> CAÇA PALAVRAS <<                                                              ");
	attron(COLOR_PAIR(5)); 
	move(3,0);
}

void escrever()
{
    nome(); // titulo
    //numeros colunas da parte superior
    printw("\t  ");
    for(i=0;i<=32;i++)
    {
        if(i<=9)
            printw("%d  ",i);
        else
            printw("%d ",i);
    }
    printw("\t\t ________________\n\t  ");
    for(i=0;i<=32;i++){
            printw("⇓  ",i);
    }

    printw("\t\t/____PALAVRAS____\\\n\n");

    // matriz principal
    for(i=0;i<=21;i++)
    {
        // numeros das linhas esquerda
        printw("  %2d ⇒    ",i);
        for(j=0;j<=32;j++)
        {
            if(c_principal[i][j]== '.')
                c_principal[i][j] = 'A'+ rand()%25;

            // verificar se coordenadas (i,j) pertencem as palavras encontradas pelo usuario
            i_condicao_colorir = 0;
            for(l = 0; l < i_cont_coord_certas; l+=2)
            {
                if((i_coordenadas_palavras_certas[l] == i) && (i_coordenadas_palavras_certas[l + 1] == j))
                    i_condicao_colorir = 1 ;
            }
            // se as coordenadas (i,j) pertencerem as palavras encontradas pelo usuario colorir o valor nas coordenadas
            if(i_condicao_colorir)
                attron(COLOR_PAIR(6)); // vermelho - texto,preto - fundo 
            else
                attron(COLOR_PAIR(5)); // verde - texto,preto - fundo 

            // caracteres da matriz principal
            printw("%c  ",c_principal[i][j]);
        }
        // numeros das linhas direita
        printw("  ⇐ %d\t  ",i);
        
        // palavras escritas no lado
        // verificar se i é indice das palavras encontradas pelo usuario
        i_condicao_colorir = 0;        
        for(n = 0;n < acertos;n++)
        {
            if(i == i_palavras_corretas[n])
                i_condicao_colorir  = 1;
        }
        // se i for  indice das palavras encontradas pelo usuario colorir a palavra
        if(i_condicao_colorir)
            attron(COLOR_PAIR(6)); 
        else
            attron(COLOR_PAIR(4)); 		
		
        if(i_condicao_colorir_pal_lado)
        	attron(COLOR_PAIR(6)); // vermelho - texto,preto - fundo 

        if(i < quantidadePalavras)
            printw("%s\n",c_palavras[i_linhas_palavras_selecionadas[i]]); 
        else
            printw("\n");
        
        attron(COLOR_PAIR(5)); // verde - texto,preto - fundo 
    }
    
    printw("\n\t  ");
    
    for(i=0;i<=32;i++)
        printw("⇑  ",i);
    
    printw("\n\t  ");

    //numeros colunas da parte inferior
    for(i=0;i<=32;i++)
    {
        if(i<=9)
            printw("%d  ",i);
        else
            printw("%d ",i);
    }    
    printw("\n");
}

void v_painelDeTentativasAcertos()
{
    printw("\t\t __________________________________\t\t ___________________________________\n");
    printw("\t\t/________Faltam %2d palavras________\\\t\t/______%d Tentativas restantes.______\\\n\n",quantidadePalavras - acertos,tentativas);
}

void v_mensagemDeErro()
{
    mvprintw(37,0,"Você errou. Ultilize as opções disponíveis e tente novamente.");
    mvprintw(39,0,"Digite qualquer tecla para continuar...");
    getch();
    //__fpurge(stdin);
    fflush(stdin);


    clear();
}

void v_login()
{
	clear();
    v_escrever_menu_principal();
    mvprintw(5,67,"%c",c_opcao_menu);
    v_escrever_menu_login();
    scanw("\n%s",usuario.c_username);
    //fflush(stdin);
    __fpurge(stdin);
    i_condicao_login_aceito = i_consulta_nome();
    while(i_condicao_login_aceito == 777)
    {
        attron(COLOR_PAIR(6));
        mvprintw(5,40,"Username não encontrado.");
        mvprintw(6,40,"Tente novamente!");
        attron(COLOR_PAIR(5));
        v_pause();
        clear();
        v_escrever_menu_principal();
        mvprintw(5,67,"%c",c_opcao_menu);
    	v_escrever_menu_login();        
        scanw("\n%s",usuario.c_username);
        i_condicao_login_aceito = i_consulta_nome();
    }
}

void v_cadastrar()
{
    v_escrever_menu_cadastrar();
    scanw("%s",usuario.c_username);
    //fflush(stdin);
    __fpurge(stdin);

    //verificar nome usuario 
    i_condicao_atualizar_dados = i_consulta_nome();

    while(i_condicao_atualizar_dados != 777)
    {    
        attron(COLOR_PAIR(6));
        mvprintw(5,40,"O nome já foi cadastrado.");
        mvprintw(6,40,"Tente novamente!");
        attron(COLOR_PAIR(5));
        v_pause();
        clear();
        v_escrever_menu_principal();
        mvprintw(5,67,"%c",c_opcao_menu);
        v_escrever_menu_cadastrar();
        scanw("%s",usuario.c_username);
        i_condicao_atualizar_dados = i_consulta_nome();    
    }     
}

int i_consulta_nome()
{
    for(i=0; i < i_cont_cadastrados;i++)
    {
        if(strcmp(s_rankeados_aux[i].c_username, usuario.c_username) == 0)
            return i; // posicao do usuario no vetor      
    }
    return 777;
}

void v_rank_nivel()
{
    int i, j;
    
    char c_aux_nome[17];
    int i_aux_n1;
    int i_aux_n2;
    int i_aux_n3;
    int i_aux_num_jogos;

    if(c_nivel_rank == '1')
    {
        for(i = 0; i < (i_cont_cadastrados - 1);i++)
        {
            for(j = i + 1; j < i_cont_cadastrados;j++)
            {
                if(s_rankeados_aux[i].i_vitorias_n1 < s_rankeados_aux[j].i_vitorias_n1)
                {
                    // username
                    strcpy(c_aux_nome, s_rankeados_aux[i].c_username);
                    strcpy(s_rankeados_aux[i].c_username, s_rankeados_aux[j].c_username);
                    strcpy(s_rankeados_aux[j].c_username, c_aux_nome);
                    
                    // vitorias nivel 1
                    i_aux_n1 = s_rankeados_aux[i].i_vitorias_n1;
                    s_rankeados_aux[i].i_vitorias_n1 = s_rankeados_aux[j].i_vitorias_n1;
                    s_rankeados_aux[j].i_vitorias_n1 = i_aux_n1;

                    // vitorias nivel 2
                    i_aux_n2 = s_rankeados_aux[i].i_vitorias_n2;
                    s_rankeados_aux[i].i_vitorias_n2 = s_rankeados_aux[j].i_vitorias_n2;
                    s_rankeados_aux[j].i_vitorias_n2 = i_aux_n2;

                    // vitorias nivel 3
                    i_aux_n3 = s_rankeados_aux[i].i_vitorias_n3;
                    s_rankeados_aux[i].i_vitorias_n3 = s_rankeados_aux[j].i_vitorias_n3;
                    s_rankeados_aux[j].i_vitorias_n3 = i_aux_n3;

                    //numero de jogos total
                    i_aux_num_jogos = s_rankeados_aux[i].i_numero_jogos;
                    s_rankeados_aux[i].i_numero_jogos = s_rankeados_aux[j].i_numero_jogos;
                    s_rankeados_aux[j].i_numero_jogos = i_aux_num_jogos;
                }
            }
        }
    }


    if(c_nivel_rank == '2')
    {
        for(i = 0; i < (i_cont_cadastrados - 1) ;i++)
        {
            for(j = i + 1; j < i_cont_cadastrados;j++)
            {
                if(s_rankeados_aux[i].i_vitorias_n2 < s_rankeados_aux[j].i_vitorias_n2)
                {
                    // username
                    strcpy(c_aux_nome, s_rankeados_aux[i].c_username);
                    strcpy(s_rankeados_aux[i].c_username, s_rankeados_aux[j].c_username);
                    strcpy(s_rankeados_aux[j].c_username, c_aux_nome);
                    
                    // vitorias nivel 1
                    i_aux_n1 = s_rankeados_aux[i].i_vitorias_n1;
                    s_rankeados_aux[i].i_vitorias_n1 = s_rankeados_aux[j].i_vitorias_n1;
                    s_rankeados_aux[j].i_vitorias_n1 = i_aux_n1;

                    // vitorias nivel 2
                    i_aux_n2 = s_rankeados_aux[i].i_vitorias_n2;
                    s_rankeados_aux[i].i_vitorias_n2 = s_rankeados_aux[j].i_vitorias_n2;
                    s_rankeados_aux[j].i_vitorias_n2 = i_aux_n2;

                    // vitorias nivel 3
                    i_aux_n3 = s_rankeados_aux[i].i_vitorias_n3;
                    s_rankeados_aux[i].i_vitorias_n3 = s_rankeados_aux[j].i_vitorias_n3;
                    s_rankeados_aux[j].i_vitorias_n3 = i_aux_n3;

                    //numero de jogos total
                    i_aux_num_jogos = s_rankeados_aux[i].i_numero_jogos;
                    s_rankeados_aux[i].i_numero_jogos = s_rankeados_aux[j].i_numero_jogos;
                    s_rankeados_aux[j].i_numero_jogos = i_aux_num_jogos;
                }
            }
        }
    }

    if((c_nivel_rank == '3'))
    {
        for(i = 0; i < (i_cont_cadastrados - 1);i++)
        {
            for(j = i + 1; j < i_cont_cadastrados;j++)
            {
                if(s_rankeados_aux[i].i_vitorias_n3 < s_rankeados_aux[j].i_vitorias_n3)
                {
                    // username
                    strcpy(c_aux_nome, s_rankeados_aux[i].c_username);
                    strcpy(s_rankeados_aux[i].c_username, s_rankeados_aux[j].c_username);
                    strcpy(s_rankeados_aux[j].c_username, c_aux_nome);
                    
                    // vitorias nivel 1
                    i_aux_n1 = s_rankeados_aux[i].i_vitorias_n1;
                    s_rankeados_aux[i].i_vitorias_n1 = s_rankeados_aux[j].i_vitorias_n1;
                    s_rankeados_aux[j].i_vitorias_n1 = i_aux_n1;

                    // vitorias nivel 2
                    i_aux_n2 = s_rankeados_aux[i].i_vitorias_n2;
                    s_rankeados_aux[i].i_vitorias_n2 = s_rankeados_aux[j].i_vitorias_n2;
                    s_rankeados_aux[j].i_vitorias_n2 = i_aux_n2;

                    // vitorias nivel 3
                    i_aux_n3 = s_rankeados_aux[i].i_vitorias_n3;
                    s_rankeados_aux[i].i_vitorias_n3 = s_rankeados_aux[j].i_vitorias_n3;
                    s_rankeados_aux[j].i_vitorias_n3 = i_aux_n3;

                    //numero de jogos total
                    i_aux_num_jogos = s_rankeados_aux[i].i_numero_jogos;
                    s_rankeados_aux[i].i_numero_jogos = s_rankeados_aux[j].i_numero_jogos;
                    s_rankeados_aux[j].i_numero_jogos = i_aux_num_jogos;
                }
            }
        }
    }
}

void v_imprime_rank()
{
    int i;
    int fim;
    // por causa da curses (tela) seria no maximo 29
    if(i_cont_cadastrados > 25)
    	fim = 25;
    else
    	fim = i_cont_cadastrados;	

    attron(COLOR_PAIR(11));
    mvprintw(9,69,"  Posição            Nome      Nº vitórias     Nº de jogos( Total ) ");
    attron(COLOR_PAIR(5)); 
    if(c_nivel_rank == '1')
    {
        for(i = 0; i < fim; i++)
            mvprintw(10+i,69,"│%3d º   │%16s\t │    %3d\t │  \t   %3d   \t│", i+1, s_rankeados_aux[i].c_username, s_rankeados_aux[i].i_vitorias_n1, s_rankeados_aux[i].i_numero_jogos);
    }
    
    if(c_nivel_rank == '2')
    
    {
        for(i=0;i < fim; i++)
            mvprintw(10+i,69,"│%3d º   │%16s\t │    %3d\t │  \t   %3d   \t│", i+1, s_rankeados_aux[i].c_username, s_rankeados_aux[i].i_vitorias_n2, s_rankeados_aux[i].i_numero_jogos);
    }
    
    if(c_nivel_rank == '3')
    
    {
        for(i=0;i < fim; i++)
        	mvprintw(10+i,69,"│%3d º   │%16s\t │    %3d\t │  \t   %3d   \t│", i+1, s_rankeados_aux[i].c_username, s_rankeados_aux[i].i_vitorias_n3, s_rankeados_aux[i].i_numero_jogos);
    }
    mvprintw(10+i,69,"└────────┴──────────────────┴───────────────┴──────────────────────┘");
}

void v_selecao_temas()
{
	do
    {
		v_escrever_menu_temas();
		c_opcao_tema = getch();
		//fflush(stdin);
        __fpurge(stdin);

        i_condicao_sair_tema = 0;
        switch(c_opcao_tema)
        {
            case '1':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Praia.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            case '2':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Cores.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            case '3':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Cozinha.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            case '4':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Adjetivos.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            case '5':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Mamíferos.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            case '6':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Profissões.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            case '7':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Capitais_no_Mundo.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            case '8':
                //abrindo arquivo
    			if((f_arquivo_palavras = fopen("Temas_M/Sabores_de_Sorvete.txt","r")) == NULL)
    			{
        			printw("Erro(arquivos palavras).\n");
        			sair();
    			}                        
            break;
            default:
                //printw("\nEntre com uma opção  válida.\n");
                //v_pause();
                clear();
                v_escrever_menu_principal();
                v_escrever_menu_login();
                mvprintw(3,41,"%s",usuario.c_username);
                v_escrever_menu_niveis();
                mvprintw(8,41,"%c",c_nivel_escolhido );
                i_condicao_sair_tema = 1; 
            break;
        }
    }while(i_condicao_sair_tema);
}

void v_jogar()
{
    do
    {
        v_escrever_menu_niveis();
        c_nivel_escolhido = getch();
        //fflush(stdin);
        __fpurge(stdin);
        
        i_condicao_sair = 0;
        switch(c_nivel_escolhido)
        {
            case '1':
                tentativas = 7;    
                quantidadePalavras = 8;                        
            break;
            case '2':
                tentativas = 5;    
                quantidadePalavras = 13;                        
            break;
            case '3':
                tentativas = 3;    
                quantidadePalavras = 19;
            break;
            default:
                //printw("\nEntre com uma opção  válida.\n");
                //v_pause();
                clear();
                v_escrever_menu_principal();
                mvprintw(5,67,"%c",c_opcao_menu);
                v_escrever_menu_login();
                mvprintw(3,41,"%s",usuario.c_username);
                i_condicao_sair = 1; 
            break;
        }
    }while(i_condicao_sair);
	

	v_selecao_temas();    
    clear();
    preencherMatriz(); 
    retirarPalavrasArquivo(); 
    selecionarPalavrasJogo();
    v_preencherMatrizPalavras();


    while(tentativas)
    {
        escrever();
        v_painelDeTentativasAcertos();
        do{
            printw("\t\tPosicão da palavra horizontal[H], vertical[V] ou diagonal[D]: ");
            letraPosicao = getch();
            __fpurge(stdin);
            //fflush(stdin);
            i_condicao_continuar = 1;
            if(letraPosicao == 'h' || letraPosicao == 'H'){
                posicaoPalavra = (int) 'H';
                break;
            }

            if(letraPosicao == 'v' || letraPosicao == 'V'){
                posicaoPalavra = (int) 'V';
                break;
            }

            if(letraPosicao == 'd' || letraPosicao == 'D'){
                posicaoPalavra = (int) 'D';
                break;
            }
            if(letraPosicao == 'p' || letraPosicao == 'P'){
            	printw("\n");
            	scanw("%s",c_opcao_resolver);
            	//fflush(stdin);
            	__fpurge(stdin);
            	if(strcmp(c_opcao_resolver,"pass7") == 0)
            	{           		
            		i_condicao_colorir_pal_lado = 1;
            		i_cont_coord_certas = 0;           		
					// passar todas as coordenadas das palavras ultilizaadas no jogo para o vetor
					for(i =0;i < quantidadePalavras;i++)
					{
						i_palavras_corretas[i] = i_linhas_palavras_selecionadas[i];

						for(j=2; j < i_Coordenadas[i][1] * 2 + 2; j++)
						{
							i_coordenadas_palavras_certas[i_cont_coord_certas] = i_Coordenadas[i][j];
							i_cont_coord_certas++;
						}
						
					}

            		acertos = quantidadePalavras;
            		escrever();
        			v_painelDeTentativasAcertos();
        			printw("\t\tPosicão da palavra horizontal[H], vertical[V] ou diagonal[D]: ");
            		printw("%c",letraPosicao);
        			attron(COLOR_PAIR(8));
                    printw("\n\n\t*********************        VOCE GANHOU!        *********************\n");
                    attron(COLOR_PAIR(5));

        			if(c_opcao_menu == '1') // usuario entrou por login 
                    {
                        if(c_nivel_escolhido == '1')
                            s_rankeados_aux[i_condicao_login_aceito].i_vitorias_n1++; //i_condicao_login_aceito == posição do usuario no vetor 
                        else if(c_nivel_escolhido == '2')
                            s_rankeados_aux[i_condicao_login_aceito].i_vitorias_n2++;
                        else if(c_nivel_escolhido == '3')
                            s_rankeados_aux[i_condicao_login_aceito].i_vitorias_n3++;
                        
                        s_rankeados_aux[i_condicao_login_aceito].i_numero_jogos++; 
                    }
                    
                    /*if(c_opcao_menu == '2') // usuario entrou por cadastro
                    {
                        strcpy(s_rankeados_aux[i_cont_cadastrados - 1].c_username, usuario.c_username);
                        if(c_nivel_escolhido == '1')
                            s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n1++;
                        else if(c_nivel_escolhido == '2')
                            s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n2++;
                        else if(c_nivel_escolhido == '3')
                            s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n3++;
            
                        s_rankeados_aux[i_cont_cadastrados - 1].i_numero_jogos++;
                    }*/
        			v_pause();
			        v_novoJogo();
            	}
            	else
            	{
            		i_condicao_colorir_pal_lado = 0; 
            		printw("\nIsso não funcionou.\n");
 					v_pause();
	           	}            	
            }

            v_mensagemDeErro();
            escrever();
            v_painelDeTentativasAcertos();
        }while(i_condicao_continuar);


        do{
            printw("\n\t\t[Linha,Coluna]: ");
            scanw("%d,%d",&linha,&coluna); 
            __fpurge(stdin);
            //fflush(stdin);
            

            i_condicao_continuar = 1;
            if(((linha >= 0) && (linha <= 21)) && ((coluna >= 0) && (coluna <= 32)))
            {
                break;    
            }
            
            printw("As coordenadas sairam do perímetro previsto! \n");
            printw("Se continuar assim, será difícil ganhar!\n");
            printw("Tente novamente!\n");
            v_pause();
            clear();
            tentativas--;    
            escrever();
            v_painelDeTentativasAcertos();
            do{
                printw("\t\tPosicão da palavra horizontal[H], vertical[V] ou diagonal[D]: ");
                letraPosicao = getch();
                __fpurge(stdin);
                //fflush(stdin);
                i_condicao_continuar_s = 1;
                if(letraPosicao == 'h' || letraPosicao == 'H'){
                    posicaoPalavra = (int) 'H';
                    break;
                }

                if(letraPosicao == 'v' || letraPosicao == 'V'){
                    posicaoPalavra = (int) 'V';
                    break;
                }

                /*if(letraPosicao == 'd' || letraPosicao == 'D'){
                    posicaoPalavra = (int) 'D';
                    break;
                }*/
                v_mensagemDeErro();
                escrever();
                v_painelDeTentativasAcertos();
            }while(i_condicao_continuar_s);
            
        }while(i_condicao_continuar);


        i_condicao_acertou = 0;
        for(i=0;i < quantidadePalavras;i++)
        {
            if(i_Coordenadas[i][0] == posicaoPalavra)
            {
                if((i_Coordenadas[i][2] == linha) && (i_Coordenadas[i][3] == coluna))
                {
                    if(acertos >1)
                    {
                        i_condicao_reticao = 0;
                        for(n = 0;n < acertos;n++)
                        {
                            if(i == i_palavras_corretas[n])
                                i_condicao_reticao = 1;
                        }
                    }    

                    if(i_condicao_reticao){
                        printw("Você já encontrou essa palavra. Não ira ganhar pontos novamente.\n");
                        v_pause();
                        break;
                    }
                    else
                    {
                        i_condicao_acertou = 1;
                        acertos++;
                        v_coordenadas_certas();
                        if(acertos == quantidadePalavras)
                        {
                            attron(COLOR_PAIR(8));
                            printw("\t*********************        VOCE GANHOU!        *********************\n");
                            attron(COLOR_PAIR(5));

                            if(c_opcao_menu == '1') // usuario entrou por login 
                            {
                                if(c_nivel_escolhido == '1')
                                    s_rankeados_aux[i_condicao_login_aceito].i_vitorias_n1++; //i_condicao_login_aceito == posição do usuario no vetor 
                                else if(c_nivel_escolhido == '2')
                                    s_rankeados_aux[i_condicao_login_aceito].i_vitorias_n2++;
                                else if(c_nivel_escolhido == '3')
                                    s_rankeados_aux[i_condicao_login_aceito].i_vitorias_n3++;
                                
                                s_rankeados_aux[i_condicao_login_aceito].i_numero_jogos++; 
                            }
                            
                            /*if(c_opcao_menu == '2') // usuario entrou por cadastro
                            {
                                strcpy(s_rankeados_aux[i_cont_cadastrados - 1].c_username, usuario.c_username);
                                if(c_nivel_escolhido == '1')
                                    s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n1++;
                                else if(c_nivel_escolhido == '2')
                                    s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n2++;
                                else if(c_nivel_escolhido == '3')
                                    s_rankeados_aux[i_cont_cadastrados - 1].i_vitorias_n3++;
                    
                                s_rankeados_aux[i_cont_cadastrados - 1].i_numero_jogos++;
                            }*/

                            v_novoJogo();
                        }
                    
                    }
                            
                }
            }
        }
        if(i_condicao_acertou){
            mvprintw(38,16,"Palavra encontrada.");
            v_pause();
        }else{
            tentativas--;       
            if(tentativas == 0)
            {
                attron(COLOR_PAIR(8));
                printw("              _         _\n");
    			printw("VOCÊ PERDEU!   \\_(0.0)_/\n");
    			attron(COLOR_PAIR(5));
	            if(c_opcao_menu == '1') // usuario entrou por login 
	            {  
	                s_rankeados_aux[i_condicao_login_aceito].i_numero_jogos++; //i_condicao_login_aceito == posição do usuario no vetor 
	            }
	            
	            /*
	            if(c_opcao_menu == '2') // usuario entrou por cadastro
	            {
	                strcpy(s_rankeados_aux[i_cont_cadastrados].c_username, usuario.c_username);
	                s_rankeados_aux[i_cont_cadastrados].i_numero_jogos++;
	            }
	            */
	    		v_novoJogo();
    		}
    		else
    		{
                printw("As coordenadas que voce digitou encontraram o mais absoluto vácuo!\n");
            	printw("Tente novamente!");
    			v_pause();
        	} 
        }
       clear();
    }
}

void v_coordenadas_certas()
{
    i_palavras_corretas[acertos-1] = i;
    for(o = 2; o < i_Coordenadas[i][1] * 2 + 2; o++)
    {
        i_coordenadas_palavras_certas[i_cont_coord_certas] = i_Coordenadas[i][o];
        i_cont_coord_certas++;     
    }
}

void sair()
{
    endwin(); //finalizando um programa com a biblioteca curses
    exit(0);  
} 

void v_novoJogo()
{    
    if((f_arquivo_cadastrados =  fopen("arquivo_cadastrados.txt","wb")) == NULL) {
        printw("Erro na abertura do arquivo cadastrados!\n");
        sair();
    }
    fwrite(s_rankeados_aux, sizeof(struct player), i_cont_cadastrados, f_arquivo_cadastrados);    
    fclose(f_arquivo_cadastrados); 	

	if((f_arquivo_cont_cadastrados = fopen("main_dados.txt","wb")) == NULL) {
        printw("Erro na abertura do arquivo com a quanidade de usuarios cadastrados.\n");
        sair();
    }
    fwrite(&i_cont_cadastrados, sizeof(int),1,f_arquivo_cont_cadastrados);
    fclose(f_arquivo_cont_cadastrados);

    v_pause();
    do
    {
         clear();
        mvprintw(1,2,"Dejesa retornar ao menu principal ? [S][N]: ");
        c_resposta_jogar = getch();		
        __fpurge(stdin);
        //fflush(stdin);
        if(c_resposta_jogar == 's' || c_resposta_jogar == 'S')
        {
            clear();
            main();
        }
        else
        {
            if((c_resposta_jogar == 'n') ||(c_resposta_jogar =='N'))
            {
                clear();
                sair();
            }            
        }
    }while(1);     

}

/********************************************************************************************************************************/
/**                                          MODO DE ESCRITA                                                                    */
/********************************************************************************************************************************/

void horizontalDireitaEsquerda()
{
    do{
       i_condicao_sobrepor = 0;
        n = rand()%21;
        m = rand()%(32-tamanhoPalavra);
        for(i=0;i <tamanhoPalavra;i++){
            if((c_principal[n][(m+tamanhoPalavra)-i] != '.') && (c_principal[n][(m+tamanhoPalavra)-i] != c_palavras[numLinha][i]))
                i_condicao_sobrepor = 1;
        }
    }while (i_condicao_sobrepor);

    for(i=0;i <tamanhoPalavra;i++){
        c_principal[n][(m+tamanhoPalavra)-i] = c_palavras[numLinha][i];
        x = n;
        y = (m+tamanhoPalavra)-i;
        coordenadasPalavras();
    }
    contCoordenadas = 2;
}

void horizontalEsquerdaDireita()
{
    do{
        i_condicao_sobrepor =0 ;
        n = rand()%21;
        m = rand()%(32-tamanhoPalavra);
        for(i=0;i < tamanhoPalavra;i++){
            if((c_principal[n][m+i] != '.') && (c_principal[n][m+i] != c_palavras[numLinha][i]))
                i_condicao_sobrepor = 1;
        }
    }while (i_condicao_sobrepor);

    for(i=0;i < tamanhoPalavra;i++){
        c_principal[n][m+i] = c_palavras[numLinha][i];
        x = n;
        y = m + i;
        coordenadasPalavras();
    }
    contCoordenadas = 2;
}

void verticalSuperiorInferior()
{
    do{
        i_condicao_sobrepor = 0;
        n = rand()%(21-tamanhoPalavra);
        m = rand()%32;
        for(i=0;i < tamanhoPalavra;i++){
            if((c_principal[n+i][m] != '.') && (c_principal[n+i][m] != c_palavras[numLinha][i]))
                    i_condicao_sobrepor = 1;
        }
    }while(i_condicao_sobrepor);

    for(i=0;i < tamanhoPalavra;i++){
        c_principal[n+i][m] = c_palavras[numLinha][i];
        x = n+i;
        y = m;
        coordenadasPalavras();
    }
    contCoordenadas = 2;
}

void verticalInferiorSuperior()
{
    do{
        i_condicao_sobrepor = 0;
        n = rand()%(21-tamanhoPalavra);
        m = rand()%32;
        for(i=0;i < tamanhoPalavra;i++){
            if((c_principal[(n+tamanhoPalavra)-i][m] != '.') && 
            	(c_principal[(n+tamanhoPalavra)-i][m] != c_palavras[numLinha][i]))
                i_condicao_sobrepor = 1;
        }
    }while(i_condicao_sobrepor);

    for(i=0;i < tamanhoPalavra;i++){
        c_principal[(n+tamanhoPalavra)-i][m] = c_palavras[numLinha][i];
        x = (n+tamanhoPalavra)-i;
        y = m;
        coordenadasPalavras();
    }
    contCoordenadas = 2;
}

void diagonalPrincipalSuperiorInferior()
{
    do{
        i_condicao_sobrepor = 0;
        n = rand()%(21-tamanhoPalavra);
        m = rand()%(32-tamanhoPalavra);
        contDiagonal = 0;
        for(i=n;i <(n+tamanhoPalavra);i++){
            for(j=m;j <(m+tamanhoPalavra);j++){
                if((i-n)==(j-m)){
                    if((c_principal[i][j] != '.') &&
                    (c_principal[i][j] != c_palavras[numLinha][contDiagonal]))
                        i_condicao_sobrepor = 1;
                contDiagonal++;
                }
            }
        }
    }while(i_condicao_sobrepor);

    contDiagonal = 0;
    for(i= n;i <(n+tamanhoPalavra);i++){
        for(j=m;j <(m+tamanhoPalavra);j++){
            if((i-n)==(j-m)){
                c_principal[i][j] = c_palavras[numLinha][contDiagonal];
                contDiagonal++;
                x = i;
                y = j;
                coordenadasPalavras();
            }
        }
    }
    contCoordenadas = 2;
}

void diagonalPrincipalInferiorSuperior()
{
    contCoordenadas = ((tamanhoPalavra*2)+1);
    do{
        i_condicao_sobrepor = 0;
        n = rand()%(21-tamanhoPalavra);
        m = rand()%(32-tamanhoPalavra);
        contDiagonal = (tamanhoPalavra-1);
        for(i=n;i <(n+tamanhoPalavra);i++){
            for(j=m;j <(m+tamanhoPalavra);j++){
                if((i-n)==(j-m)){
                    if((c_principal[i][j] != '.')&&
                    (c_principal[i][j] != c_palavras[numLinha][contDiagonal]))
                            i_condicao_sobrepor = 1;
                    contDiagonal--;
                }
            }
        }
    }while(i_condicao_sobrepor);

    contDiagonal = (tamanhoPalavra-1);
    for(i= n;i <(n+tamanhoPalavra);i++){
        for(j=m;j <(m+tamanhoPalavra);j++){
            if((i-n)==(j-m)){
                c_principal[i][j] = c_palavras[numLinha][contDiagonal];
                contDiagonal--;
                x = i;
                y = j;
                coordenadasPalavrasInferiorSuperior();
            }
        }
    }
    contCoordenadas = 2;
}

void diagonalSecundariaSuperiorInferior()
{
    do{
        i_condicao_sobrepor = 0;
        n = rand()%(21-tamanhoPalavra);
        m = rand()%(32-tamanhoPalavra);

        contDiagonal = 0;
        for(i=n;i <(n+tamanhoPalavra);i++){
            for(j=m;j <(m+tamanhoPalavra);j++){
                if(((tamanhoPalavra-1)-(i-n))==(j-m)){
                    if((c_principal[i][j] != '.') &&
                    (c_principal[i][j] != c_palavras[numLinha][contDiagonal]))
                            i_condicao_sobrepor = 1;
                    contDiagonal++;
                }
            }
        }
    }while(i_condicao_sobrepor);

    contDiagonal = 0;
    for(i= n;i <(n+tamanhoPalavra);i++){
        for(j=m;j <(m+tamanhoPalavra);j++){
            if(((tamanhoPalavra-1)-(i-n))==(j-m)){
                c_principal[i][j] = c_palavras[numLinha][contDiagonal];
                contDiagonal++;
                x = i;
                y = j;
                coordenadasPalavras();
            }
        }
    }
    contCoordenadas = 2;
}

void diagonalSecundariaInferiorSuperior()
{
    contCoordenadas = ((tamanhoPalavra*2)+1);
    do{
        i_condicao_sobrepor = 0;
        n = rand()%(21-tamanhoPalavra);
        m = rand()%(32-tamanhoPalavra);

        contDiagonal = (tamanhoPalavra-1);
        for(i=n;i <(n+tamanhoPalavra);i++){
            for(j=m;j <(m+tamanhoPalavra);j++){
                if(((tamanhoPalavra-1)-(i-n))==(j-m)){
                    if((c_principal[i][j] != '.') &&
                    (c_principal[i][j] != c_palavras[numLinha][contDiagonal]))
                            i_condicao_sobrepor = 1;
                    contDiagonal--;
                }
            }
        }
    }while(i_condicao_sobrepor);

    contDiagonal = (tamanhoPalavra-1);
    for(i= n;i <(n+tamanhoPalavra);i++){
        for(j=m;j <(m+tamanhoPalavra);j++){
            if(((tamanhoPalavra-1)-(i-n))==(j-m)){
                c_principal[i][j] = c_palavras[numLinha][contDiagonal];
                contDiagonal--;
                x = i;
                y = j;
                coordenadasPalavrasInferiorSuperior();
            }
        }
    }
    contCoordenadas = 2;
}

void coordenadasPalavras()
{
    i_Coordenadas[cont][contCoordenadas] = x;
    contCoordenadas++;
    i_Coordenadas[cont][contCoordenadas] = y;
    contCoordenadas++;
}

void coordenadasPalavrasInferiorSuperior()
{
    i_Coordenadas[cont][contCoordenadas] = y;
    contCoordenadas--;
    i_Coordenadas[cont][contCoordenadas] = x;
    contCoordenadas--;
}



