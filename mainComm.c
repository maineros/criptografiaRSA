#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>

void menuOpcoes();
void limparTela();
void barraDeCarregamento(char textoCabecalho[], char textoFinal[], int duracao);
void gerarChavePublica();
void receberPQE(mpz_t p, mpz_t q, mpz_t e, mpz_t n, mpz_t phi);
int ePrimo(mpz_t num);
int saoCoprimos(mpz_t a, mpz_t b);
void criptografarMensagem();
void descriptografarMensagem();
void exibirMensagem();

int main()
{
    menuOpcoes();
    return 0;
}

// apresenta o menu de opcoes para o usuario e recebe a acao desejada
void menuOpcoes()
{
    limparTela();

    while (1)
    {
        limparTela();

        printf("=== PROJETO DE MATEMATICA DISCRETA: CRIPTOGRAFIA RSA ===\n\n");
        printf("Escolha uma opcao:\n");
        printf("\t[1] - Gerar chave publica\n\t[2] - Criptografar mensagem\n\t[3] - Descriptografar mensagem\n\t[4] - Ver mensagem descriptografada\n\t[5] - Sair do programa\n\n");
        printf("Digite aqui: ");

        int opcao;

        if (scanf("%d", &opcao) != 1)
        {
            getchar();

            opcao = 0;
        }

        switch (opcao)
        {
        case 1:
            gerarChavePublica();
            break;
        case 2:
            criptografarMensagem();
            break;
        case 3:
            descriptografarMensagem();
            break;
        case 4:
            exibirMensagem();
            break;
        case 5:
            limparTela();
            printf("\nA equipe agradece pelo uso do programa!\n\n");
            exit(0);
        default:
            limparTela();
            printf("Opcao invalida. Tente novamente digitando um numero valido.\n");
            system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
            break;
        }
    }

    return;
}

// limpa a tela do terminal
void limparTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return;
}

// animacao de carregamento para o terminal
void barraDeCarregamento(char textoCabecalho[], char textoFinal[], int duracao) 
{
    const char spinner[] = "|/-\\";
    int i = 0;

    printf("\n");

    while (duracao > 0) 
    {
        printf("\r%s %c", textoCabecalho, spinner[i]);
        fflush(stdout);
        usleep(100000); // 100 milissegundos
        i = (i + 1) % 4;
        duracao--;
    }

    printf("\r%s                                           \n", textoFinal);
}

// gera a chave publica com 'e' e 'n' e a imprime em um arquivo .txt
void gerarChavePublica()
{
    limparTela();
    printf("\nOpcao escolhida: Gerar chave publica.\n\n");

    // declarando as variaveis 'p', 'q', 'e', 'n' e 'phi' com a lib gmp
    mpz_t p, q, e, n, phi;
    mpz_inits(p, q, e, n, phi, NULL);

    // calculando cada variavel na funcao
    receberPQE(p, q, e, n, phi);

    // salvando a chave publica em um arquivo
    FILE *chavePublicaArq = fopen("chave_publica.txt", "w");
    if (chavePublicaArq == NULL)
    {
        printf("\nErro ao criar o arquivo de chave publica.\n\n");
        mpz_clears(p, q, e, n, phi, NULL);

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");

        return;
    }

    // adicionando os valores da chave publica ao arquivo e fechando-o
    gmp_fprintf(chavePublicaArq, "p = %Zd\nq = %Zd\nn = %Zd\ne = %Zd\n", p, q, n, e);
    fclose(chavePublicaArq);

    // adicionando animacao de carregamento
    barraDeCarregamento("Geracao de chave publica em andamento...", "Geracao concluida!", 50);

    printf("\nChave publica gerada com sucesso e salva em chave_publica.txt\n\n");
    mpz_clears(p, q, e, n, phi, NULL);

    // mostra a mensagem no terminal e retorna ao menu principal
    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");

    return;
}

// recebe e calcula os valores de 'p', 'q', 'e', 'n' e 'phi'
void receberPQE(mpz_t p, mpz_t q, mpz_t e, mpz_t n, mpz_t phi)
{
    printf("Digite dois numeros primos p e q: ");
    gmp_scanf("%Zd %Zd", p, q);

    // verificar validade dos primos
    while (!ePrimo(p) || !ePrimo(q))
    {
        if (!ePrimo(p) && !ePrimo(q))
            printf("Os numeros p e q nao sao numeros primos; corrija-os e insira os valores novamente.\n\n");
        else if (!ePrimo(p))
            printf("O numero p nao eh um numero primo; corrija-o e insira os valores novamente.\n\n");
        else if (!ePrimo(q))
            printf("O numero q nao eh um numero primo; corrija-o e insira os valores novamente.\n\n");
        else
            printf("Numeros invalidos.\n\n");

        printf("Digite dois numeros primos p e q: ");
        gmp_scanf("%Zd %Zd", p, q);
    }

    // atribuindo valor a 'n'
    mpz_mul(n, p, q);

    // declarando as variaveis para p - 1 e q - 1
    mpz_t pMenos1, qMenos1;
    mpz_inits(pMenos1, qMenos1, NULL);

    // calculando e atribuindo valor a 'phi'
    mpz_sub_ui(pMenos1, p, 1);
    mpz_sub_ui(qMenos1, q, 1);
    mpz_mul(phi, pMenos1, qMenos1);
    
    gmp_printf("Digite um expoente \"e\" relativamente primo a %Zd (p-1)*(q-1): ", phi);
    gmp_scanf("%Zd", e);

    while (!saoCoprimos(e, phi))
    {
        gmp_printf("O numero e informado nao eh relativamente primo a %Zd; corrija-o e insira o valor novamente.\n\n", phi);

        gmp_printf("Digite um numero e relativamente primo a %Zd: ", phi);
        gmp_scanf("%Zd", e);
    }

    mpz_clears(pMenos1, qMenos1, NULL);

    return;
}

// retorna 1 se o numero for primo e 0 se nao
int ePrimo(mpz_t num)
{
    // retorna 2 se o numero for primo de certeza, 1 se provavelmente for primo e 0 se nao for primo
    return mpz_probab_prime_p(num, 50) > 0;
}

// verifica se dois numeros sao primos
int saoCoprimos(mpz_t a, mpz_t b)
{
    mpz_t mdc;
    mpz_init(mdc);

    // calcula o mdc entre dois numeros
    mpz_gcd(mdc, a, b);

    // mpz_cmp_ui compara se dois numeros sao iguais. nesse caso, se o mdc entre os dois numeros é igual a 1
    int resultado = mpz_cmp_ui(mdc, 1) == 0;

    mpz_clear(mdc);
    return resultado;
}

// criptografa o texto dado pelo usuario e o imprime em um arquivo .txt
void criptografarMensagem()
{
    limparTela();
    printf("\nOpcao escolhida: Criptografar mensagem.\n\n");
    
    // inicializando 'p', 'q' e 'e' com a lib gmp para extrai-los do arquivo de chave publica
    mpz_t p, q, e, n;
    mpz_inits(p, q, e, n, NULL);

    // pergunta ao usuario qual opcao de obtencao da chave publica ele prefere
    printf("Deseja inserir a chave publica manualmente ou extrai-la de um arquivo chave_publica.txt?\n");
    printf("Digite aqui (M para inserir manualmente ou A para arquivo): ");
    
    char escolha;
    scanf(" %c", &escolha);

    switch (escolha)
    {
    case 'M':
        printf("\n\nDigite o valor de \"e\": ");
        gmp_scanf("%Zd", e);
        printf("\nDigite o valor de \"n\": ");
        gmp_scanf("%Zd", n);

        break;
    case 'A':
        // abrindo o arquivo de chave publica para extrair 'p', 'q', 'n' e 'e'
        FILE *chavePublicaArq = fopen("chave_publica.txt", "r+");

        // verificando se ha alguma chave publica no arquivo indicado
        if (chavePublicaArq == NULL)
        {
            printf("\nAinda nao foi criada uma chave publica!\n\n");

            mpz_clears(p, q, e, n, NULL);

            system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
            return;
        }

        // recebendo os valores da chave publica 'p', 'q', 'n' e 'e'
        gmp_fscanf(chavePublicaArq, "p = %Zd\nq = %Zd\nn = %Zd\ne = %Zd\n", p, q, n, e);

        // fechando o arquivo de chave publica
        fclose(chavePublicaArq);

        break;
    default:
        printf("O caractere inserido nao eh valido.\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    // limpando buffer para retirar possivel "\n" do fscanf
    getchar();

    // recebendo o texto a ser criptografado
    char texto[1001];
    printf("\n\nInsira o texto a ser criptografado (ate 1000 caracteres): ");
    fgets(texto, 1000, stdin);

    // achando o tamanho do texto
    int tamanho = strlen(texto);

    // inicializando os arrays para o texto puro e o texto criptografado
    mpz_t textoPuro, textoCriptografado;
    mpz_inits(textoPuro, textoCriptografado, NULL);

    // criando o arquivo para o texto criptografado
    FILE *textoCriptografadoArq = fopen("texto_criptografado.txt", "w");

    // tratamento de erro para criacao do arquivo de texto criptografado
    if (textoCriptografadoArq == NULL)
    {
        printf("\nErro ao criar o arquivo com o texto criptografado.\n\n");

        mpz_clears(textoPuro, textoCriptografado, NULL);

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    // criptografando o texto e adicionando-o ao arquivo
    for (int i = 0; i < tamanho; i++)
    {
        // convertendo o caractere do texto para um inteiro sem sinal e armazenando o valor em textoPuro
        mpz_set_ui(textoPuro, (unsigned char)texto[i]);

        // criptografando o texto utilizando da exponenciacao modular rapida (num^e mod n)
        mpz_powm(textoCriptografado, textoPuro, e, n);

        // imprimindo o texto criptografado no arquivo
        gmp_fprintf(textoCriptografadoArq, "%Zd ", textoCriptografado);
    }

    mpz_clears(p, q, e, n, textoPuro, textoCriptografado, NULL);

    // fechando o arquivo para garantir que o texto foi impresso corretamente
    fclose(textoCriptografadoArq);

    // adicionando animacao de carregamento
    barraDeCarregamento("Criptografia em andamento...", "Criptografia concluida!", 50);

    printf("\nTexto criptografado com sucesso e salvo em texto_criptografado.txt\n\n");
    
    // mostra a mensagem no terminal e retorna ao menu principal
    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");

    return;
}

// descriptografa o texto dado pelo usuario e o imprime em um arquivo .txt
void descriptografarMensagem()
{
    limparTela();
    printf("\nOpcao escolhida: Descriptografar mensagem.\n");

    // inicializando 'p', 'q' e 'e' com a lib gmp para extrai-los do arquivo de chave publica
    mpz_t p, q, e, n, phi, d;
    mpz_inits(p, q, e, n, phi, d, NULL);

    // abrindo o arquivo de chave publica para extrair 'p', 'q' e 'e'
    FILE *chavePublicaArq = fopen("chave_publica.txt", "r+");

    // verificando se ha alguma chave publica no arquivo indicado
    if (chavePublicaArq == NULL)
    {
        printf("\nAinda nao foi criada uma chave publica!\n\n");

        mpz_clears(p, q, e, n, NULL);

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    // recebendo os valores da chave publica 'p', 'q' e 'e'
    gmp_fscanf(chavePublicaArq, "p = %Zd\nq = %Zd\nn = %Zd\ne = %Zd\n", p, q, n, e);

    // declarando as variaveis para p - 1 e q - 1
    mpz_t pMenos1, qMenos1;
    mpz_inits(pMenos1, qMenos1, NULL);

    // calculando e atribuindo valor a 'phi'
    mpz_sub_ui(pMenos1, p, 1);
    mpz_sub_ui(qMenos1, q, 1);
    mpz_mul(phi, pMenos1, qMenos1);

    // limpando buffer para retirar possivel "\n" do fscanf
    getchar();

    // fechando o arquivo de chave publica
    fclose(chavePublicaArq);

    // calculando o inverso modular de e mod phi (d) [sendo phi = (p - 1) * (q - 1)]
    mpz_invert(d, e, phi);

    // abrindo arquivo com o texto criptografado
    FILE *textoCriptografadoArq = fopen("texto_criptografado.txt", "r");

    // verificando se ha algum texto criptografado no arquivo dado
    if (textoCriptografadoArq == NULL)
    {
        printf("\nAinda nao existe um texto para ser descriptografado!\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    // criando o arquivo para o texto descriptografado
    FILE *textoDescriptografadoArq = fopen("texto_descriptografado.txt", "w");
    
    // tratamento de erro para criacao do arquivo de texto criptografado
    if (textoDescriptografadoArq == NULL)
    {
        printf("\nErro ao criar o arquivo do texto descriptografado.\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    // inicializando variaveis para guardar o texto criptografado retirado do arquivo e o texto descriptografado
    mpz_t textoCriptografado, textoDescriptografado;
    mpz_inits(textoCriptografado, textoDescriptografado, NULL);

    // lendo o arquivo enquanto nao chegar ao seu final (End of File)
    while (gmp_fscanf(textoCriptografadoArq, "%Zd", textoCriptografado) != EOF)
    {
        // descriptografando o texto utilizando da exponenciacao modular rapida (num^d mod n)
        mpz_powm(textoDescriptografado, textoCriptografado, d, n);

        // imprimindo o texto descriptografado no arquivo
        gmp_fprintf(textoDescriptografadoArq, "%c", (char)mpz_get_ui(textoDescriptografado));
    }

    // fechando o arquivo do texto criptografado
    fclose(textoCriptografadoArq);

    // fechando o arquivo para garantir que o texto foi impresso corretamente
    fclose(textoDescriptografadoArq);

    // adicionando animacao de carregamento
    barraDeCarregamento("Descriptografia em andamento...", "Descriptografia concluida!", 50);

    printf("\nTexto descriptografado com sucesso e salvo em texto_descriptografado.txt\n\n");

    // mostra a mensagem no terminal e retorna ao menu principal
    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
    return;
}

// exibe a mensagem descriptografada na tela
void exibirMensagem()
{
    limparTela();
    printf("\nOpcao escolhida: Exibir mensagem descriptografada.\n");

    // abrindo o arquivo para extrair o texto descriptografado
    FILE *textoDescriptografadoArq = fopen("texto_descriptografado.txt", "r");

    // verificando se ha algum texto descriptografado no arquivo dado
    if (textoDescriptografadoArq == NULL)
    {
        printf("\nAinda nao existe um texto descriptografado para ser impresso!\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");

        return;
    }

    // extraindo o texto do arquivo e colocando-o em um array para impressao
    char textoDescriptografado[1001];
    int tamanho = 0;
    for (int i = 0; fscanf(textoDescriptografadoArq, "%c", &textoDescriptografado[i]) != EOF; i++)
    {
        tamanho++;
    }

    // adicionando animacao de carregamento
    barraDeCarregamento("Exibicao da mensagem em andamento...", "                        ", 50);

    printf("Mensagem: ");

    // imprimindo o texto na tela
    for (int i = 0; i < tamanho; i++)
    {
        printf("%c", textoDescriptografado[i]);
    }

    printf("\nMensagem impressa com sucesso!\n\n");

    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
    return;
}
