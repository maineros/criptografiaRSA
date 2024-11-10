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

void limparTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    return;
}

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

void gerarChavePublica()
{
    limparTela();
    printf("\nOpcao escolhida: Gerar chave publica.\n\n");

    mpz_t p, q, e, n, phi;
    mpz_inits(p, q, e, n, phi, NULL);

    receberPQE(p, q, e, n, phi);

    FILE *chavePublicaArq = fopen("chave_publica.txt", "w");
    if (chavePublicaArq == NULL)
    {
        printf("\nErro ao criar o arquivo de chave publica.\n\n");
        mpz_clears(p, q, e, n, phi, NULL);

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    gmp_fprintf(chavePublicaArq, "p = %Zd\nq = %Zd\nn = %Zd\ne = %Zd\n", p, q, n, e);
    fclose(chavePublicaArq);

    barraDeCarregamento("Geracao de chave publica em andamento...", "Geracao concluida!", 50);

    printf("\nChave publica gerada com sucesso e salva em chave_publica.txt\n\n");
    mpz_clears(p, q, e, n, phi, NULL);

    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
    return;
}

void receberPQE(mpz_t p, mpz_t q, mpz_t e, mpz_t n, mpz_t phi)
{
    printf("Digite dois numeros primos p e q: ");
    gmp_scanf("%Zd %Zd", p, q);

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

    mpz_mul(n, p, q);

    mpz_t pMenos1, qMenos1;
    mpz_inits(pMenos1, qMenos1, NULL);

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

int ePrimo(mpz_t num)
{
    return mpz_probab_prime_p(num, 50) > 0;
}

int saoCoprimos(mpz_t a, mpz_t b)
{
    mpz_t mdc;
    mpz_init(mdc);

    mpz_gcd(mdc, a, b);

    int resultado = mpz_cmp_ui(mdc, 1) == 0;

    mpz_clear(mdc);
    return resultado;
}

void criptografarMensagem()
{
    limparTela();
    printf("\nOpcao escolhida: Criptografar mensagem.\n\n");
    
    mpz_t p, q, e, n;
    mpz_inits(p, q, e, n, NULL);

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
        FILE *chavePublicaArq = fopen("chave_publica.txt", "r+");
        if (chavePublicaArq == NULL)
        {
            printf("\nAinda nao foi criada uma chave publica!\n\n");

            mpz_clears(p, q, e, n, NULL);

            system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
            return;
        }

        gmp_fscanf(chavePublicaArq, "p = %Zd\nq = %Zd\nn = %Zd\ne = %Zd\n", p, q, n, e);

        fclose(chavePublicaArq);

        break;
    default:
        printf("O caractere inserido nao eh valido.\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    getchar();

    char texto[1001];
    printf("\n\nInsira o texto a ser criptografado (ate 1000 caracteres): ");
    fgets(texto, 1000, stdin);

    int tamanho = strlen(texto);

    mpz_t textoPuro, textoCriptografado;
    mpz_inits(textoPuro, textoCriptografado, NULL);

    FILE *textoCriptografadoArq = fopen("texto_criptografado.txt", "w");
    if (textoCriptografadoArq == NULL)
    {
        printf("\nErro ao criar o arquivo com o texto criptografado.\n\n");

        mpz_clears(textoPuro, textoCriptografado, NULL);

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    for (int i = 0; i < tamanho; i++)
    {
        mpz_set_ui(textoPuro, (unsigned char)texto[i]);

        mpz_powm(textoCriptografado, textoPuro, e, n);

        gmp_fprintf(textoCriptografadoArq, "%Zd ", textoCriptografado);
    }

    mpz_clears(p, q, e, n, textoPuro, textoCriptografado, NULL);

    fclose(textoCriptografadoArq);

    barraDeCarregamento("Criptografia em andamento...", "Criptografia concluida!", 50);

    printf("\nTexto criptografado com sucesso e salvo em texto_criptografado.txt\n\n");
    
    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
    return;
}

void descriptografarMensagem()
{
    limparTela();
    printf("\nOpcao escolhida: Descriptografar mensagem.\n");

    mpz_t p, q, e, n, phi, d;
    mpz_inits(p, q, e, n, phi, d, NULL);

    FILE *chavePublicaArq = fopen("chave_publica.txt", "r+");
    if (chavePublicaArq == NULL)
    {
        printf("\nAinda nao foi criada uma chave publica!\n\n");

        mpz_clears(p, q, e, n, NULL);

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    gmp_fscanf(chavePublicaArq, "p = %Zd\nq = %Zd\nn = %Zd\ne = %Zd\n", p, q, n, e);

    mpz_t pMenos1, qMenos1;
    mpz_inits(pMenos1, qMenos1, NULL);

    mpz_sub_ui(pMenos1, p, 1);
    mpz_sub_ui(qMenos1, q, 1);
    mpz_mul(phi, pMenos1, qMenos1);

    getchar();

    fclose(chavePublicaArq);

    mpz_invert(d, e, phi);

    FILE *textoCriptografadoArq = fopen("texto_criptografado.txt", "r");
    if (textoCriptografadoArq == NULL)
    {
        printf("\nAinda nao existe um texto para ser descriptografado!\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    FILE *textoDescriptografadoArq = fopen("texto_descriptografado.txt", "w");
    if (textoDescriptografadoArq == NULL)
    {
        printf("\nErro ao criar o arquivo do texto descriptografado.\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    mpz_t textoCriptografado, textoDescriptografado;
    mpz_inits(textoCriptografado, textoDescriptografado, NULL);

    while (gmp_fscanf(textoCriptografadoArq, "%Zd", textoCriptografado) != EOF)
    {
        mpz_powm(textoDescriptografado, textoCriptografado, d, n);

        gmp_fprintf(textoDescriptografadoArq, "%c", (char)mpz_get_ui(textoDescriptografado));
    }

    fclose(textoCriptografadoArq);
    fclose(textoDescriptografadoArq);

    barraDeCarregamento("Descriptografia em andamento...", "Descriptografia concluida!", 50);

    printf("\nTexto descriptografado com sucesso e salvo em texto_descriptografado.txt\n\n");

    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
    return;
}

void exibirMensagem()
{
    limparTela();
    printf("\nOpcao escolhida: Exibir mensagem descriptografada.\n");

    FILE *textoDescriptografadoArq = fopen("texto_descriptografado.txt", "r");
    if (textoDescriptografadoArq == NULL)
    {
        printf("\nAinda nao existe um texto descriptografado para ser impresso!\n\n");

        system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
        return;
    }

    char textoDescriptografado[1001];
    int tamanho = 0;
    for (int i = 0; fscanf(textoDescriptografadoArq, "%c", &textoDescriptografado[i]) != EOF; i++)
    {
        tamanho++;
    }

    barraDeCarregamento("Exibicao da mensagem em andamento...", "                        ", 50);

    printf("Mensagem: ");

    for (int i = 0; i < tamanho; i++)
    {
        printf("%c", textoDescriptografado[i]);
    }

    printf("\nMensagem impressa com sucesso!\n\n");

    system("read -p 'Retornando ao menu principal. Pressione Enter para continuar...' var");
    return;
}
