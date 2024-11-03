# CRIPTOGRAFIA RSA
## Projeto para obtenção de nota parcial na disciplina de Matemática Discreta do curso de Ciência da Computação IC/UFAL.

Para rodar o código, é necessário ter o WSL instalado. O WSL (Windows Subsystem for Linux) é uma ferramenta da Microsoft que permite rodar uma distribuição Linux diretamente no Windows; ele será utilizado para maior comodidade na instalação das bibliotecas necessárias para a compilação do código.

### Instalação do WSL
* Abra o Prompt de Comando ou o Windows Powershell;
* Digite ```wsl --install```;
* Reinicie a sua máquina;
* Pesquise por WSL no seu Menu Iniciar e abra-o;
* Caso queira criar um usuário e uma senha, apenas os digite quando for solicitado, senão, apenas feche o WSL e abra-o novamente;
* Digite ```sudo apt-get update``` para atualizar todos os drives e possíveis pendências desatualizadas do Linux;
* Digite ```sudo apt-get install gcc``` para instalar o compilador da linguagem C;
* Digite ```gcc```; caso obtenha como retorno ```FATAL ERROR```, a instalação foi concluída com sucesso;

### Uso do WSL
* Digite ```cd /mnt```;
* Siga digitando ```cd nome_da_sua_pasta``` até chegar na pasta com o código a ser compilado; também pode ser utilizado o caminho direto, como ```cd /mnt/c/seu/caminho/completo/aqui```;
* Já no diretório desejado, digite ```gcc nome_do_arquivo.c -o nome_do_arquivo_executavel```; o nome do arquivo executável é de sua preferência **e não possui nenhuma extensão no final (.exe, etc)**;
* Caso não surja nenhum aviso na tela, seu arquivo foi compilado com sucesso;
* Para executar o arquivo compilado, digite ```./nome_do_arquivo_executavel```;
* Após isso, seu arquivo deverá ser executado corretamente.

## Instalação e uso do programa
* Digite no terminal do WSL o comando ```git clone https://github.com/maineros/criptografiaRSA```;
* Acesse a pasta do projeto com ```cd nome_da_pasta_onde_voce_salvou```;
* Instale a biblioteca **GMP (GNU Multiple-Precision Library)** com o comando ```sudo get-apt install libgmp-dev```;
* Compile o programa com ```gcc main.c -o main -lgmp``` e execute com ```./main```.